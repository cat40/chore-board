cmake_minimum_required(VERSION 3.13)
# Note! Minimum GCC/G++ version is 10.4. I think. Anyway it works on 14.3.1; just update if you have issues related to the defaults.hpp file

set(PICO_BOARD_HEADER_DIRS ${CMAKE_SOURCE_DIR}) 
set(PICO_BOARD chore_board_hw)

# add_compile_options(-fanalyzer -fstack-usage)
add_compile_options(-fstack-usage)
# add_compile_options(-fsanitize=address -g3 -Wall -Wextra)

include(pico_sdk_import.cmake)

project(choreboard_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(raspberry-kitten/utilities)
add_subdirectory(raspberry-kitten/button_matrix)
add_subdirectory(raspberry-kitten/eeprom)
add_subdirectory(raspberry-kitten/pcf8523)
add_subdirectory(pi-pico-adafruit-neopixels/pico_neopixels)

# git commit hash macro, from https://stackoverflow.com/a/39739194
execute_process(
  COMMAND git describe --match="" --always --abbrev=7 --dirty
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions("-DCOMMIT_HASH=\"${COMMIT_HASH}\"")

add_executable(choreboard
  main.cpp
  settings.cpp
  chore.cpp
  command.c
  defaults.cpp
)

add_compile_options(-fstack-usage)

target_link_libraries( choreboard 
    pico_stdlib
    pico_rand
    hardware_i2c
    hardware_adc
    hardware_uart
    pico_neopixel
    kitten-utilities
    kitten-matrix
    kitten-eeprom
    kitten-pcf8523
)

target_link_options(choreboard PUBLIC "LINKER:--print-memory-usage,-Map=memory.map,--gc-sections")

pico_enable_stdio_usb(choreboard 1)
pico_enable_stdio_uart(choreboard 0)

pico_add_extra_outputs(choreboard)

