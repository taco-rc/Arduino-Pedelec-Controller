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

#include "MainView.h"
#include "Components.h"

/**
 * Main view
 */

//! Constructor
MainView::MainView(Components* components)
        : m_components(components)
{
  model.addListener(this);
}

//! Destructor
MainView::~MainView() {
  // never deleted...
}

//! This view is now enabled and displayed
void MainView::activate() {
  BaseView::activate();
  m_components->deActivateChilren(m_active);
}

//! This view is now disabled and not displayed
void MainView::deactivate() {
  BaseView::deactivate();
  m_components->deActivateChilren(m_active);
}

//! Draw speed
void MainView::drawSpeed(bool repaint) {
  if (!m_active) {
    return;
  }

  const uint8_t speedY = 25;

  if (!repaint) {
    lcd.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  } else {
    lcd.setTextColor(ILI9341_WHITE);
  }

  uint16_t speed = model.getValue(VALUE_ID_SPEED);

  lcd.setTextSize(4);
  lcd.setCursor(45, speedY);
  String str = "";
  uint8_t speed10 = (speed / 100) % 10;

  if (speed10 == 0) {
    str += " ";
  } else {
    str += speed10;
  }

  str += (speed / 10) % 10;
  str += ".";
  str += speed % 10;
  lcd.print(str);

  lcd.setTextColor(ILI9341_WHITE);
  lcd.setTextSize(2);

  if (repaint) {
    lcd.setCursor(160, speedY - 5);
    lcd.print("km");
    lcd.setCursor(167, speedY + 17);
    lcd.print("h");

    lcd.drawLine(155, speedY + 12, 185, speedY + 12, ILI9341_WHITE);
    lcd.drawLine(155, speedY + 13, 185, speedY + 13, ILI9341_WHITE);
  }
}

//! Draw battery
void MainView::drawBattery(bool repaint) {
  if (!m_active) {
    return;
  }

  if (repaint) {
    lcd.drawRect(0, 9, 29, 7*9, ILI9341_WHITE);
    lcd.fillRect(10, 0, 9, 9, ILI9341_WHITE);
  } else {

  }
  lcd.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  uint16_t batterPercent = model.getValue(VALUE_ID_BATTERY_PERC_CURRENT);

  uint16_t batteryColor = RGB_TO_565(0, 255, 0);
  if (batterPercent <= 40) {
    batteryColor = ILI9341_YELLOW;
  }
  if (batterPercent <= 30) {
    batteryColor = RGB_TO_565(255, 0, 0);
  }

  for (uint8_t y = 0; y < 7; y++) {
    uint16_t barColor = ILI9341_BLACK;
    if ((6 - y) * 14 <= batterPercent) {
      barColor = batteryColor;
    }

    lcd.fillRect(2, y * 9 + 10, 25, 7, barColor);
  }

  lcd.setTextSize(2);

  lcd.setCursor(0, 75);

  String strPercent = "";
  if (batterPercent < 10) {
    strPercent += " ";
  }

  strPercent += batterPercent;
  strPercent += "%";

  if (batterPercent < 100) {
    strPercent += " ";
  }

  lcd.print(strPercent);
}


//! Draw wattage bar
void MainView::drawWattage(bool repaint) {
  if (!m_active) {
    return;
  }

  const uint8_t wattageBarHeight = 68;
  if (repaint) {
    lcd.drawRect(211, 2, 29, wattageBarHeight + 2, ILI9341_WHITE);
  }
  else {
  }
  lcd.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  uint16_t wattage = model.getValue(VALUE_ID_POWER);

  uint16_t max_wattage = model.getValue(VALUE_ID_MAX_POWER);

  //if (wattage > 500) {
  //  wattage = 500;
 // }

  uint8_t h;
  if (wattage == 0) {
    h = 0;
  } else {
    h = (wattageBarHeight * (wattage / 10)) / (max_wattage /  10);
  }
  uint8_t y = wattageBarHeight - h;

  lcd.fillRect(213, 3, 25, y, ILI9341_BLACK);

  uint16_t  barColor = ILI9341_WHITE;
  if (wattage > 0.33 * max_wattage) {
    barColor = ILI9341_YELLOW;
  }
  if (wattage > 0.66 * max_wattage) {
    barColor = ILI9341_RED;
  }

  lcd.fillRect(213, 3 + y, 25, h, barColor);

  String strWattage = "";
  strWattage += wattage;
  strWattage += "W";

  while (strWattage.length() < 5) {
    strWattage = " " + strWattage;
  }

  lcd.setCursor(180, 75);
  lcd.print(strWattage);
}

void MainView::drawKM(bool repaint) {
    // odo KM
  if (!m_active) {
    return;
  }
  lcd.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  lcd.setTextSize(2);
  lcd.setCursor(72, 75);
  uint16_t odo = model.getValue(VALUE_ID_ODO_CURRENT);

  String strOdo = "";

  uint16_t precision = 100;

  strOdo += (odo/precision);
  strOdo += ".";
  strOdo += (odo - (odo/precision)*precision);
  strOdo += " km";

  while (strOdo.length() < 8) {
    strOdo = " " + strOdo;
  }
  lcd.print(strOdo);
}


//! Update full display
void MainView::updateDisplay(bool repaint) {
  if (!m_active) {
    return;
  }

  // Clear full screen
  if (repaint)
    lcd.fillRect(0, 0, 240, 320, ILI9341_BLACK);

  drawSpeed(repaint);
  drawKM(repaint);
  drawBattery(repaint);
  drawWattage(repaint);

  m_components->draw(repaint);
}

//! UP / DOWN Key
void MainView::movePosition(int8_t diff) {
  m_components->changeView(diff);
    repaint = true;
}

//! Key (OK) pressed
ViewResult MainView::keyPressed() {
  ViewResult result;
  result.result = VIEW_RESULT_MENU;

  // Show settings menu
  result.value = 1;

  return result;
}

//! a value was changed
void MainView::onValueChanged(uint8_t valueId) {
  if (!m_active) {
    return;
  }
  switch (valueId) {
    case VALUE_ID_SPEED:
      drawSpeed(false);
          break;
    case VALUE_ID_BATTERY_PERC_CURRENT:
      drawBattery(false);
          break;
    case VALUE_ID_POWER:
      drawWattage(false);
          break;
    case VALUE_ID_ODO_CURRENT:
      drawKM(false);
    default:
      break;
  }


}
