idf_component_register(SRCS "SHT3x.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver esp_timer esp_idf_lib_helpers i2c_dev)
