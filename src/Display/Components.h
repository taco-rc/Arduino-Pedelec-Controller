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
#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "BaseComponent.h"
#include "TextComponent.h"
#include "TimeComponent.h"
#include "DiagramComponent.h"
#include "IconComponent.h"
#include "SeparatorComponent.h"

/**
 * List with the customized components
 */

//! Max. 10 Components on the screen (should be enough, there isn't more space)
#define MAX_COMP_ACTIVE 10
#define COMP_ID_NONE -1

typedef struct _View {
    int8_t active_components_ids[MAX_COMP_ACTIVE];
    String diagram_string;
    ValueId diagram_val;
    int16_t diagram_precision;
    float_t diagram_min;
    float_t diagram_max;
} View;


enum {
    COMP_ID_SEP = 0,
    COMP_ID_ICON,
    COMP_ID_DIAG,
    COMP_ID_BAT_MAH,
    COMP_ID_BAT_VOLT,
    COMP_ID_ODO_TOTAL,
    COMP_ID_REMAINING_KM,
    COMP_ID_TIME_DRIVEN,
    COMP_ID_VESC_TEMP,
    COMP_ID_MOTOR_CURRENT,
    COMP_ID_MOTOR_RPM,
    COMP_ID_THROTTLE_POTI,
    COMP_ID_THROTTLE_WRITE,
    COMP_ID_SUPPORT_POTI,
    COMP_ID_CADENCE,
    COMP_COUNT};


class Components {
    // Constructor / Destructor
public:
    //! Constructor
    Components();

    //! Destructor
    virtual ~Components();

    // public API
public:
    //! Draw all components
    void draw(bool repaint);

    BaseComponent* g_components[COMP_COUNT];

    //! Return the component at position index
    BaseComponent* get(uint8_t index);
    uint16_t getY(uint8_t index);

    //! remove the element at index, but does not delete it
    void remove(uint8_t index);

    void changeView(int8_t diff);
    //! Activate / Deactivate children
    void deActivateChilren(bool enabled);

private:
    //! Update the Y position of all elements, and remove invisible elements from the list
    void updatePositionAndRemoveInvisible();
    void activateView(uint8_t num);
    // Member
private:
    //! List with the components

//  BaseComponent* m_components[COMP_COUNT];
    int8_t m_active_components_ids[MAX_COMP_ACTIVE];
    uint16_t m_y_top[MAX_COMP_ACTIVE];
    int8_t m_cur_view;
    DiagramComponent* diagramComponent;
};
#endif //COMPONENT_H