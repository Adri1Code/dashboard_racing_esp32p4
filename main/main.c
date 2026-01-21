#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bsp/esp-bsp.h"    
#include "bsp/display.h"
#include "lvgl.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "display_init.h"
#include "sd_card.h"
#include "slider.h"
#include "screen.h"
#include "memory.h"
#include "ui.h"
#include "ui_manager.h"

#define LOG_BREAK printf("\n")

static const char *TAG = "APP_MAIN";

void app_main(void)
{ 
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    LOG_BREAK;
    ESP_LOGI(TAG, "Debut du programme");

    waveshare_display_init();

    ram_static_analysis();
    psram_static_analysis();
    flash_static_analysis();

    DIR *dir = sd_card_mount();

    sd_card_scan(&dir);

    bsp_display_lock(0);                                 // verrouillage de LVGL pour manipuler les objets

    ui_manager();

    bsp_display_unlock();                                // deverouillage de LVGL

    LOG_BREAK;
    ESP_LOGI(TAG, "Fin du programme");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}