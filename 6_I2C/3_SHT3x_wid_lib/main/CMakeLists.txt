idf_component_register(SRCS "humid_temp.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_idf_lib_helpers SHT3x i2c_dev)
