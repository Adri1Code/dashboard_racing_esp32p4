idf_component_register(SRCS "sd_card.c" "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES lvgl waveshare__esp32_p4_wifi6_touch_lcd_7b esp_lvgl_port)