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

#include "DisplayController.h"

#include "MainView.h"
#include "MenuView.h"
#include "MainViewEdit.h"
#include "Components.h"
#include "BaseView.h"

/**
 * Control the whole display Navigation and output
 */

//! Model with all data
DataModel model;

//! Customizeable components on the main screen
Components components;

//! Main view with speed etc.
MainView mainView(&components);

//! Edit custmizeable part of the main view
MainViewEdit mainViewEdit(&components);

//! Menu view to show a menu
MenuView menuView;

//! Current active view
BaseView *currentView;

//! DISPLAY
#define TFT_MOSI 11
#define TFT_CLK 13
#define TFT_MISO 12

#define TFT_DC  9
#define TFT_CS 10
#define TFT_RST 10

// Use Hardware SPI
ILI9341_t3 lcd = ILI9341_t3(TFT_CS, TFT_DC);//, TFT_RST);

boolean repaint = true;

//! Call once on startup
void displayControllerSetup() {
    lcd.begin();
    repaint = true;
    currentView = &mainView;
    currentView->activate();
}


void updatePosition(int8_t diff) {
    currentView->movePosition(diff);
}

void updateDisplay() {
    if (repaint) {
        currentView->updateDisplay(repaint);
        repaint = false;
    }
    //always draw Diagramm if active
    if (components.g_components[COMP_ID_DIAG]->is_active())
        components.g_components[COMP_ID_DIAG]->draw(false);
}

void exitMenu(){
    currentView->deactivate();
    currentView = &mainView;
    currentView->activate();
    repaint = true;
}

int keyPressed() {
    ViewResult result = currentView->keyPressed();
    //Serial.println("KEY Result: ");
    //Serial.print(result.result);
    //Serial.print(result.value);
    //Serial.println();
    int response = DISPLAY_ACTION_NONE;

    if (result.result == VIEW_RESULT_MENU) {
        currentView->deactivate();
        menuView.setRootMenuId(result.value);
        currentView = &menuView;
        currentView->activate();
    } else if (result.result == VIEW_RESULT_BACK) {
        currentView->deactivate();
        currentView = &mainView;
        currentView->activate();
        response = DISPLAY_ACTION_MENU_DISABLED;
    } else if (result.result == VIEW_RESULT_SELECTED) {
        currentView->deactivate();

        if (MENU_ID_VIEW_EDIT == result.value) {
            currentView = &mainViewEdit;
        }
        else if (MENU_ID_COMPONENT_REMOVE == result.value) {
            mainViewEdit.removeSelected();
            currentView = &mainViewEdit;
        }
        else if(result.value == MENU_ID_ADD_POTI_CB) {
            response = DISPLAY_ACTION_POTI_UP;
        }
        else if(result.value == MENU_ID_DEC_POTI_CB) {
            response = DISPLAY_ACTION_POTI_DOWN;
        }
        else if(result.value == MENU_ID_LIGHT_CB) {
            response = DISPLAY_ACTION_TOGGLE_LIGHTS;
        }
        else if(result.value == MENU_ID_BLUETOOTH_CB) {
            response = DISPLAY_ACTION_TOGGLE_BLUETOOTH;
        }
        else if(result.value == MENU_ID_PROFIL_CB) {
            response = DISPLAY_ACTION_TOGGLE_ACTIVE_PROFILE;
        }
        else if(result.value == MENU_ID_RESET_KM) {
            response = DISPLAY_ACTION_RESET_ODO;
        }
        else if(result.value == MENU_ID_RESET_WH) {
            response = DISPLAY_ACTION_RESET_MAH;
        }
        else if(result.value == MENU_ID_TURN_OFF) {
            response = DISPLAY_ACTION_SHUTDOWN;
        }

        else {
           // currentView = &mainView;
        }

        currentView->activate();
    } else if (result.result == VIEW_RESULT_CHECKBOX_CHECKED) {
        if (result.value == MENU_ID_EM_BRAKE_CB) {
            response = DISPLAY_ACTION_DISABLE_BRAKE;
        }
        else if (result.value == MENU_ID_EM_PEDAL_CB) {
            response = DISPLAY_ACTION_DISABLE_PAS;
        }
        else if (result.value == MENU_ID_EM_SPEED_CB) {
            response = DISPLAY_ACTION_DISABLE_WHEEL;
        }
        else if (result.value == MENU_ID_EM_SPEEDCTRL_CB) {
            response = DISPLAY_ACTION_DISABLE_THROTTLE;
        }


        //! Checkbox toggled
    } else if (result.result == VIEW_RESULT_CHECKBOX_UNCHECKED) {

        if (result.value == MENU_ID_EM_BRAKE_CB) {
            response = DISPLAY_ACTION_ENABLE_BRAKE;
        }
        else if (result.value == MENU_ID_EM_PEDAL_CB) {
            response = DISPLAY_ACTION_ENABLE_PAS;
        }
        else if (result.value == MENU_ID_EM_SPEED_CB) {
            response = DISPLAY_ACTION_ENABLE_WHEEL;
        }
        else if (result.value == MENU_ID_EM_SPEEDCTRL_CB) {
            response = DISPLAY_ACTION_ENABLE_THROTTLE;
        }

    }
    repaint = true;
    return response;
}

void updateDataModel(uint8_t value_id, uint16_t value){
    model.setValue(value_id, value);
}

void updateIconModel(uint8_t icon_id, boolean value){
    model.setIcon(icon_id, value);
}

/*
//! Execute 1 byte command
void displayControlerCommand1(uint8_t cmd, uint8_t value) {
  switch (cmd) {
    case DISP_CMD_STATES:
      if(value & DISP_BIT_STATE_BLUETOOTH) {
        model.showIcon(ICON_ID_BLUETOOTH);
      } else {
        model.clearIcon(ICON_ID_BLUETOOTH);
      }

      if(value & DISP_BIT_STATE_BRAKE) {
        model.showIcon(ICON_ID_BRAKE);
      } else {
        model.clearIcon(ICON_ID_BRAKE);
      }

      if(value & DISP_BIT_STATE_LIGHT) {
        model.showIcon(ICON_ID_LIGHT);
      } else {
        model.clearIcon(ICON_ID_LIGHT);
      }
//  ICON_ID_HEART     = (1 << 3)


      break;
  }
}

void updateValues(uint16_t bat_perc, float_t speed, )

//! Execute 2 byte command
void displayControlerCommand2(uint8_t cmd, uint16_t value) {
  switch (cmd) {
    case DISP_CMD_BATTERY:
      model.setValue(VALUE_ID_BATTERY_VOLTAGE_CURRENT, value);
      break;
    case DISP_CMD_BATTERY_MAX:
      model.setValue(VALUE_ID_BATTERY_VOLTAGE_MAX, value);
      break;
    case DISP_CMD_BATTERY_MIN:
      model.setValue(VALUE_ID_BATTERY_VOLTAGE_MIN, value);
      break;
    case DISP_CMD_SPEED:
      model.setValue(VALUE_ID_SPEED, value);
      break;
    case DISP_CMD_WATTAGE:
      mainView.setWattage(value);
      break;
  }
}*/
