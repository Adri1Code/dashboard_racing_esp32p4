#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "ui_helpers.h"
#include "ui_events.h"

// --- DECLARATION DES IMAGES (Ajoute cette section) ---
LV_IMG_DECLARE(ui_img_screen_config_png);
LV_IMG_DECLARE(ui_img_led_noire_png);
LV_IMG_DECLARE(ui_img_led_verte_png);
LV_IMG_DECLARE(ui_img_led_orange_png);
LV_IMG_DECLARE(ui_img_led_rouge_png);
LV_IMG_DECLARE(ui_img_led_cyan_png);
LV_IMG_DECLARE(ui_img_led_bleue_png);

// --- SCREENS ---
#include "screens/ui_screenconfig.h"

// --- VARIABLES ---
extern lv_obj_t *ui____initial_actions0;

// --- PROTOTYPES DES FONCTIONS D'ÉVÉNEMENTS ---
// Cela règle les erreurs "implicit declaration"
void set_target_led(lv_event_t * e);
void set_target_colour(lv_event_t * e);
void apply_color_green(lv_event_t * e);
void apply_color_orange(lv_event_t * e);
void apply_color_red(lv_event_t * e);
void apply_color_cyan(lv_event_t * e);
void apply_color_blue(lv_event_t * e);

void ui_init(void);
void ui_destroy(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif