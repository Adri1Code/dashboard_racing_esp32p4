#include "ui.h"
#include "ui_manager.h"
#include "ui_manager_slider.h"
#include "nvs.h"
#include "slider.h"
#include "bsp/esp-bsp.h"

static const char *TAG = "UI_MANAGER";

void ui_manager(void)
{
    ESP_LOGI(TAG, "Initialisation de l'interface utilisateur...");

    ui_init();

    ui_slider();
}








