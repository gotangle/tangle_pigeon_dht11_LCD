#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "driver/rtc_io.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "rom/uart.h"

// sntp
#include "lwip/apps/sntp.h"
#include "lwip/err.h"

// wifi
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"

#include "message_builder.h"

// temp & humid sensor data
#include "dht11_rmt.h"

// The BOOT butten on board, push on LOW.
#define WAKE_UP_GPIO GPIO_NUM_0

// ESP32-DevKitC V4 onboard LED
#define BLINK_GPIO GPIO_NUM_2

static const char *TAG = "TanglePigeon";

char sensor_data[40];

static EventGroupHandle_t wifi_event_group;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const static int CONNECTED_BIT = BIT0;

// log the sleep time.
static RTC_DATA_ATTR struct timeval sleep_enter_time;

static esp_err_t wifi_event_handler(void *ctx, system_event_t *event) {
  switch (event->event_id) {
    case SYSTEM_EVENT_STA_START:
      esp_wifi_connect();
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
	  lcd_print(1, 8, COLOR_BLACK, "IP:%s",ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      /* This is a workaround as ESP32 WiFi libs don't currently
             auto-reassociate. */
      esp_wifi_connect();
      xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
      break;
    default:
      break;
  }
  return ESP_OK;
}

static void wifi_conn_init(void) {
  tcpip_adapter_init();
  wifi_event_group = xEventGroupCreate();
  ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  wifi_config_t wifi_config = {
      .sta =
          {
              .ssid = CONFIG_WIFI_SSID,
              .password = CONFIG_WIFI_PASSWORD,
          },
  };
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());
}

static void initialize_nvs() {
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
}

static void restart_in(int second) {
  for (int i = second; i >= 0; i--) {
    ESP_LOGI(TAG, "Restarting in %d seconds...", i);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  ESP_LOGI(TAG, "Restarting now.\n");
  fflush(stdout);
  esp_restart();
}

static void init_time() {
  // init sntp
  ESP_LOGI(TAG, "Initializing SNTP: %s, Timezone: %s", CONFIG_SNTP_SERVER, CONFIG_SNTP_TZ);
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, CONFIG_SNTP_SERVER);
  sntp_init();

  // wait for time to be set
  time_t now = 0;
  struct tm timeinfo = {0};
  int retry = 0;
  const int retry_count = 10;
  while (timeinfo.tm_year < (2018 - 1900) && ++retry < retry_count) {
    ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    time(&now);
    localtime_r(&now, &timeinfo);
  }
  
  if (timeinfo.tm_year < (2018 - 1900)) {
    ESP_LOGE(TAG, "Sync SNPT failed...");
    lcd_print(1, 10, COLOR_RED, "Get time failed.");
    restart_in(5);
  }
}
 
static void update_time() {
  // set timezone
  char strftime_buf[32];
  struct tm timeinfo = {0};
  time_t now = 0;
  time(&now);
  setenv("TZ", CONFIG_SNTP_TZ, 1);
  tzset();
  localtime_r(&now, &timeinfo);
  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
  lcd_print(1, 2, COLOR_BLACK, "Date/Time:");
  lcd_print(1, 4, COLOR_BLACK,strftime_buf);
  ESP_LOGI(TAG, "The current date/time is: %s\n", strftime_buf);
}

static void check_receiver_address() {
  size_t addr_len = 0;
#ifdef CONFIG_MSG_RECEIVER
  addr_len = strlen(CONFIG_MSG_RECEIVER);
#endif
  if (!(addr_len == HASH_LENGTH_TRYTE + 9)) {
    lcd_print(1, 4, COLOR_RED, "Invalid hash");
    lcd_print(1, 6, COLOR_RED, "Restart in 5s");
    ESP_LOGE(TAG, "please set a valid hash(CONFIG_IOTA_RECEIVER in sdkconfig!");
    restart_in(5);
  }
}

