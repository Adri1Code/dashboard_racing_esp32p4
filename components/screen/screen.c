#include "screen.h"
#include "slider.h"
#include "sd_card.h"
#include "esp_log.h"
#include "bsp/esp-bsp.h"    
#include "bsp/display.h"
#include "lvgl.h" 

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

static const char *TAG = "SCREEN";

static lv_obj_t *screen1 = NULL;
static lv_obj_t *screen2 = NULL;
static uint32_t timer_call_counter = 0;

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

// Gestion d'un timer pour passer du premier ecran au deuxieme ecran
void timer_to_change_screen(lv_timer_t *timer)
{
    timer_call_counter++; 

    ESP_LOGI(TAG, "Nombre de declenchements du timer: %d", timer_call_counter); 
    ESP_LOGI(TAG, "Temps ecoule: %d * 2000ms = %dms", timer_call_counter, timer_call_counter * 2000);

    // si le timer est declenche au moins une fois on change d'ecran
    if (timer_call_counter >= 1)
    {
        ESP_LOGI(TAG, "Passage a l'ecran suivant");
        lv_scr_load_anim(screen2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0, false);

        lv_timer_del(timer);
        timer_call_counter = 0;
    }
}


// Afficher du texte sur un écran
void screen_print_text(lv_obj_t *screen)
{
    lv_obj_t *label = lv_label_create(screen);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_44, 0);
    lv_label_set_text(label, "Screen 1");
    lv_obj_center(label);                             
}


// Creation et configuration du premier screen
void first_screen_cfg()
{
    screen1 = lv_obj_create( NULL );                     
    lv_scr_load(screen1);

    sd_card_load_jpg_on_screen(screen1, "logo.jpg");

    // Creer un timer qui se declenche tous les 2 secondes
    lv_timer_create(timer_to_change_screen, 2000, &timer_call_counter);            

    // Changer de screen
    //lv_obj_add_event_cb(screen1, screen_long_press_event_cb, LV_EVENT_LONG_PRESSED, NULL);
}

// Creation et configuration du second screen
void second_screen_cfg()
{
    screen2 = lv_obj_create( NULL ); 

    sd_card_load_jpg_on_screen(screen2, "config_led_std.jpg");

    //lv_obj_t *label2 = lv_label_create(screen2);
    //lv_obj_set_style_text_font(label2, &lv_font_montserrat_44, 0);
    //lv_label_set_text(label2, "Screen 2");
    //lv_obj_center(label2);

    // Changer de screen
    //lv_obj_add_event_cb(screen2, screen_long_press_event_cb, LV_EVENT_LONG_PRESSED, NULL);
}


