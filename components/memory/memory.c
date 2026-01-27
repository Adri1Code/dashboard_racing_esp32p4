#include "memory.h"
#include "nvs_flash.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_spiffs.h"
#include "esp_image_format.h"
#include <string.h>

#define LOG_BREAK printf("\n")

static const char *TAG = "MEMORY";

// Afficher stats de la RAM
void ram_static_analysis(void){
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
void psram_static_analysis(void)
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


void flash_static_analysis(void) {
    LOG_BREAK;
    ESP_LOGI(TAG, "============ ANALYSE PRECISE DE LA FLASH ============");

    esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);

    while (it != NULL) {
        const esp_partition_t *p = esp_partition_get(it);
        float usage_pct = 0.0f;
        uint32_t used_size = 0;
        const char* type_str = "Data";

        // Cas 1 : Partition Application (factory)
        if (p->type == ESP_PARTITION_TYPE_APP) {
            type_str = "Application";
            esp_image_metadata_t data;
            const esp_partition_pos_t pos = {
                .offset = p->address,
                .size = p->size,
            };
            
            // Vérifie l'image et récupère la taille réelle du binaire
            if (esp_image_verify(ESP_IMAGE_VERIFY, &pos, &data) == ESP_OK) {
                used_size = data.image_len;
                usage_pct = ((float)used_size / p->size) * 100.0f;
            }
        } 
        // Cas 2 : Partition NVS
        else if (strcmp(p->label, "nvs") == 0) {
            type_str = "NVS Storage";
            nvs_stats_t nvs_stats;
            if (nvs_get_stats(p->label, &nvs_stats) == ESP_OK) {
                used_size = nvs_stats.used_entries * 32; // 1 entrée = 32 octets
                usage_pct = ((float)nvs_stats.used_entries / nvs_stats.total_entries) * 100.0f;
            }
        }
        // Cas 3 : Partition PHY_INIT (Calibration)
        else if (strcmp(p->label, "phy_init") == 0) {
            type_str = "RF Calib";
            usage_pct = 100.0f; // Toujours considérée pleine
        }

        ESP_LOGI(TAG, "| %-10s | %-12s | %4lu / %4lu Ko | %6.2f%%", 
                 p->label, 
                 type_str,
                 (used_size > 0 ? used_size / 1024 : 0), 
                 p->size / 1024, 
                 usage_pct);

        it = esp_partition_next(it);
    }
    esp_partition_iterator_release(it);
}