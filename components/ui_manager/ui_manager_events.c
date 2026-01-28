#include "ui_manager.h"
#include "nvs_flash.h"
#include "ui_ScreenLeds.h"

extern const lv_img_dsc_t ui_img_bouton_vert_png;
extern const lv_img_dsc_t ui_img_bouton_orange_png;
extern const lv_img_dsc_t ui_img_bouton_rouge_png;
extern const lv_img_dsc_t ui_img_bouton_cyan_png;
extern const lv_img_dsc_t ui_img_bouton_bleu_png;
extern const lv_img_dsc_t ui_img_bouton_noir_png;

// Helper : Convertir une image en code couleur (pour la sauvegarde NVS)
static led_color_t get_color_code_from_img(const void * img_src) {
    if (img_src == &ui_img_bouton_vert_png) return COLOR_GREEN;
    if (img_src == &ui_img_bouton_orange_png) return COLOR_ORANGE;
    if (img_src == &ui_img_bouton_rouge_png) return COLOR_RED;
    if (img_src == &ui_img_bouton_cyan_png) return COLOR_CYAN;
    if (img_src == &ui_img_bouton_bleu_png) return COLOR_BLUE;
    return COLOR_NONE; // Par défaut ou si noir
}

// Helper : Convertir un code couleur en image (pour le chargement NVS)
static const void* get_img_from_color_code(uint32_t code) {
    switch (code) {
        case COLOR_GREEN: return &ui_img_bouton_vert_png;
        case COLOR_ORANGE: return &ui_img_bouton_orange_png;
        case COLOR_RED: return &ui_img_bouton_rouge_png;
        case COLOR_CYAN: return &ui_img_bouton_cyan_png;
        case COLOR_BLUE: return &ui_img_bouton_bleu_png;
        default: return &ui_img_bouton_noir_png;
    }
}

static lv_obj_t ** all_leds_array[] = {
    &ui_PanelShift1, // LED 1
    &ui_PanelShift2,
    &ui_PanelShift3,
    &ui_PanelShift4,
    &ui_PanelShift5,
    &ui_PanelShift6,
    &ui_PanelShift7,
    &ui_PanelShift8,
    &ui_PanelShiftMaxi1,
    &ui_PanelShiftMaxi2,
    &ui_PanelShiftMaxi3,
    &ui_PanelShiftMaxi4,
    &ui_PanelShiftMaxi5,
    &ui_PanelPOil,
    &ui_PanelTempWater,
    // ... ajoute les autres LEDs ici (Limiteur, RPM, etc.)
    NULL // Toujours finir par NULL pour la sécurité
};

// Variable globale pour stocker la LED sélectionnée
lv_obj_t * current_selected_led = NULL;

static char console_text[32] = "";      // Buffer pour le texte affiché 
//lv_obj_t *ui_console_label = NULL;      // Label pour afficher le texte
extern lv_obj_t *ui_console_label;


void save_global_config_to_nvs(void)
{
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    
    if (err != ESP_OK) {    
        printf("Erreur ouverture NVS (%s)\n", esp_err_to_name(err));
        return;
    }

    int i = 0;
    // On boucle tant qu'il y a des LEDs dans le tableau
    while (all_leds_array[i] != NULL) {
        lv_obj_t * led = *all_leds_array[i]; // Récupérer l'objet
        
        // 1. Récupérer l'image actuelle
        const void * img_src = lv_obj_get_style_bg_img_src(led, LV_PART_MAIN);
        
        // 2. Convertir en code (0, 1, 2...) grâce à notre helper précédent
        uint32_t color_code = get_color_code_from_img(img_src);
        
        // 3. Créer une clé unique basée sur l'index (led_0, led_1...)
        char key[16];
        sprintf(key, "led_%d", i);
        
        // 4. Écrire sans "commit" à chaque fois (plus rapide)
        nvs_set_u32(my_handle, key, color_code);
        
        i++;
    }

    // 5. Valider et fermer une seule fois à la fin
    nvs_commit(my_handle);
    nvs_close(my_handle);
    
    printf("Sauvegarde globale terminee : %d LEDs sauvegardees.\n", i);
}

