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
#ifndef DISPLAY_CONTROLLER_H_
#define DISPLAY_CONTROLLER_H_

#include "Arduino.h"

/**
 * Control the whole display Navigation and output
 */

//! Call once on startup
void displayControllerSetup();

//! Call in the main loop
int keyPressed();

void exitMenu();

void updatePosition(int8_t diff);

void updateDisplay();

void updateDataModel(uint8_t value_id, uint16_t value);

void updateIconModel(uint8_t icon_id, boolean value);

#endif