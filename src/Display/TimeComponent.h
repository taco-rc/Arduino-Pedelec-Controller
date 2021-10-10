//
// Created by daniel on 03/08/16.
//

#ifndef PEDELEC_CONTROLLER_TIMECOMPONENT_H
#define PEDELEC_CONTROLLER_TIMECOMPONENT_H

#include "TextComponent.h"

class TimeComponent: public TextComponent {
    public:
  //! Constructor
  TimeComponent(String text, ValueId display_val):
          TextComponent(text, display_val, 0) {};

  //! Destructor
  virtual void draw(bool repaint);

};

#endif //PEDELEC_CONTROLLER_TIMECOMPONENT_H
