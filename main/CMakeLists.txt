idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES display_init sd_card slider screen lvgl waveshare__esp32_p4_wifi6_touch_lcd_7b esp_lvgl_port)