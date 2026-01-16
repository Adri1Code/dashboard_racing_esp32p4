#pragma once

#ifndef SCREEN_H
#define SCREEN_H

#include "lvgl.h"

void screen_long_press_event_cb();
void screen_print_text(lv_obj_t *screen);
void first_screen_cfg();
void second_screen_cfg();


#endif