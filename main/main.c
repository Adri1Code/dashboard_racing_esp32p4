#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bsp/esp-bsp.h"    
#include "bsp/display.h"
#include "lvgl.h"
#include "display_init.h"
#include "sd_card.h"
#include "slider.h"
#include "screen.h"
#include "memory.h"

#define LOG_BREAK printf("\n")

static const char *TAG = "APP_MAIN";

void app_main(void)
{ 
    LOG_BREAK;
    ESP_LOGI(TAG, "Debut du programme");

    waveshare_display_init();

    ram_static_analysis();
    psram_static_analysis();
    flash_static_analysis();

    DIR *dir = sd_card_mount();

    sd_card_scan(&dir);

    bsp_display_lock(0);                                 // verrouillage de LVGL pour manipuler les objets

    first_screen_cfg();

    second_screen_cfg();

    third_screen_cfg();

    bsp_display_unlock();                                // deverouillage de LVGL

    LOG_BREAK;
    ESP_LOGI(TAG, "Fin du programme");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}