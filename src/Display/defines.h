/*
ILI9341 LCD Display controller for Arduino_Pedelec_Controller

Copyright (C) 2016
Andreas Butti, andreas.b242 at gmail dot com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

/**
 * Defines, e.g. for Umlauts
 */
#ifndef DEFINES_H_
#define DEFINES_H_

#include "ILI9341_t3.h"

#define UE "\x81"
#define OE "\x99"
#define AE "\x83"

#define ae "\x84"
#define oe "\x94"
#define ue "\xa3"

#define RGB_TO_565(r, g, b) ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

extern ILI9341_t3 lcd;
extern boolean repaint;

class DataModel;
extern DataModel model;

enum IconId {
    ICON_ID_BLUETOOTH = (1 << 0),
    ICON_ID_BRAKE     = (1 << 1),
    ICON_ID_LIGHT     = (1 << 2),
    ICON_ID_PROFILE   = (1 << 3),
    ICON_ID_PAS       = (1 << 4),
    ICON_ID_HEART     = (1 << 5)
};

enum ValueId {
    VALUE_ID_NONE = 0,
    //! the speed in 0.1 km/h and update display if needed
    VALUE_ID_SPEED,
    //power in W;
    VALUE_ID_POWER,
    VALUE_ID_MAX_POWER,

    //! Current voltage
    VALUE_ID_BATTERY_VOLTAGE_CURRENT,
    //Cuurent bat perc
    VALUE_ID_BATTERY_PERC_CURRENT,
    //Current Bat mah
    VALUE_ID_BATTERY_MAH_USED,
    VALUE_ID_BATTERY_CURRENT,
    VALUE_ID_THROTTLE_POTI,
    VALUE_ID_THROTTLE_WRITE,
    VALUE_ID_SUPPORT_POTI,
    VALUE_ID_ODO_TOTAL,
    VALUE_ID_ODO_CURRENT,
    VALUE_ID_REMAINING_KM,
    VALUE_ID_TIME_DRIVEN,
    VALUE_ID_VESC_TEMP,
    VALUE_ID_MOTOR_CURRENT,
    VALUE_ID_MOTOR_RPM,
    VALUE_ID_ENC,
    VALUE_ID_CADENCE,
    VALUE_COUNT
};

enum {
    DISPLAY_ACTION_NONE = 0,
    DISPLAY_ACTION_SHUTDOWN,
    DISPLAY_ACTION_MENU_DISABLED,
    DISPLAY_ACTION_RESET_ODO,
    DISPLAY_ACTION_RESET_MAH,
    DISPLAY_ACTION_TOGGLE_LIGHTS,
    DISPLAY_ACTION_TOGGLE_BLUETOOTH,
    DISPLAY_ACTION_TOGGLE_ACTIVE_PROFILE,
    DISPLAY_ACTION_POTI_UP,
    DISPLAY_ACTION_POTI_DOWN,
    DISPLAY_ACTION_DISABLE_POTI,
    DISPLAY_ACTION_DISABLE_WHEEL,
    DISPLAY_ACTION_DISABLE_BRAKE,
    DISPLAY_ACTION_DISABLE_PAS,
    DISPLAY_ACTION_DISABLE_THROTTLE,
    DISPLAY_ACTION_ENABLE_POTI,
    DISPLAY_ACTION_ENABLE_WHEEL,
    DISPLAY_ACTION_ENABLE_BRAKE,
    DISPLAY_ACTION_ENABLE_PAS,
    DISPLAY_ACTION_ENABLE_THROTTLE
};

#endif