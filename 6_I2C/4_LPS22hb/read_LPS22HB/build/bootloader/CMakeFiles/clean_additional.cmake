# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "bootloader.bin"
  "bootloader.map"
<<<<<<< HEAD
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
=======
  "config\\sdkconfig.cmake"
  "config\\sdkconfig.h"
>>>>>>> myorigin/master
  "project_elf_src_esp32.c"
  )
endif()
