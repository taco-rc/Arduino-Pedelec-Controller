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
 * Display a text with value on the display
 */

#include "TextComponent.h"

//! Constructor
TextComponent::TextComponent(String text, ValueId value, int precision)
             : m_text(text),
               m_display_value_id(value),
               m_precision(precision)
{
   model.addListener(this);
}

//! Destructor
TextComponent::~TextComponent() {
   //model.removeListener(this);
}

//! Y Position on display
uint8_t TextComponent::getHeight() {
   return 18;
}

void TextComponent::onValueChanged(uint8_t valueId){
   if (valueId == m_display_value_id)
      this->draw(false);
}

//! Draw the component to the display
void TextComponent::draw(bool repaint) {
   if (!m_active)
      return;
   lcd.setTextSize(2);
   lcd.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

   //if (repaint){
      lcd.setCursor(0, m_y + 2);
      lcd.print(m_text.c_str());

   //}
   lcd.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);

   uint16_t value = model.getValue(m_display_value_id);
   //uint16_t value = 0;

   //right align
   String num = String(value);
   int num_chars = num.length();
   if (m_precision > 0)
      num_chars = max(m_precision + 2, num_chars - m_precision + 2);


   for (int y = m_text.length() * 12; y < 240 - num_chars * 12; y+=12) {
      lcd.print(" ");
   }
   lcd.setCursor(240 - num_chars*12, m_y + 2);


   if (m_precision > 0) {
      int factor = (int)pow(10, m_precision);
      lcd.print(value/factor);
      lcd.print(".");
      lcd.printf("%0*d",m_precision, value - (value/factor)*factor);
   }
   else {
      lcd.print(value);
   }
}
