idf_component_register(SRCS "memory.c"
                    INCLUDE_DIRS "include"
                    REQUIRES fatfs waveshare__esp32_p4_wifi6_touch_lcd_7b spi_flash)