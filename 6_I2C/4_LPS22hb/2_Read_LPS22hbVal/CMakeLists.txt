# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS /home/dnk133/Desktop/Nidhi/ESP32/6_I2C/4_LPS22hb/2_Read_LPS22hbVal/Components/BSP/lps22hb)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(2_Read_LPS22hbVal)