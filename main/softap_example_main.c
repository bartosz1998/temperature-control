#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include <esp_http_server.h>
#include "driver/gpio.h"
#include "esp_netif_types.h"
#include "web.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

static const char *TAG = "web server";

#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_WIFI_CHANNEL   CONFIG_ESP_WIFI_CHANNEL
#define EXAMPLE_MAX_STA_CONN       CONFIG_ESP_MAX_STA_CONN

#define GPIO_LED_CONTROLLER GPIO_NUM_26 // Controller work
#define GPIO_HEATER GPIO_NUM_25 // Heater work
#define GPIO_PUMP_CH GPIO_NUM_27 // Pump Central Heating
#define GIPO_PUMP_DHW GPIO_NUM_14 // Pump Domestic Hot Water

#define GPIO_TEMP_ACTUAL ADC1_CHANNEL_6 // Input ADC actual temperature (GPIO 34)
#define GPIO_TEMP_CH ADC1_CHANNEL_7 // Input ADC Temperature Central Heating (GPIO 4)
#define GPIO_TEMP_DHW ADC1_CHANNEL_4 // Input ADC Temperature Domestic How Water (GPIO 32)

 bool controllerState = false; // Status information controller
 int temp = 20; // Set temperature value
 int tempActual = 20; //Temperature actual room value
 int tempCH = 54; // Temperature actual water Central Heating value 
 int tempDHW = 55; // Temperature actual water Domestic Hot Water value

 bool gpioControllerState = false; // Value status "Heater Work (GPIO 25)"
 bool gpiopumpCHState = false; //Value status "Pump central heating (GPIO 27)"
 bool gpiopumpDHWState = false; // Value status "Pump Domestic How Water (GPIO 14")

 bool alarm_1 = false; // Overheating CH
 bool alarm_2 = false; // Overheating DHW

/********************************************** CONFIGURE GPIO *************************************************/

static void configure_gpio (int PIN)
{
    gpio_reset_pin (PIN);

    gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
}

/********************************************** CONFIGURE GPIO END *********************************************/

/****************************************** TEMPERATURE CONTROL FUNCTION ***************************************/
/* The function control work pumps and room temperature and temperature heating water */

static void controlerOn (bool state)
{
    if(state==true){
        if(tempDHW<60)
        {
            gpio_set_level(GIPO_PUMP_DHW,0); // Turn OFF Pump Domestic Hot Water
            gpiopumpDHWState = false;
            alarm_1 = false;
        }
        else if (tempDHW>60 && tempDHW<75){
            gpio_set_level(GIPO_PUMP_DHW,1); // Turn ON Pump Domestic Hot Water
            gpiopumpDHWState = true;
            alarm_1 = false;
        }else{
            gpio_set_level(GIPO_PUMP_DHW,0); // Turn OFF Pump Domestic Hot Water
            gpiopumpDHWState = false;
            alarm_1 = true; // Alarm Overheating Domestic Hot Water
        }
        if(tempActual<temp){
            if(tempCH<60){
                gpio_set_level(GPIO_PUMP_CH,1); // Turn ON Pump Central Heating
                gpio_set_level(GPIO_HEATER, 1); // Turn ON Heater
                gpioControllerState = true;
                gpiopumpCHState = true;
                alarm_2 = false;

            }else if(tempCH>60 && tempCH<80){
                gpio_set_level(GPIO_PUMP_CH,1); // Turn ON Pump Central Heating
                gpio_set_level(GPIO_HEATER, 0); // Turn OFF Heater
                gpioControllerState = false;
                gpiopumpCHState = true;
                alarm_2 = false;
            }else if(tempCH>80){
                gpio_set_level(GPIO_PUMP_CH,0); // Turn OFF Pump Central Heating
                gpio_set_level(GPIO_HEATER, 0); // Turn OFF Heater
                gpioControllerState = false;
                gpiopumpCHState = false;
                alarm_2 = true;
            }
        }else{
            gpio_set_level(GPIO_HEATER, 0); // Turn OFF Heater
            gpio_set_level(GPIO_PUMP_CH,0); // Turn OFF Pump Central Heating
            gpioControllerState = false;
            gpiopumpCHState = false;
        }
    }else{
        gpio_set_level(GPIO_HEATER, 0); // Turn OFF Heater
        gpio_set_level(GPIO_PUMP_CH,0); // Turn OFF Pump Central Heating
        gpioControllerState = false;
        gpiopumpCHState = false;
        gpio_set_level(GIPO_PUMP_DHW,0); // Turn OFF Pump Domestic Hot Water
        gpiopumpDHWState = false;
    }
}
/*************************************** TEMPERATURE CONTROL FUNCTION END **************************************/


