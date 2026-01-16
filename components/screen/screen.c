#include "screen.h"
#include "slider.h"
#include "esp_log.h"
#include "bsp/esp-bsp.h"    
#include "bsp/display.h"
#include "lvgl.h" 

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

static const char *TAG = "SCREEN";

static lv_obj_t *screen1 = NULL;
static lv_obj_t *screen2 = NULL;

// Changement d'ecran avec pression du doigt
void screen_long_press_event_cb()
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
void first_screen_cfg()
{
    screen1 = lv_obj_create( NULL );                     
    lv_scr_load(screen1);

    lv_obj_t *label = lv_label_create(screen1);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_44, 0);
    lv_label_set_text(label, "Screen 1");
    lv_obj_center(label);                                // centrer le texte

    // Changer de screen
    lv_obj_add_event_cb(screen1, screen_long_press_event_cb, LV_EVENT_LONG_PRESSED, NULL);
}

// Creation et configuration du second screen
void second_screen_cfg()
{
    screen2 = lv_obj_create( NULL ); 

    lv_obj_t *label2 = lv_label_create(screen2);
    lv_obj_set_style_text_font(label2, &lv_font_montserrat_44, 0);
    lv_label_set_text(label2, "Screen 2");
    lv_obj_center(label2);

    // Changer de screen
    lv_obj_add_event_cb(screen2, screen_long_press_event_cb, LV_EVENT_LONG_PRESSED, NULL);
}


