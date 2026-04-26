# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/milu/.espressif/v5.5.3/esp-idf/components/bootloader/subproject"
  "/home/milu/Documents/EspIDF/progetti/BME280/build/bootloader"
  "/home/milu/Documents/EspIDF/progetti/BME280/build/bootloader-prefix"
  "/home/milu/Documents/EspIDF/progetti/BME280/build/bootloader-prefix/tmp"
  "/home/milu/Documents/EspIDF/progetti/BME280/build/bootloader-prefix/src/bootloader-stamp"
  "/home/milu/Documents/EspIDF/progetti/BME280/build/bootloader-prefix/src"
  "/home/milu/Documents/EspIDF/progetti/BME280/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/milu/Documents/EspIDF/progetti/BME280/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/milu/Documents/EspIDF/progetti/BME280/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
