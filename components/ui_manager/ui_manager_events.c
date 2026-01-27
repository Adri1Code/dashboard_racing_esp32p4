#include "ui_manager.h"

extern const lv_img_dsc_t ui_img_bouton_vert_png;
extern const lv_img_dsc_t ui_img_bouton_orange_png;
extern const lv_img_dsc_t ui_img_bouton_rouge_png;
extern const lv_img_dsc_t ui_img_bouton_cyan_png;
extern const lv_img_dsc_t ui_img_bouton_bleu_png;
extern const lv_img_dsc_t ui_img_bouton_noir_png;

// Variable globale pour stocker la LED sélectionnée
lv_obj_t * current_selected_led = NULL;

static char console_text[32] = "";      // Buffer pour le texte affiché 
lv_obj_t *ui_console_label = NULL;      // Label pour afficher le texte

// Fonction pour mémoriser quelle LED noire a été cliquée
void set_target_led(lv_event_t * e) {
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
void _apply_color(const lv_img_dsc_t * img_src) {
    if (current_selected_led != NULL) {
        // On remet l'opacité à fond
        lv_obj_set_style_opa(current_selected_led, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        // On change l'image de fond
        lv_obj_set_style_bg_img_src(current_selected_led, img_src, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_clear_state(current_selected_led, LV_STATE_CHECKED);
        current_selected_led = NULL;   
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

void keyboard_add_digit(const char* digit){
    size_t len = strlen(console_text);
    if (len < 4)        // limite de 4 caracteres
    {
        strcat(console_text, digit);
        update_console_display();
    }
}

void keyboard_backspace(lv_event_t * e) {
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


void add_key_highlighting(lv_obj_t * obj) {
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












