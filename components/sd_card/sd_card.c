#include "sd_card.h"

#define MOUNT_POINT "/sdcard"
#define LOG_BREAK printf("\n")

static const char *TAG = "SD_CARD";

// Monter la carte SD
DIR *sd_card_mount()
{  
    LOG_BREAK; 
    ESP_LOGI(TAG, "Montage de la carte SD...");
    esp_err_t ret = bsp_sdcard_mount();

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Erreur montage SD (%s)", esp_err_to_name(ret));
        return NULL;
    } 
     
    DIR *sd_directory = opendir( MOUNT_POINT );
    if (sd_directory == NULL)
    {
        ESP_LOGW(TAG, "Echec a %s, tentative sur /sd"); 
        sd_directory = opendir( "/sd" );
    }

    return sd_directory;
}


// Lire le contenu de la carte SD
void sd_card_scan(DIR **sd_directory)
{
    if (sd_directory != NULL && *sd_directory != NULL)
    {
        struct dirent *entry;
        ESP_LOGI(TAG, "--- Contenu de la carte SD ---");
        ESP_LOGI(TAG, "Chemin: %s", MOUNT_POINT);

        while ( (entry = readdir(*sd_directory)) != NULL )
        {
            if ( entry->d_name[0] == '.' || strcmp(entry->d_name, "System Volume Information") == 0 ){ continue; }
            ESP_LOGI(TAG, "Trouvé: %s", entry->d_name);
        } 

        ESP_LOGI(TAG, "------------------------");
        closedir( *sd_directory );                        // libere les ressources internes
        *sd_directory = NULL;                           // securise le pointeur pour le reste du programme
    } else{ ESP_LOGE(TAG, "Erreur : Impossible d'accéder au système de fichiers."); }   
}


// Afficher une image stockee dans la carte SD sur un écran
void sd_card_load_jpg_on_screen(lv_obj_t *screen, const char *jpg_filename)
{
    LOG_BREAK;
    ESP_LOGI(TAG, "Chargement d'une image depuis la carte SD...");

    char lvgl_jpg_path[128];

    // On ajoute le préfixe "A:" pour que LVGL utilise son driver POSIX
    snprintf(lvgl_jpg_path, sizeof(lvgl_jpg_path), "A:%s/%s", MOUNT_POINT, jpg_filename);

    ESP_LOGI(TAG, "Chemin de l'image pour LVGL : %s", lvgl_jpg_path);

    lv_obj_t *image = lv_img_create( screen );
    if (image == NULL) return;

    // LVGL va maintenant intercepter "A:", voir qu'il s'agit du driver POSIX,
    // et ouvrir le fichier en interne.
    lv_img_set_src(image, lvgl_jpg_path);
    lv_obj_center( image );

    ESP_LOGI(TAG, "L'image est affichee");
}






