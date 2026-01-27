idf_component_register(
    SRCS 
        "main.c"
    INCLUDE_DIRS 
        "."
    REQUIRES 
        fatfs 
        ui_manager 
        display_init
        sd_card 
        slider 
        screen 
        memory 
        lvgl 
        waveshare__esp32_p4_wifi6_touch_lcd_7b 
        esp_lvgl_port nvs_flash
)