//
// Created by daniel on 03/08/16.
//

#include "TimeComponent.h"


//! Draw the component to the display
void TimeComponent::draw(bool repaint) {
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

    int num_chars = 8; //hh:mm:ss

    for (int y = m_text.length() * 12; y < 240 - num_chars * 12; y+=12) {
        lcd.print(" ");
    }
    lcd.setCursor(240 - num_chars*12, m_y + 2);
    //hours
    lcd.printf("%02d", value/(60*60));
    lcd.print(":");
    value-= value/(60*60) * 60 * 60;

    //minutes
    lcd.printf("%02d", value/(60));
    lcd.print(":");
    value-= value/(60) * 60;

    //seconds
    lcd.printf("%02d", value);

}