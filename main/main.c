#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "bsp/esp-bsp.h"    
#include "bsp/display.h"
#include "lvgl.h"

#include "sd_card.h"
#include "slider.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

static const char *TAG = "APP_MAIN";

static lv_obj_t *screen1 = NULL;
static lv_obj_t *screen2 = NULL;

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


// Changement d'ecran avec pression du doigt
static void screen_long_press_event_cb()
{
    lv_point_t point;
    lv_indev_get_point(lv_indev_get_act(), &point);

    if (point.x > SCREEN_WIDTH / 2)
    {
        ESP_LOGI(TAG, "Appui long detecte sur la droite - Ecran 1->2");
        lv_scr_load_anim(screen2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
    }
    else
    {
        ESP_LOGI(TAG, "Appui long detecte sur la gauche - Ecran 2->1");
        lv_scr_load_anim(screen1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
    }
}

// Creation et configuration du premier screen
static void first_screen_cfg()
{
    screen1 = lv_obj_create( NULL );                     
    lv_scr_load(screen1);

    slider_brightness_cfg(screen1);

    lv_obj_t *label = lv_label_create(screen1);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_44, 0);
    lv_label_set_text(label, "Screen 1");
    lv_obj_center(label);                                // centrer le texte

    // Changer de screen
    lv_obj_add_event_cb(screen1, screen_long_press_event_cb, LV_EVENT_LONG_PRESSED, NULL);
}

// Creation et configuration du second screen
static void second_screen_cfg()
{
    screen2 = lv_obj_create( NULL ); 

    lv_obj_t *label2 = lv_label_create(screen2);
    lv_obj_set_style_text_font(label2, &lv_font_montserrat_44, 0);
    lv_label_set_text(label2, "Screen 2");
    lv_obj_center(label2);

    // Changer de screen
    lv_obj_add_event_cb(screen2, screen_long_press_event_cb, LV_EVENT_LONG_PRESSED, NULL);
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