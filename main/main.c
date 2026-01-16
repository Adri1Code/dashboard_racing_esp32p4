#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "bsp/esp-bsp.h"    
#include "bsp/display.h"
#include "lvgl.h"

#include "sd_card.h"
#include "slider.h"
#include "screen.h"

static const char *TAG = "APP_MAIN";

//static lv_obj_t *screen1 = NULL;
//static lv_obj_t *screen2 = NULL;

// Configuration de l'affichage et initialisation du materiel
static void waveshare_display_init()
{
    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),   // parametres de base pour LVGL
        .buffer_size = BSP_LCD_DRAW_BUFF_SIZE,          // quantite de memoire allouee pour rendu graphique
        .double_buffer = BSP_LCD_DRAW_BUFF_DOUBLE,      // rendu plus fluide
        .flags = {
            .buff_dma = true,                           // transferts memoire sans CPU
            .buff_spiram = false,                       // utilise RAM interne plutot que SPIRAM
            .sw_rotate = true,                          // pivot de l'affichage
        }
    };

    lv_display_t *disp = bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();
    bsp_display_brightness_set(100);

    if (disp != NULL) {
        bsp_display_rotate(disp, LV_DISPLAY_ROTATION_180);
    } 
}

void app_main(void)
{ 
    ESP_LOGI(TAG, "Debut du programme");

    waveshare_display_init();

    DIR* dir = sd_card_mount();

    sd_card_scan(&dir);

    bsp_display_lock(0);                                 // verrouillage de LVGL pour manipuler les objets

    first_screen_cfg();

    second_screen_cfg();

    bsp_display_unlock();                                // deverouillage de LVGL

    ESP_LOGI(TAG, "Fin du programme");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}