void load_global_config_from_nvs(void)
{
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &my_handle);
    
    if (err != ESP_OK) return; // Pas de sauvegarde trouvée ou erreur

    int i = 0;
    while (all_leds_array[i] != NULL) {
        lv_obj_t * led = *all_leds_array[i];
        
        char key[16];
        sprintf(key, "led_%d", i);
        
        uint32_t color_code = 0;
        // Si la lecture échoue (ex: nouvelle LED ajoutée), on garde 0 par défaut
        if (nvs_get_u32(my_handle, key, &color_code) == ESP_OK) {
            
            if (color_code != 0) { // Si ce n'est pas noir
                const void * img_src = get_img_from_color_code(color_code);
                
                // Appliquer le style visuel
                lv_obj_set_style_bg_img_src(led, img_src, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_opa(led, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_opa(led, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
        i++;
    }
    
    nvs_close(my_handle);
    printf("Chargement global termine.\n");
}

// Fonction pour mémoriser quelle LED noire a été cliquée
void set_target_led(lv_event_t * e) 
{
    //current_selected_led = lv_event_get_target(e);
    
    // Optionnel : un petit effet visuel pour confirmer la sélection
    // Ici on réduit un peu l'opacité pour montrer qu'elle est "active"
    //lv_obj_set_style_opa(current_selected_led, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * target = lv_event_get_target(e);

    // Si une autre LED avait déjà le halo, on le retire
    if (current_selected_led != NULL) {
        lv_obj_clear_state(current_selected_led, LV_STATE_CHECKED);
    }

    current_selected_led = target;
    
    // On ajoute l'état CHECKED pour faire apparaître le halo cyan hardcodé
    lv_obj_add_state(current_selected_led, LV_STATE_CHECKED);

}

// Fonction générique (interne) pour appliquer une couleur
void _apply_color(const lv_img_dsc_t * img_src) 
{
    if (current_selected_led != NULL) {
        // On remet l'opacité à fond
        lv_obj_set_style_opa(current_selected_led, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        // On change l'image de fond
        lv_obj_set_style_bg_img_src(current_selected_led, img_src, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_clear_state(current_selected_led, LV_STATE_CHECKED);
        current_selected_led = NULL;  
        
        check_and_auto_save();
    }
}

// Vos fonctions appelées par les événements SquareLine
void apply_color_green(lv_event_t * e)  { _apply_color(&ui_img_bouton_vert_png); }
void apply_color_orange(lv_event_t * e) { _apply_color(&ui_img_bouton_orange_png); }
void apply_color_red(lv_event_t * e)    { _apply_color(&ui_img_bouton_rouge_png); }
void apply_color_cyan(lv_event_t * e)   { _apply_color(&ui_img_bouton_cyan_png); }
void apply_color_blue(lv_event_t * e)   { _apply_color(&ui_img_bouton_bleu_png); }

// Correction pour les panels 7 et 8 (vous aviez mis set_target_colour au lieu de led)
void set_target_colour(lv_event_t * e) { set_target_led(e); }

void reset_led_to_black(lv_event_t * e)
{
    lv_obj_t * led = lv_event_get_target(e);
    
    // Vérifier si la LED a une couleur (pas l'image noire)
    const void * current_img = lv_obj_get_style_bg_img_src(led, LV_PART_MAIN);
    
    if (current_img != &ui_img_bouton_noir_png) {
        // La LED est colorée, on la remet en noir
        lv_obj_set_style_bg_img_src(led, &ui_img_bouton_noir_png, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(led, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_img_opa(led, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_opa(led, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        
        // IMPORTANT : Forcer aussi pour l'état PRESSED
        lv_obj_set_style_bg_img_opa(led, 255, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_clear_state(led, LV_STATE_CHECKED);
        if (current_selected_led == led) {
            current_selected_led = NULL;
        }
    }
}


void update_console_display()
{
    if (ui_console_label != NULL)
    {
        lv_label_set_text(ui_console_label, console_text);
    }
}

void keyboard_add_digit(const char* digit)
{
    size_t len = strlen(console_text);
    if (len < 4)        // limite de 4 caracteres
    {
        strcat(console_text, digit);
        update_console_display();
    }
}

void keyboard_backspace(lv_event_t * e) 
{
    size_t len = strlen(console_text);
    if (len > 0) {
        console_text[len - 1] = '\0';
        update_console_display();
    }
}

void keyboard_dot(lv_event_t *e)
{
    if (strchr(console_text, '.') == NULL)
    {
        keyboard_add_digit(".");
    }
}

void keyboard_0(lv_event_t * e){ keyboard_add_digit("0"); }
void keyboard_1(lv_event_t * e){ keyboard_add_digit("1"); }
void keyboard_2(lv_event_t * e){ keyboard_add_digit("2"); }
void keyboard_3(lv_event_t * e){ keyboard_add_digit("3"); }
void keyboard_4(lv_event_t * e){ keyboard_add_digit("4"); }
void keyboard_5(lv_event_t * e){ keyboard_add_digit("5"); }
void keyboard_6(lv_event_t * e){ keyboard_add_digit("6"); }
void keyboard_7(lv_event_t * e){ keyboard_add_digit("7"); }
void keyboard_8(lv_event_t * e){ keyboard_add_digit("8"); }
void keyboard_9(lv_event_t * e){ keyboard_add_digit("9"); }

void check_and_auto_save(void)
{
    int i = 0;
    bool all_filled = true;

    while (all_leds_array[i] != NULL) {
        lv_obj_t * led = *all_leds_array[i];
        const void * img_src = lv_obj_get_style_bg_img_src(led, LV_PART_MAIN);
        
        // Si une LED est encore noire, on ne sauvegarde pas encore
        if (img_src == &ui_img_bouton_noir_png || img_src == NULL) {
            all_filled = false;
            break;
        }
        i++;
    }

    if (all_filled && i > 0) {
        printf("Toutes les LEDs sont affectees. Sauvegarde automatique...\n");
        save_global_config_to_nvs();
    }
}

void add_key_highlighting(lv_obj_t * obj) 
{
    // Supprime le carré blanc par défaut dont on a parlé
    lv_obj_set_style_outline_width(obj, 0, LV_PART_MAIN | LV_STATE_ANY);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_ANY);

    // Ajoute la surbrillance bleue uniquement quand on appuie (PRESSED)
    lv_obj_set_style_outline_color(obj, lv_color_hex(0x00A2FF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_outline_width(obj, 6, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_outline_opa(obj, 200, LV_PART_MAIN | LV_STATE_PRESSED);
    
    lv_obj_set_style_outline_pad(obj, 2, LV_PART_MAIN | LV_STATE_PRESSED);

    // Optionnel : léger voile bleu sur le bouton
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x00A2FF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(obj, 60, LV_PART_MAIN | LV_STATE_PRESSED);
}












