#include "display_init.h"
#include "bsp/display.h"
#include "bsp/esp-bsp.h"    
#include "esp_log.h"

static const char *TAG = "DISPLAY";

// Configuration de l'affichage et initialisation du materiel
void waveshare_display_init()
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

    lv_display_t *disp = bsp_display_start_with_config( &cfg );
    bsp_display_backlight_on();
    bsp_display_brightness_set( 100 );                  

    if (disp != NULL) {
        bsp_display_rotate(disp, LV_DISPLAY_ROTATION_180);
    }
    
    printf("\n");
    ESP_LOGI(TAG, "Affichage configuré");

#if LV_USE_FS_POSIX
    lv_fs_posix_init(); 
    ESP_LOGI(TAG, "LVGL FS POSIX initialisé");
#elif LV_USE_FS_FATFS
    lv_fs_fatfs_init();
    ESP_LOGI(TAG, "LVGL FS FATFS initialisé");
#endif

}