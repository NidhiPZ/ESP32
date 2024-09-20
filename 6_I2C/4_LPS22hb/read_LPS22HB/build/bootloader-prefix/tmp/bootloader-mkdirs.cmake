# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
<<<<<<< HEAD
  "/home/dnk133/esp/v5.3.1/esp-idf/components/bootloader/subproject"
  "/home/dnk133/Desktop/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader"
  "/home/dnk133/Desktop/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix"
  "/home/dnk133/Desktop/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix/tmp"
  "/home/dnk133/Desktop/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix/src/bootloader-stamp"
  "/home/dnk133/Desktop/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix/src"
  "/home/dnk133/Desktop/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix/src/bootloader-stamp"
=======
  "C:/Users/DNK133/ESP/v5.3/esp-idf/components/bootloader/subproject"
  "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader"
  "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix"
  "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix/tmp"
  "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix/src"
  "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix/src/bootloader-stamp"
>>>>>>> myorigin/master
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
<<<<<<< HEAD
    file(MAKE_DIRECTORY "/home/dnk133/Desktop/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/dnk133/Desktop/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
=======
    file(MAKE_DIRECTORY "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Nidhi/ESP32/6_I2C/4_LPS22hb/read_LPS22HB/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
>>>>>>> myorigin/master
endif()
