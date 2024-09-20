# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/DNK133/ESP/v5.3/esp-idf/components/bootloader/subproject"
  "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/2_read_LPS22Hb/build/bootloader"
  "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/2_read_LPS22Hb/build/bootloader-prefix"
  "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/2_read_LPS22Hb/build/bootloader-prefix/tmp"
  "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/2_read_LPS22Hb/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/2_read_LPS22Hb/build/bootloader-prefix/src"
  "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/2_read_LPS22Hb/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/2_read_LPS22Hb/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/2_read_LPS22Hb/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
