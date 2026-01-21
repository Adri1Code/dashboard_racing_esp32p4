# 1. On liste les sources manuellement pour être précis ou on utilise GLOB
file(GLOB_RECURSE SOURCES 
    "ui.c"
    "ui_helpers.c"
    "ui_events.c"
    "components/*.c"
    "fonts/*.c"
    "screens/*.c"
)

# 2. Enregistrement du composant pour l'ESP32
idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS "." 
                "components" 
                "fonts"
                "screens" 
    REQUIRES lvgl  # Ou le nom de ton composant LVGL (souvent 'lvgl')
)