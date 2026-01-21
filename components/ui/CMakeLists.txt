file(GLOB_RECURSE SOURCES 
    "ui.c"
    "ui_helpers.c"
    "ui_events.c"
    "components/*.c"
    "fonts/*.c"
    "screens/*.c"
)

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS "." 
                "components" 
                "fonts"
                "screens" 
    REQUIRES lvgl  
)
