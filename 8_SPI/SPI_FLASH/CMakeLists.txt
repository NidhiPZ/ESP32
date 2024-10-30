# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS /home/dnk133/Desktop/Nidhi/ESP32/8_SPI/SPI_FLASH/components/mx25r3235f)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(SPI_FLASH)