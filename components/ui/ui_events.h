#ifndef _UI_EVENTS_H
#define _UI_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void set_target_led(lv_event_t * e);
void set_target_colour(lv_event_t * e);
void apply_color_green(lv_event_t * e);
void apply_color_orange(lv_event_t * e);
void apply_color_red(lv_event_t * e);
void apply_color_cyan(lv_event_t * e);
void apply_color_blue(lv_event_t * e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif