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

#include "IconComponent.h"

#define ICON_TOP_Y (m_y + 2)

//! Constructor
IconComponent::IconComponent() {
    model.addListener(this);
}

//! Destructor
IconComponent::~IconComponent() {
    model.removeListener(this);
}

const uint16_t ICON_HEIGHT = 40;

//! Y Position on display
uint8_t IconComponent::getHeight() {
    return ICON_HEIGHT + 2 + 3;
}

const uint16_t ICON_DISABLED_COLOR = RGB_TO_565(60, 60, 60);

//! Draw Bluetooth Icon
void IconComponent::drawBluetooth(bool repaint) {
    if (!m_active || !repaint) {
        return;
    }

    uint16_t ix = 210;
    uint16_t blX1 = ix;
    uint16_t blX2 = ix + 10;
    uint16_t blX3 = ix + 20;

    uint16_t iconColor;
    if (model.getIcon() & ICON_ID_BLUETOOTH) {
        iconColor = ILI9341_BLUE;
    } else {
        iconColor = ICON_DISABLED_COLOR;
    }

    lcd.drawLine(blX2, ICON_TOP_Y, blX2, ICON_TOP_Y + ICON_HEIGHT, iconColor);

    lcd.drawLine(blX2, ICON_TOP_Y, blX3, ICON_TOP_Y + ICON_HEIGHT/4, iconColor);
    lcd.drawLine(blX3, ICON_TOP_Y + ICON_HEIGHT - ICON_HEIGHT/4, blX1, ICON_TOP_Y + 10, iconColor);

    lcd.drawLine(blX2, ICON_TOP_Y + ICON_HEIGHT, blX3, ICON_TOP_Y + ICON_HEIGHT - ICON_HEIGHT/4, iconColor);
    lcd.drawLine(blX3, ICON_TOP_Y + ICON_HEIGHT/4, blX1, ICON_TOP_Y + ICON_HEIGHT - 10, iconColor);

}

//! Draw Brakes Icon
void IconComponent::drawPAS(bool repaint) {
    if (!m_active || !repaint) {
        return;
    }

    uint16_t ix = 5;
    uint8_t breakRadius = 20;
    uint8_t offset = 7;
    uint16_t iconColor;
    if (model.getIcon() & ICON_ID_PAS) {
        iconColor = ILI9341_GREEN;
    } else {
        iconColor = ICON_DISABLED_COLOR;
    }

    for (uint8_t i = 0; i < 2; i++) {
        lcd.drawCircle(ix + breakRadius, ICON_TOP_Y + breakRadius, breakRadius - i, iconColor);
        lcd.drawCircle(ix + breakRadius+breakRadius/2, ICON_TOP_Y + breakRadius, breakRadius - i, iconColor);
    }

    lcd.fillRect(ix + offset, ICON_TOP_Y, breakRadius + breakRadius - offset + 4, breakRadius + breakRadius + 2, ILI9341_BLACK);

    //lcd.drawCircle(ix + breakRadius + 7, ICON_TOP_Y + breakRadius, breakRadius, iconColor);
    //lcd.drawCircle(ix + breakRadius + 7, ICON_TOP_Y + breakRadius, breakRadius-1, iconColor);

    lcd.setTextSize(2);
    if (model.getIcon() & ICON_ID_PAS)
        lcd.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    else
        lcd.setTextColor(ICON_DISABLED_COLOR, ILI9341_BLACK);
    lcd.setCursor(ix + 8, ICON_TOP_Y + 14);
    lcd.print("PAS");
    /*for (int8_t i = -1; i < 2; i++) {
      uint8_t bx = ix + breakRadius + i + 7;
      uint8_t y1 = ICON_TOP_Y + 6;
      uint8_t y2 = ICON_TOP_Y + ICON_HEIGHT - 7 - 6;

      lcd.drawLine(bx, y1, bx, y2, iconColor);
      lcd.drawLine(bx, y2 + 4, bx, y2 + 7, iconColor);
    }*/
}


