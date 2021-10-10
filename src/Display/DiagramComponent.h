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

#pragma once
#define DATA_LENGTH 120
#define UPDATE_PERIOD_S 1.0 * 1000

#include "BaseComponent.h"

/**
 * Display a diagram with a history of the value
 */

class DiagramComponent: public BaseComponent, public DataListener {
    // Constructor / Destructor
public:
    //! Constructor
    DiagramComponent(String text, ValueId value, float_t min, float_t max);


    //! Destructor
    virtual ~DiagramComponent();

    //! Return the height in pixel
    virtual uint8_t getHeight();

    //! Draw the component to the display
    virtual void draw(bool repaint);

    virtual void onValueChanged(uint8_t valueId);

    void set_text(const String &m_text);
    void set_display_value_id(ValueId m_display_value_id);
    void set_min_max(float_t min, float_t max);
    void set_precision(int16_t precision);

    // Member
private:
    //! Diagram data (the display is 240px wide / 2px)
    String m_text;


private:
    ValueId m_display_value_id;
    float_t m_data[DATA_LENGTH];
    uint8_t m_cur_pose_index;
    int16_t m_precision;
    long m_last_draw;
    float current_value;
    uint16_t current_count;
    float min_value;
    float max_value;
};
