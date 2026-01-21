#include "slider.h"
#include "esp_log.h"  
#include "bsp/display.h"
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"

static const char *TAG = "SLIDER";

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

// Modifie la luminosite selon le changement sur le slider
void brightness_slider_event_cb(lv_event_t *event)
{
    lv_obj_t *slider = lv_event_get_target( event );
    uint32_t value = lv_slider_get_value( slider );

    bsp_display_brightness_set( value );
    save_brightness_to_nvs( value );

    ESP_LOGI(TAG, "Luminositee ajustee a : %ld%%", value);
}

// Creation d'un slider avec reglage de la luminosite
void slider_brightness_cfg(lv_obj_t *screen)
{
    uint32_t saved_value = load_brightness_from_nvs();

    bsp_display_brightness_set( saved_value );

    lv_obj_t *slider = lv_slider_create( screen );       // creer un slider
    lv_slider_set_range(slider, 0, 100);                 // range du slider
    lv_slider_set_value(slider, saved_value, LV_ANIM_OFF);

    lv_obj_set_width(slider, lv_pct(80));                // 80% de la largeur de l'ecran
    lv_obj_align(slider, LV_ALIGN_TOP_MID, 0, 20);       // aligne en haut au centre

    // Evenement : utilisateur change le slider 
    lv_obj_add_event_cb(slider, brightness_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *slider_label = lv_label_create( screen );      
    lv_label_set_text(slider_label, "Brightness");
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
}


