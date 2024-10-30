# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/dnk133/esp/v5.3.1/esp-idf/components/bootloader/subproject"
  "/home/dnk133/Desktop/Nidhi/ESP32/11_WIFI/Esp_softAp/build/bootloader"
  "/home/dnk133/Desktop/Nidhi/ESP32/11_WIFI/Esp_softAp/build/bootloader-prefix"
  "/home/dnk133/Desktop/Nidhi/ESP32/11_WIFI/Esp_softAp/build/bootloader-prefix/tmp"
  "/home/dnk133/Desktop/Nidhi/ESP32/11_WIFI/Esp_softAp/build/bootloader-prefix/src/bootloader-stamp"
  "/home/dnk133/Desktop/Nidhi/ESP32/11_WIFI/Esp_softAp/build/bootloader-prefix/src"
  "/home/dnk133/Desktop/Nidhi/ESP32/11_WIFI/Esp_softAp/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/dnk133/Desktop/Nidhi/ESP32/11_WIFI/Esp_softAp/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/dnk133/Desktop/Nidhi/ESP32/11_WIFI/Esp_softAp/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()