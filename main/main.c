#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "esp_partition.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "spi_flash_mmap.h"
#include "bsp/esp-bsp.h"    
#include "bsp/display.h"
#include "lvgl.h"
#include "display_init.h"
#include "sd_card.h"
#include "slider.h"
#include "screen.h"

#define LOG_BREAK printf("\n")

static const char *TAG = "APP_MAIN";

void ram_static_analysis(){
    LOG_BREAK;
    ESP_LOGI(TAG, "============ ANALYSE RAM ============");
    size_t free_ram_internal = heap_caps_get_free_size( MALLOC_CAP_INTERNAL );
    size_t total_ram_internal = heap_caps_get_total_size( MALLOC_CAP_INTERNAL );
    ESP_LOGI(TAG, "RAM interne : Libre: %d Ko / Totale: %d Ko", free_ram_internal / 1024 , total_ram_internal / 1024); 
}

void psram_static_analysis()
{
    LOG_BREAK;
    ESP_LOGI(TAG, "============ ANALYSE PSRAM ============");
    size_t free_psram_internal = heap_caps_get_free_size( MALLOC_CAP_SPIRAM );
    size_t total_psram_internal = heap_caps_get_total_size( MALLOC_CAP_SPIRAM );
    if(total_psram_internal > 0)
    {
        ESP_LOGI(TAG, "PSRAM (EXTERNE) : Libre: %d Ko / Totale: %d Ko", free_psram_internal / 1024, total_psram_internal / 1024);
    }else{
        ESP_LOGW(TAG, "PSRAM non detectee"); 
    }
}

void flash_static_analysis()
{
    LOG_BREAK;
    ESP_LOGI(TAG, "============ ANALYSE FLASH ============");
    uint32_t flash_size;
    esp_flash_get_size(NULL, &flash_size);
    ESP_LOGI(TAG, "Taille de la FLASH : %lu Mo", flash_size / (1024 * 1024));

    ESP_LOGI(TAG, "Detail du fichier partitions.csv :");
    esp_partition_iterator_t partition_iterator = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);

    while(partition_iterator != NULL)
    {
        const esp_partition_t *p = esp_partition_get(partition_iterator);
        ESP_LOGI(TAG, "- Partition '%s' | Type: 0x%x | Taille: %lu Ko", p->label, p->type, p->size / 1024);
        partition_iterator = esp_partition_next(partition_iterator);
    }

    esp_partition_iterator_release(partition_iterator);
}


void app_main(void)
{ 
    LOG_BREAK;
    ESP_LOGI(TAG, "Debut du programme");

    waveshare_display_init();

    ram_static_analysis();
    psram_static_analysis();
    flash_static_analysis();

    DIR *dir = sd_card_mount();

    sd_card_scan(&dir);

    bsp_display_lock(0);                                 // verrouillage de LVGL pour manipuler les objets

    first_screen_cfg();

    second_screen_cfg();

    third_screen_cfg();

    bsp_display_unlock();                                // deverouillage de LVGL

    LOG_BREAK;
    ESP_LOGI(TAG, "Fin du programme");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}