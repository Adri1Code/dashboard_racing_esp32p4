#include "memory.h"

#define LOG_BREAK printf("\n")

static const char *TAG = "MEMORY";

// Afficher stats de la RAM
void ram_static_analysis(){
    LOG_BREAK;
    ESP_LOGI(TAG, "============ ANALYSE RAM ============");
    size_t free_ram_internal = heap_caps_get_free_size( MALLOC_CAP_INTERNAL );
    size_t total_ram_internal = heap_caps_get_total_size( MALLOC_CAP_INTERNAL );
    float ram_occupancy_ratio = (1.0f - ((float)free_ram_internal / total_ram_internal)) * 100.0f;
    ESP_LOGI(TAG, "RAM (INTERNE) : Libre: %d Ko", free_ram_internal / 1024); 
    ESP_LOGI(TAG, "RAM (INTERNE) : Totale: %d Ko", total_ram_internal / 1024);
    ESP_LOGI(TAG, "RAM (INTERNE) : Pleine à %.2f%%", ram_occupancy_ratio); 
}

// Afficher stats de la PSRAM
void psram_static_analysis()
{
    LOG_BREAK;
    ESP_LOGI(TAG, "============ ANALYSE PSRAM ============");
    size_t free_psram_internal = heap_caps_get_free_size( MALLOC_CAP_SPIRAM );
    size_t total_psram_internal = heap_caps_get_total_size( MALLOC_CAP_SPIRAM );
    float psram_occupancy_ratio = (1.0f - ((float)free_psram_internal / total_psram_internal)) * 100.0f;
    if(total_psram_internal > 0)
    {
        ESP_LOGI(TAG, "PSRAM (EXTERNE) : Libre: %d Ko", free_psram_internal / 1024);
        ESP_LOGI(TAG, "PSRAM (EXTERNE) : Totale: %d Ko", total_psram_internal / 1024); 
        ESP_LOGI(TAG, "PSRAM (EXTERNE) : Pleine à %.2f%%", psram_occupancy_ratio);  
    } else{ ESP_LOGW(TAG, "PSRAM non detectee"); }
}

// Afficher stats de la FLASH
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