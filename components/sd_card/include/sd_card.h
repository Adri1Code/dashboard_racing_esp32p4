#pragma once
#ifndef SD_CARD_H
#define SD_CARD_H

#include "dirent.h"
#include "esp_log.h"
#include "bsp/esp-bsp.h"    
#include "bsp/display.h"

DIR *sd_card_mount();
void sd_card_scan(DIR **sd_directory);
void sd_card_load_jpg_on_screen(lv_obj_t *screen, const char *jpg_filename);



#endif  