void app_main() {
	
	const int gpio_pin    = CONFIG_DHT11_DATA_PIN;
    const int rmt_channel = 0;

    // Set up the RMT_RX module
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    dht11_rmt_rx_init(gpio_pin, rmt_channel);
	
    if(dht11_rmt_rx(gpio_pin, rmt_channel, &humidity, &temp_x10)) {
	   
    /* Clamp the inputs to enure we don't blow the buffer out */
    if(temp_x10 > 999) temp_x10 = 999;
    if(temp_x10 < -999) temp_x10 = -999;
    if(humidity > 1000) humidity = 100;
    if(humidity < 0) humidity = 0;
    
    /* Build the message */
	const char template[] = "Temperature=%i.%i*C humidity=%i%%";
    sprintf(sensor_data, template, temp_x10/10, temp_x10%10, humidity);

    } else {
        ESP_LOGI(TAG, "Sensor failure - retrying\n");
        sleep(5);
        if(dht11_rmt_rx(gpio_pin, rmt_channel, &humidity, &temp_x10)) {
			ESP_LOGI(TAG, "Temperature:%i.%i*C humidity:%i%%\n", temp_x10/10, temp_x10%10, humidity);

        } else {
          ESP_LOGI(TAG, "Sensor failure\n");
        }
    }
	
  gpio_pad_select_gpio(BLINK_GPIO);
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(BLINK_GPIO, 1);

	// init lcd
	lcd_init();
	lcd_fill_screen(COLOR_WHITE);
	
	lcd_print(1, 2, COLOR_BLACK, "Checking hash...");
	check_receiver_address();

	initialize_nvs();
  
	// init wifi
    lcd_print(1, 4, COLOR_BLACK, "Init WiFi...");
	wifi_conn_init();

	ESP_LOGI(TAG, "Connecting to WiFi network...");
	/* Wait for the callback to set the CONNECTED_BIT in the event group. */
	xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
	ESP_LOGI(TAG, "Connected to AP");
	ESP_LOGI(TAG, "IRI Node: %s, port: %d, HTTPS:%s\n", CONFIG_IRI_NODE_URI, CONFIG_IRI_NODE_PORT,
           CONFIG_ENABLE_HTTPS ? "True" : "False");
	
	lcd_print(1, 6, COLOR_BLACK, "SSID: %s", CONFIG_WIFI_SSID);

	// get time from sntp
	init_time();

	// show Receiver address
	ESP_LOGI(TAG, "Msg Address: %s\n", CONFIG_MSG_RECEIVER);
  
	//Get the derived MAC address for each network interface
		uint8_t derived_mac_addr[6] = {0};
	//Get MAC address for WiFi Station interface
		ESP_ERROR_CHECK(esp_read_mac(derived_mac_addr, ESP_MAC_WIFI_STA));
  lcd_print(1, 10, COLOR_BLACK, "MAC:%X:%X:%X:%X:%X:%X",
             derived_mac_addr[0], derived_mac_addr[1], derived_mac_addr[2],
             derived_mac_addr[3], derived_mac_addr[4], derived_mac_addr[5]);
			 
	//struct timeval now;
	//gettimeofday(&now, NULL);
	//int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;

  lcd_print(1, 12, COLOR_BLUE, "Ready to go...");
  vTaskDelay(7 * 1000 / portTICK_PERIOD_MS);
  lcd_fill_screen(COLOR_WHITE);
  
#if 0
  switch (esp_sleep_get_wakeup_cause()) {
    case ESP_SLEEP_WAKEUP_EXT0: {
      printf("Wake up from GPIO_%d\n", WAKE_UP_GPIO);
      send_message(ESP_SLEEP_WAKEUP_EXT0);
      break;
    }
    case ESP_SLEEP_WAKEUP_TIMER: {
      printf("Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
      send_message(ESP_SLEEP_WAKEUP_TIMER);
      break;
    }
    case ESP_SLEEP_WAKEUP_UNDEFINED:
      printf("Power on reset \n");
      send_message(ESP_SLEEP_WAKEUP_UNDEFINED);
      break;
    default:
      printf("Not a deep sleep reset\n");
  }

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  ESP_LOGI(TAG, "Enabling timer wakeup, %d minutes", CONFIG_WAKE_UP_TIME);
  esp_sleep_enable_timer_wakeup(CONFIG_WAKE_UP_TIME * 60000000);  // minutes

  rtc_gpio_init(WAKE_UP_GPIO);
  gpio_pullup_dis(WAKE_UP_GPIO);
  gpio_pulldown_en(WAKE_UP_GPIO);
  esp_sleep_enable_ext0_wakeup(WAKE_UP_GPIO, 0);

  // Isolate GPIO12 pin from external circuits. This is needed for modules
  // which have an external pull-up resistor on GPIO12 (such as ESP32-WROVER)
  // to minimize current consumption.
  rtc_gpio_isolate(GPIO_NUM_12);

  ESP_LOGI(TAG, "Entering deep sleep\n");
  gettimeofday(&sleep_enter_time, NULL);

  // wifi done
  ESP_ERROR_CHECK(esp_wifi_stop());

  esp_deep_sleep_start();
#else
	while(1){		
		update_time();
		// show Temp & humid data
		ESP_LOGI(TAG, "Temperature:%i.%i*C humidity:%i%%\n", temp_x10/10, temp_x10%10, humidity);
		lcd_print(1, 6, COLOR_BLACK, "Temp:%i.%iC humid:%i%%\n", temp_x10/10, temp_x10%10, humidity);
		send_message(ESP_SLEEP_WAKEUP_TIMER);
		vTaskDelay(CONFIG_WAKE_UP_TIME*60*1000 / portTICK_PERIOD_MS);		
	}
#endif
}
