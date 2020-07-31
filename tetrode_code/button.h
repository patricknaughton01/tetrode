#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

class Button{
  public: 
    Button(int pin);
    void init();
    void reg_next_state();
    int get_last_state();
    int get_this_state();
    bool pressed();
    bool released();
    bool up();
    bool down();
  private:
    const int pin;
    int last_state;
    int this_state;
};

#endif // BUTTON_H
