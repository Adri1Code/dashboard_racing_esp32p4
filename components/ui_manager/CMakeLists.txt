idf_component_register(SRCS "ui_manager.c;ui_manager_slider.c"
                    INCLUDE_DIRS "include"
                    REQUIRES ui lvgl slider memory nvs_flash waveshare__esp32_p4_wifi6_touch_lcd_7b)