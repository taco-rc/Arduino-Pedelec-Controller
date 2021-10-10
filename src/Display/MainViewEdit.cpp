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

#include "MainViewEdit.h"
#include "MenuEntries.h"
#include "Components.h"


/**
 * Custmize main view
 */

//! Constructor
MainViewEdit::MainViewEdit(Components* components)
        : MainView(components),
          m_selectedId(0),
          m_lastSelectedId(-1),
          m_selectionPosition(0)
{
}

//! Destructor
MainViewEdit::~MainViewEdit() {
}

//! UP / DOWN Key
void MainViewEdit::movePosition(int8_t diff) {
  m_lastSelectedId = m_selectedId;

  m_selectedId += diff;
  if (m_selectedId < 0) {
    m_selectedId = 0;
  }

  if (m_selectedId - 1 >= MAX_COMP_ACTIVE) {
    m_selectedId = MAX_COMP_ACTIVE - 1;
  }

  while (m_components->get(m_selectedId) == NULL && m_selectedId > 0) {
    m_selectedId--;
  }
  drawSelection();
}

//! draw the selection mark
void MainViewEdit::drawSelection() {
  if (!m_active) {
    return;
  }

  if (m_lastSelectedId != -1) {
    BaseComponent* component = m_components->get(m_lastSelectedId);
    if (component != NULL) {
      lcd.fillRect(0, m_components->getY(m_lastSelectedId) - 1, 240, component->getHeight() + 2, ILI9341_BLACK);
      component->setY(m_components->getY(m_lastSelectedId));
      component->draw(true);
    }
  }
  m_lastSelectedId = -1;

  BaseComponent* component = m_components->get(m_selectedId);
  if (component == NULL) {
    return;
  }

  for (uint8_t i = 0; i <= 1; i++) {
    lcd.drawRect(i, m_components->getY(m_selectedId) + i, 240 - 2 * i, component->getHeight() + 1 - 2 * i, ILI9341_MAGENTA);
  }
}

//! Update full display
void MainViewEdit::updateDisplay(bool repaint) {
  if (!m_active) {
    return;
  }

  MainView::updateDisplay(repaint);

  drawSelection();
}

//! remove the selected component
void MainViewEdit::removeSelected() {
  m_components->remove(m_selectedId);
  updateDisplay(true);
}

//! Key (OK) pressed
ViewResult MainViewEdit::keyPressed() {
  ViewResult result;
  result.result = VIEW_RESULT_NOTHING;
  result.value = 0;

  if (m_selectedId != -1) {
    BaseComponent* component = m_components->get(m_selectedId);
    if (component != NULL) {
      result.result = VIEW_RESULT_MENU;
      result.value = MENU_ID_COMPONENT;
    }
  }

  return result;
}
