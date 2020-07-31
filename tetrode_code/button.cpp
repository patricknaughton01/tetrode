#include "button.h"

Button::Button(int pin):pin(pin), last_state(HIGH), this_state(HIGH){}

void Button::init(){
  pinMode(pin, INPUT);
}

void Button::reg_next_state(){
  last_state = this_state;
  this_state = digitalRead(pin);
}

int Button::get_last_state(){
  return last_state;
}

int Button::get_this_state(){
  return this_state;
}

bool Button::pressed(){
  return ((last_state == HIGH) && (this_state == LOW));
}

bool Button::released(){
  return ((last_state == LOW) && (this_state == HIGH));
}

bool Button::up(){
  return this_state == HIGH;
}

bool Button::down(){
  return this_state == LOW;
}
