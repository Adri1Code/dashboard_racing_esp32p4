file(GLOB_RECURSE SOURCES 
    "*.c"
    "components/*.c"
    "fonts/*.c"
    "images/*.c"
    "screens/*.c"
)

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS "." 
                "components" 
                "fonts"
                "images"
                "screens"          
    REQUIRES lvgl
)