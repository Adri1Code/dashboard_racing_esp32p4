#pragma once

#ifndef MEMORY_H
#define MEMORY_H

#include "esp_system.h"
#include "esp_heap_caps.h"
#include "esp_partition.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "spi_flash_mmap.h"

void ram_static_analysis(void);
void psram_static_analysis(void);
void flash_static_analysis(void);


#endif