#include "ui.h"
#include "ui_manager_slider.h"
#include "slider.h"
#include "bsp/esp-bsp.h"
#include "nvs.h"

static const char *TAG = "UI_MANAGER_SLIDER";

void ui_slider(void)
{
    if (ui_sliderbrightness != NULL)
    {
        lv_obj_add_event_cb(ui_sliderbrightness, brightness_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
        uint32_t saved_value = load_brightness_from_nvs();
        lv_slider_set_value(ui_sliderbrightness, saved_value, LV_ANIM_OFF);
        bsp_display_brightness_set(saved_value);
    
        ESP_LOGI(TAG, "Slider luminosité lié et synchronisé.");
    }
}





