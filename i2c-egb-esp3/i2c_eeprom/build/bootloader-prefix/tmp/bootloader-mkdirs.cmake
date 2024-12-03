# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Alan/esp/v5.2.1/esp-idf/components/bootloader/subproject"
  "C:/Users/Alan/Documents/GIT_propios/td3_egb_2024/i2c-egb-esp3/i2c_eeprom/build/bootloader"
  "C:/Users/Alan/Documents/GIT_propios/td3_egb_2024/i2c-egb-esp3/i2c_eeprom/build/bootloader-prefix"
  "C:/Users/Alan/Documents/GIT_propios/td3_egb_2024/i2c-egb-esp3/i2c_eeprom/build/bootloader-prefix/tmp"
  "C:/Users/Alan/Documents/GIT_propios/td3_egb_2024/i2c-egb-esp3/i2c_eeprom/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/Alan/Documents/GIT_propios/td3_egb_2024/i2c-egb-esp3/i2c_eeprom/build/bootloader-prefix/src"
  "C:/Users/Alan/Documents/GIT_propios/td3_egb_2024/i2c-egb-esp3/i2c_eeprom/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Alan/Documents/GIT_propios/td3_egb_2024/i2c-egb-esp3/i2c_eeprom/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Alan/Documents/GIT_propios/td3_egb_2024/i2c-egb-esp3/i2c_eeprom/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