/************************************************* LOGIN TO SYSTEM *********************************************/
/* The function logs to system. User = "admin" Password = "admin" */

static esp_err_t postLogin_handler(httpd_req_t *req)
{
    char nameUser[10] = "";
    char passwordUser[10] = "";
    char content[100];
    size_t recv_size = MIN(req->content_len,sizeof(content));
    int ret = httpd_req_recv(req,content,recv_size);
    char *token = strtok(content,"&");

    while (token != NULL)
    {
        if(strstr(token,"fname")){
            strncpy(nameUser,token+6,10);
        }else if(strstr(token,"lname")){
            strncpy(passwordUser,token+6,10);
        }
        token = strtok(NULL, "&");
    }

    if(ret <= 0){
        if(ret == HTTPD_SOCK_ERR_TIMEOUT){
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }

    if(((strcmp(nameUser,"admin"))==0) && (strcmp(passwordUser,"admin")==0)){
        httpd_resp_send(req,onPanel,HTTPD_RESP_USE_STRLEN);
    }else{
        httpd_resp_send(req,onRegisterError,HTTPD_RESP_USE_STRLEN);
    }

    return ESP_OK;
}

httpd_uri_t registerLogin = {
    .uri       = "/postLogin",
    .method    = HTTP_POST,
    .handler   = postLogin_handler,
};

/******************************************** LOGIN TO SYSTEM END *****************************************/

/****************************************** ACTUAL TEMPERATURE ROOM ***************************************/
/* The function reads actual temperature by analog to digital converter */

static esp_err_t roomTemperature_handler(httpd_req_t *req)
{
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(GPIO_TEMP_ACTUAL , ADC_ATTEN_DB_11);

    tempActual = (((((adc1_get_raw(GPIO_TEMP_ACTUAL) * 5.0)/1024.0)/10)-0.5)*100);
    char temp_actual[]= " ";
    itoa(tempActual,temp_actual,10);

    controlerOn(controllerState);
    httpd_resp_send(req, temp_actual, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static const httpd_uri_t roomTemperature = {
    .uri       = "/roomTemperature",
    .method    = HTTP_GET,
    .handler   = roomTemperature_handler,
};

/******************************************* ACTUAL TEMPERATURE ROOM END ***************************************/

/******************************************* TEMPERATURE CENTRAL HEATING ***************************************/
/* The function reads central heating temperature by analog to digital converter */

static esp_err_t chTemperature_handler(httpd_req_t *req)
{
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(GPIO_TEMP_CH , ADC_ATTEN_DB_11);

    tempCH = (((((adc1_get_raw(GPIO_TEMP_CH) * 5.0)/1024.0)/10)-0.5)*100);
    char temp_actual_ch[]= " ";
    itoa(tempCH,temp_actual_ch,10);

    httpd_resp_send(req, temp_actual_ch, HTTPD_RESP_USE_STRLEN);
    controlerOn(controllerState);
    return ESP_OK;
}

static const httpd_uri_t chTemperature = {
    .uri       = "/CHTemperature",
    .method    = HTTP_GET,
    .handler   = chTemperature_handler,
};

/************************************** TEMPERATURE CENTRAL HEATING END *****************************************/

/*************************************** TEMPERTURE DOMESTIC HOT WATER ******************************************/
/* The function reads Domestic Hot Water temperature by analog to digital converter */

static esp_err_t dhwTemperature_handler(httpd_req_t *req)
{
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(GPIO_TEMP_DHW , ADC_ATTEN_DB_11);

    tempDHW = (((((adc1_get_raw(GPIO_TEMP_DHW) * 5.0)/1024.0)/10)-0.5)*100);
    char temp_actual_dhw[]= " ";
    itoa(tempDHW,temp_actual_dhw,10);

    httpd_resp_send(req, temp_actual_dhw, HTTPD_RESP_USE_STRLEN);
    controlerOn(controllerState);

    return ESP_OK;
}

static const httpd_uri_t dhwTemperature = {
    .uri       = "/DHWTemperature",
    .method    = HTTP_GET,
    .handler   = dhwTemperature_handler,
};

/************************************** TEMPERTURE DOMESTIC HOT WATER END **************************************/

/************************************************* ALARM CONTROL ***********************************************/
/* The function reads Alarm and shows errors */

static esp_err_t alarm_handler(httpd_req_t *req)
{
    if(alarm_1 == false && alarm_2 == false){
        httpd_resp_send(req, "NO ALARM", HTTPD_RESP_USE_STRLEN);
    }else if(alarm_1 == true && alarm_2 == true){
        httpd_resp_send(req, "OVERHEATING", HTTPD_RESP_USE_STRLEN);
    }else if(alarm_1 == true){
        httpd_resp_send(req, "OVERHEATING DHW", HTTPD_RESP_USE_STRLEN);
    }else if(alarm_2 == true){
        httpd_resp_send(req, "OVERHEATING CH", HTTPD_RESP_USE_STRLEN);
    }else{
        httpd_resp_send(req, "ERROR", HTTPD_RESP_USE_STRLEN);
    } 

    return ESP_OK;

}

static const httpd_uri_t valueAlarm = {
    .uri       = "/alarm",
    .method    = HTTP_GET,
    .handler   = alarm_handler,
};

/************************************************ ALARM CONTROL END *********************************************/

/********************************************** EXPECTED TEMPERATURE ********************************************/
/* The function shows expected temperature */

static esp_err_t expectedTemp_handler(httpd_req_t *req)
{
    char temp_actual_exoected[]= " ";
    itoa(temp,temp_actual_exoected,10);

    httpd_resp_send(req, temp_actual_exoected, HTTPD_RESP_USE_STRLEN);
    controlerOn(controllerState);

    return ESP_OK;
}

static const httpd_uri_t expectedTemp = {
    .uri       = "/expectedTemp",
    .method    = HTTP_GET,
    .handler   = expectedTemp_handler,
};

/****************************************** EXPECTED TEMPERATURE END  ********************************************/

/************************************** PUMP DOMESTIC HOW WATER CONTROL ******************************************/
static esp_err_t pumpDhwControl_handler(httpd_req_t *req)
{
    if(gpiopumpDHWState==true){
         httpd_resp_send(req, "WORK", HTTPD_RESP_USE_STRLEN);
    }else{
         httpd_resp_send(req, "NOT WORK", HTTPD_RESP_USE_STRLEN);
    }
    return ESP_OK;
}

static const httpd_uri_t pumpDhwControl = {
    .uri       = "/pumpDHWControl",
    .method    = HTTP_GET,
    .handler   = pumpDhwControl_handler,
};

/**************************************** PUMP DOMESTIC HOW WATER CONTROL ***************************************/

/********************************************** HEATER CONTROL STATUS *******************************************/
static esp_err_t heaterTemperature_handler(httpd_req_t *req)
{ 
    if(gpioControllerState==true){
         httpd_resp_send(req, "WORK", HTTPD_RESP_USE_STRLEN);
    }else{
         httpd_resp_send(req, "NOT WORK", HTTPD_RESP_USE_STRLEN);
    }
    return ESP_OK;
}

static const httpd_uri_t heaterTemperature = {
    .uri       = "/heaterTemperature",
    .method    = HTTP_GET,
    .handler   = heaterTemperature_handler,
};

/****************************************** HEATER CONTROL STATUS END *******************************************/

/***************************************** PUMP CENTRAL HEATING CONTROL *****************************************/
static esp_err_t pumpChControl_handler(httpd_req_t *req)
{
    if(gpiopumpCHState==true){
         httpd_resp_send(req, "WORK", HTTPD_RESP_USE_STRLEN);
    }else{
         httpd_resp_send(req, "NOT WORK", HTTPD_RESP_USE_STRLEN);
    }
    return ESP_OK;
}

static const httpd_uri_t pumpChControl = {
    .uri       = "/pumpChControl",
    .method    = HTTP_GET,
    .handler   = pumpChControl_handler,
};

/******************************************** PUMP CENTRAL HEATING CONTROL **************************************/

/********************************************** ON/OFF CONTROLER STATUS *****************************************/
static esp_err_t controlerStatus_handler(httpd_req_t *req)
{
    if(controllerState==false){
        httpd_resp_send(req, "ON CONTROLER", HTTPD_RESP_USE_STRLEN);
        
    }else{
        httpd_resp_send(req, "OFF CONTROLER", HTTPD_RESP_USE_STRLEN);
    }

    return ESP_OK;
}

static const httpd_uri_t controlerStatus = {
    .uri       = "/controlerStatus",
    .method    = HTTP_GET,
    .handler   = controlerStatus_handler,
};

/******************************************** ON/OFF CONTROLER STATUS END ****************************************/

/********************************************* SIDE EXPECTED TEMPERATURE *****************************************/
static esp_err_t temperature_handler(httpd_req_t *req)
{
    httpd_resp_send(req,onTemperature,HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static const httpd_uri_t temperature = {
    .uri       = "/temperature",
    .method    = HTTP_GET,
    .handler   = temperature_handler,
};

/******************************************** SIDE EXPECTED TEMPERATURE END *************************************/

/********************************************* ACTUAL EXPECTED TEMPERATURE **************************************/
/* Function reads value temperature expencted and shows in side onPanel */

static esp_err_t temperature_action_handler(httpd_req_t *req)
{
    char valueTemperature[10] = "";
    char content_action[100];
    size_t recv_size = MIN(req->content_len,sizeof(content_action));
    int ret3 = httpd_req_recv(req,content_action,recv_size);

    char *token_action = strtok(content_action,"&");
    strncpy(valueTemperature,token_action+8,10);

    printf("%s\n",valueTemperature);

    temp = atoi(valueTemperature);

    httpd_resp_send(req,onPanel,HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

httpd_uri_t temperature_action = {
    .uri       = "/temperature_action",
    .method    = HTTP_POST,
    .handler   = temperature_action_handler,
};

/**************************************** ACTUAL EXPECTED TEMPERATURE END ***************************************/

/************************************************ ON/OFF CONTROLER **********************************************/
/* The function whitch on controler */

static esp_err_t onControler_handler(httpd_req_t *req)
{
    if(controllerState==false){
        httpd_resp_send(req, "OFF CONTROLER", HTTPD_RESP_USE_STRLEN);
        controllerState = true;
        gpio_set_level(GPIO_LED_CONTROLLER, 1);
        
    }else{
        httpd_resp_send(req, "ON CONTROLER", HTTPD_RESP_USE_STRLEN);
        controllerState = false;
        gpio_set_level(GPIO_LED_CONTROLLER, 0);
    }

    return ESP_OK;
}

static const httpd_uri_t onControler = {
    .uri       = "/onControler",
    .method    = HTTP_GET,
    .handler   = onControler_handler,
};

/******************************************** ON/OFF CONTROLER END **********************************************/

/********************************************** FIRST LOGIN PAGE  ***********************************************/
 /* An HTTP GET handler */
static esp_err_t loginHandler(httpd_req_t *req)
{
    esp_err_t error;
    const char *response = (const char *) req->user_ctx;
    error = httpd_resp_send(req, response, strlen(response));
    if(error != ESP_OK)
    {
        ESP_LOGI(TAG, "Error %d while seding Response", error);
    }
    else ESP_LOGI(TAG, "Response sent Successfully");
    return error;
}

static const httpd_uri_t root = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = loginHandler,
    .user_ctx  = onRegister 
};

/******************************************* FIRST LOGIN PAGE END ***********************************************/

esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &root);
        httpd_register_uri_handler(server, &roomTemperature);
        httpd_register_uri_handler(server, &registerLogin);
        httpd_register_uri_handler(server, &onControler);
        httpd_register_uri_handler(server, &chTemperature);
        httpd_register_uri_handler(server, &dhwTemperature);
        httpd_register_uri_handler(server, &heaterTemperature);
        httpd_register_uri_handler(server, &pumpDhwControl);
        httpd_register_uri_handler(server, &pumpChControl);
        httpd_register_uri_handler(server, &controlerStatus);
        httpd_register_uri_handler(server, &temperature);
        httpd_register_uri_handler(server, &temperature_action);
        httpd_register_uri_handler(server, &expectedTemp);
        httpd_register_uri_handler(server, &valueAlarm);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static void stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_stop(server);
}

static void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

void wifi_init_softap(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}

void app_main(void)
{
    static httpd_handle_t server = NULL;

    configure_gpio (GPIO_LED_CONTROLLER);
    configure_gpio(GPIO_HEATER);
    configure_gpio(GPIO_PUMP_CH);
    configure_gpio(GIPO_PUMP_DHW);

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();
    
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED, &connect_handler, &server));
   // ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
    

    }
