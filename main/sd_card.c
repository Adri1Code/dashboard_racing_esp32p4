#include "sd_card.h"

#define MOUNT_POINT "/sdcard"

static const char *TAG = "APP_MAIN";

// Monter la carte SD
DIR* sd_card_mount()
{   // montage sd card 
    ESP_LOGI(TAG, "Montage de la carte SD...");
    esp_err_t ret = bsp_sdcard_mount();

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Erreur montage SD (%s)", esp_err_to_name(ret));
        return NULL;
    } 
     
    DIR *dir = opendir(MOUNT_POINT);
    if (dir == NULL)
    {
        ESP_LOGW(TAG, "Echec a %s, tentative sur /sd"); 
        dir = opendir("/sd");
    }

    return dir;
}


// Lire le contenu de la carte SD
void sd_card_scan(DIR *dir)
{
    if (dir != NULL)
    {
        struct dirent *entry;
        ESP_LOGI(TAG, "--- Contenu de la carte SD ---");
        ESP_LOGI(TAG, "Chemin: %s", MOUNT_POINT);

        while ( (entry = readdir(dir)) != NULL )
        {
            if ( entry->d_name[0] == '.' || strcmp(entry->d_name, "System Volume Information") == 0 ){ continue; }
            ESP_LOGI(TAG, "Trouvé: %s", entry->d_name);
        } 

        ESP_LOGI(TAG, "------------------------");
        closedir(dir);
    } else{ ESP_LOGE(TAG, "Erreur : Impossible d'accéder au système de fichiers."); }   
}







