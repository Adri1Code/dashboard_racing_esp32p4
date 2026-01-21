#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"

// Charger une valeur en NVS
void save_brightness_to_nvs(uint32_t value)
{
    nvs_handle_t my_handle;
    if (nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle) == ESP_OK)
    {
        nvs_set_u32(my_handle, "brightness", value);
        nvs_commit(my_handle);  // valider l'ecriture
        nvs_close(my_handle);
    }
}

// Lire une valeur en NVS
uint32_t load_brightness_from_nvs()
{
    nvs_handle_t my_handle;
    uint32_t value = 100;
    if (nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &my_handle) == ESP_OK)
    {
        nvs_get_u32(my_handle, "brightness", &value);
        nvs_close(my_handle);
    }
    return value;
}



