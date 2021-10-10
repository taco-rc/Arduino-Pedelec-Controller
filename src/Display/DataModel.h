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

#ifndef DATAMODEL_H_
#define DATAMODEL_H_
/**
 * Model with all data which can be displayed by the different views
 */

#include "Arduino.h"
#include "defines.h"




class DataListener {
public:
    //! Icon changed
    virtual void onIconUpdate(uint8_t iconId) {};

    //! a value was changed
    virtual void onValueChanged(uint8_t valueId) {};

};

class DataModel {
    // Constructor / Destructor
public:
    //! Constructor
    DataModel();

    // public API
public:
    //! Clear an icon
    void clearIcon(uint8_t icon);

    void setIcon(uint8_t icon, boolean value);

    //! Show an icon
    void showIcon(uint8_t icon);

    //! Bitmask with icon state
    uint8_t getIcon();

    //! Set the value
    void setValue(uint8_t valueId, uint16_t value);

    //! Get a value
    uint16_t getValue(uint8_t valueId);

public:
    //! Add a listener to get informed about changes
    void addListener(DataListener* listener);

    //! Remove a listener
    void removeListener(DataListener* listener);

private:
    //! fire an icon state change
    void fireIconUpdate(uint8_t iconId);

    //! fire a value changed
    void fireValueUpdate(uint8_t valueId);

    // Member
private:
    //! Bitmask with icon state
    uint8_t m_iconState;

    //! Listener list
    DataListener* m_listener[VALUE_COUNT * 2]; //maximum two listeners per value

    //! Values
    uint16_t m_values[VALUE_COUNT];
};
#endif