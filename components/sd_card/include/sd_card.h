#pragma once
#ifndef SD_CARD_H
#define SD_CARD_H

#include "dirent.h"
#include "esp_log.h"
#include "bsp/esp-bsp.h"    
#include "bsp/display.h"

DIR *sd_card_mount();
void sd_card_scan(DIR **sd_directory);


#endif  










