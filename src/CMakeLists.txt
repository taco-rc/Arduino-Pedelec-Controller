#=============================================================================#
# Author: Thomas Jarosch                                                      #
# Date:   24.04.2012                                                          #
#                                                                             #
# Description: Pedelec controller cmake project                               #
#                                                                             #
#=============================================================================#
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/Arduino_Pedelec_Controller)

set(FIRMWARE_NAME pcontroller)

#set(${FIRMWARE_NAME}_BOARD ${FC_PROCESSOR})
#set(${FIRMWARE_NAME}_PORT /dev/ttyUSB0)            # Serial upload port

# special compile flags for the Travis CI environment
# (avr-gcc is rather outdated and outputs crazy warnings)
if("$ENV{TRAVIS}" STREQUAL "true")
    message(STATUS "Detected Travis CI environment")

    # disable compiler invocation output since Travis CI has a 4MB log output limit
    set_property(GLOBAL PROPERTY RULE_MESSAGES off)
endif("$ENV{TRAVIS}" STREQUAL "true")

# Hack until we replace the .ino file with a stub
# and move everything to main.cpp
#configure_file(main.cpp main.cpp COPYONLY)


import_arduino_library(EEPROM)
import_arduino_library(SPI)
import_arduino_library(ILI9341_t3)
import_arduino_library(OneWire)
import_arduino_library(Wire)


set(${FIRMWARE_NAME}_SRCS
        main.cpp
        BMP085.cpp
        #BMP085.h
        DSPC01_nano.cpp
        #DSPC01_nano.h
        DallasTemp.cpp
        #DallasTemp.h
        HX711.cpp
        #HX711.h
        LiquidCrystalDogm.cpp
        #LiquidCrystalDogm.h
        MenuSystem.cpp
        #MenuSystem.h
        PCD8544_charset.cpp
        PCD8544_nano.cpp
        #PCD8544_nano.h
        PID_v1_nano.cpp
        #PID_v1_nano.h
        #config.h
        display.cpp
        #display.h
        display_backlight.cpp
        #display_backlight.h
        display_bafang.cpp
        #display_bafang.h
        display_kingmeter.cpp
        #display_kingmeter.h
        ds1307.cpp
        #ds1307.h
        #globals.h
        hrmi_funcs.cpp
        #hrmi_funcs.h
        menu.cpp
        #menu.h
        serial_command.cpp
        #serial_command.h
        serial_lcd.cpp
        #serial_lcd.h
        switches.cpp
        #switches.h
        switches_action.h
        VESC/buffer.cpp
        #VESC/buffer.h
        VESC/crc.cpp
        #VESC/crc.h
        #VESC/config.h
        #VESC/datatypes.h
        VESC/vesc_uart.cpp
        Display/BaseComponent.cpp
        Display/BaseView.cpp
        Display/Components.cpp
        Display/DataModel.cpp
        Display/DiagramComponent.cpp
        Display/DisplayController.cpp
        Display/IconComponent.cpp
        Display/MainView.cpp
        Display/MainViewEdit.cpp
        Display/MenuEntries.cpp
        Display/MenuView.cpp
        Display/SeparatorComponent.cpp
        Display/TextComponent.cpp
        Display/TimeComponent.cpp
        Encoder/Encoder.cpp
        )

add_teensy_executable(${FIRMWARE_NAME} "${${FIRMWARE_NAME}_SRCS}")