//! Draw Brakes Icon
void IconComponent::drawBrakes(bool repaint) {
    if (!m_active || !repaint) {
        return;
    }

    uint16_t ix = 67;
    uint8_t breakRadius = 20;

    uint16_t iconColor;
    if (model.getIcon() & ICON_ID_BRAKE) {
        iconColor = ILI9341_RED;
    } else {
        iconColor = ICON_DISABLED_COLOR;
    }

    for (uint8_t i = 0; i < 2; i++) {
        lcd.drawCircle(ix + breakRadius, ICON_TOP_Y + breakRadius, breakRadius - i, iconColor);
        lcd.drawCircle(ix + breakRadius+14, ICON_TOP_Y + breakRadius, breakRadius - i, iconColor);
    }

    lcd.fillRect(ix + 7, ICON_TOP_Y, breakRadius + breakRadius, breakRadius + breakRadius + 2, ILI9341_BLACK);

    lcd.drawCircle(ix + breakRadius + 7, ICON_TOP_Y + breakRadius, breakRadius, iconColor);
    lcd.drawCircle(ix + breakRadius + 7, ICON_TOP_Y + breakRadius, breakRadius-1, iconColor);


    for (int8_t i = -1; i < 2; i++) {
        uint8_t bx = ix + breakRadius + i + 7;
        uint8_t y1 = ICON_TOP_Y + 6;
        uint8_t y2 = ICON_TOP_Y + ICON_HEIGHT - 7 - 6;

        lcd.drawLine(bx, y1, bx, y2, iconColor);
        lcd.drawLine(bx, y2 + 4, bx, y2 + 7, iconColor);
    }
}

//! Draw Light Icon
void IconComponent::drawLight(bool repaint) {
    if (!m_active || !repaint) {
        return;
    }
    uint16_t ix = 137;
    uint8_t lightRadius = 12;
    uint8_t top_offset = 2;
    uint16_t iconColor;
    if (model.getIcon() & ICON_ID_LIGHT) {
        iconColor = ILI9341_YELLOW;
    } else {
        iconColor = ICON_DISABLED_COLOR;
    }

    lcd.fillCircle(ix + lightRadius, top_offset + ICON_TOP_Y + lightRadius, lightRadius, iconColor);
    lcd.fillRect(ix + lightRadius - 4, top_offset + ICON_TOP_Y + lightRadius * 2, 9, 8, iconColor);

    lcd.fillRect(ix + lightRadius - 2, top_offset + ICON_TOP_Y + lightRadius * 2 + 10, 5, 2, iconColor);
}


//! Draw Light Icon
void IconComponent::drawProfile(bool repaint) {
    if (!m_active || !repaint) {
        return;
    }

    uint16_t ix = 177;

    uint16_t iconNum;
    if (model.getIcon() & ICON_ID_PROFILE) {
        iconNum = 2;
        lcd.setTextColor(ILI9341_RED, ILI9341_BLACK);
    } else {
        iconNum = 1;
        lcd.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
    }
    lcd.setCursor(ix, ICON_TOP_Y + 5);
    lcd.setTextSize(4);
    lcd.print(iconNum);

}


//! Draw the component to the display
void IconComponent::draw(bool repaint) {
    drawPAS(repaint);
    drawBrakes(repaint);
    drawLight(repaint);
    drawProfile(repaint);
    drawBluetooth(repaint);
}

//! Icon changed
void IconComponent::onIconUpdate(uint8_t iconId) {
    if (iconId & ICON_ID_BLUETOOTH) {
        drawBluetooth(true);
    }
    if (iconId & ICON_ID_BRAKE) {
        drawBrakes(true);
    }
    if (iconId & ICON_ID_LIGHT) {
        drawLight(true);
    }
    if (iconId & ICON_ID_PROFILE) {
        drawProfile(true);
    }
    if (iconId & ICON_ID_PAS) {
        drawPAS(true);
    }
}
