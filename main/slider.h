#pragma once
#ifndef SLIDER_H
#define SLIDER_H

#include "esp_log.h"
#include "bsp/esp-bsp.h"    

void brightness_slider_event_cb(lv_event_t* event);
void slider_brightness_cfg(lv_obj_t *screen1);

#endif