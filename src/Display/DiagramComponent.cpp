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
 * Display a diagram with a history of the value
 */

#include "DiagramComponent.h"

uint16_t map_to_uint(float_t x, float_t in_min, float_t in_max, int16_t out_min, int16_t
out_max)
{
    return uint16_t((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

float_t map_to_float(long x, int16_t in_min, int16_t in_max, float_t out_min, float_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//! Constructor
DiagramComponent::DiagramComponent(String text, ValueId value, float_t min, float_t max)
        : m_text(text),
          m_display_value_id(value),
          m_data{0},
          m_cur_pose_index(0),
          m_precision(1),
          m_last_draw(0),
          current_value(0.0),
          current_count(0),
          min_value(min),
          max_value(max)
{
    model.addListener(this);
}

//! Destructor
DiagramComponent::~DiagramComponent() {
}

//! Y Position on display
uint8_t DiagramComponent::getHeight() {
    return 61;
}

void DiagramComponent::onValueChanged(uint8_t valueId){
    if (valueId == m_display_value_id) {
        current_count++;
        current_value *= float((current_count - 1)) / current_count;
        current_value += float(model.getValue(m_display_value_id))/current_count;

        //if (millis() > m_last_draw + UPDATE_PERIOD_S) {
        //    this->draw(false);
        //}
    }
}


const uint16_t DIAGRAM_LINE_COLOR = RGB_TO_565(0xBE, 0x82, 0x34);
const uint16_t DIAGRAM_DATA_COLOR = RGB_TO_565(0x28, 0x2B, 0xDA);

//! Draw the component to the display
void DiagramComponent::draw(bool repaint) {
    if (!m_active)
        return;

    //reset counts
    if (millis() > m_last_draw + UPDATE_PERIOD_S) {
        m_cur_pose_index += 1;
        m_cur_pose_index %= DATA_LENGTH;

        m_data[m_cur_pose_index] = current_value; //map_to_uint(current_value, min_value, max_value, 0, 1023);

        m_last_draw = millis();
        current_count = 1;
        current_value = model.getValue(m_display_value_id);
    }
    else {
        return;
    }

    //long max_val = 0;
    float_t max_val = 0.;

    for (uint8_t i = 0; i < DATA_LENGTH - 1; i++) {
        max_val = max(m_data[i], max_val);
    }
    //long mapped = map(max_val, 0, max_val, 0, 59);
    uint16_t mapped = map_to_uint(max_val, 0, max_val, 0, 59);
    lcd.fillRect(0, m_y + 60 - mapped, 240, mapped, ILI9341_BLACK);

#ifndef BUFFER_SHIFT
    for (uint16_t i = 0; i < DATA_LENGTH -1; i++) {
        uint16_t x = i * 2;
        uint16_t index = (m_cur_pose_index + i + 1) % DATA_LENGTH;
        uint16_t index1 = (m_cur_pose_index + i + 2) % DATA_LENGTH;


        uint16_t y1 = m_y + 59 - constrain(map_to_uint((m_data[index]), 0, max_val, 0, 59), 0, 59);
        uint16_t y2 = m_y + 59 - constrain(map_to_uint((m_data[index1]), 0, max_val, 0, 59), 0, 59);

        lcd.drawLine(x, y1, x + 2, y2, ILI9341_GREEN);
        lcd.drawLine(x, y1 + 1, x + 2, y2 + 1, ILI9341_GREEN);
    }
#else //NOT WORKING WITHOUT MISO, never tested
    uint16_t awColors[60*2];

    for (uint8_t i = 0; i < DATA_LENGTH - 2; ++i) {
        lcd.readRect(i+1, m_y, 1, 60, awColors);
        lcd.writeRect(i, m_y, 1, 60, awColors);
    }


    uint8_t index = (m_cur_pose_index-1) % DATA_LENGTH;
    uint8_t index1 = (m_cur_pose_index) % DATA_LENGTH;

    uint16_t y1 = m_y + 60 - map(long(m_data[index]), 0, 1023, 0, 60);
    uint16_t y2 = m_y + 60 - map(long(m_data[index1]), 0, 1023, 0, 60);

    lcd.drawLine(240-2, y1, 240-2 + 2, y2, DIAGRAM_DATA_COLOR);
    lcd.drawLine(240-2, y1 + 1, 240-2 + 2, y2 + 1, DIAGRAM_DATA_COLOR);

#endif
    for (uint8_t i = 0; i <= 60; i += 30) {
        lcd.drawLine(0, m_y + i, 240, m_y + i, DIAGRAM_LINE_COLOR);
    }

    for (uint16_t x = 20; x < 240; x += 40) {
        lcd.drawLine(x, m_y + 1, x, m_y + 58, DIAGRAM_LINE_COLOR);
    }

    lcd.setTextSize(2);
    lcd.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    lcd.setCursor(0, m_y + 2 + 45);
    lcd.print(m_text.c_str());
    // print current max value
    lcd.setCursor(0, m_y + 2);
    //lcd.print(constrain(map_to_float(max_val, 0, 1023, min_value, max_value), min_value, max_value)/m_precision, 1);
    if (m_precision > 0) {
        int factor = (int)pow(10, m_precision);
        lcd.printf("%.*f", m_precision, float(max_val/factor));
    }
    else {
      lcd.printf("%d", (int)max_val);
    }
}

void DiagramComponent::set_text(const String &m_text) {
    DiagramComponent::m_text = m_text;
}

void DiagramComponent::set_display_value_id(ValueId m_display_value_id) {
    m_cur_pose_index = 0;
    current_count = 0;
    current_value = 0.0;
    m_last_draw = millis();

    DiagramComponent::m_display_value_id = m_display_value_id;
    for (int i = 0; i < DATA_LENGTH - 1; ++i) {
        m_data[i] = 0;
    }

}

void DiagramComponent::set_min_max(float_t min, float_t max) {
    min_value = min;
    max_value = max;
}

void DiagramComponent::set_precision(int16_t precision) {
    m_precision = precision;
}

