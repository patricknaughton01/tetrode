#include "button.h"

const unsigned int button1_pin = 0;
const unsigned int button2_pin = 1;
const unsigned int button3_pin = 8;

enum State{
  FWD,
  REV,
  STOP
};

Button b1(button1_pin);
Button b2(button2_pin);
Button b3(button3_pin);

void setup() {
  // put your setup code here, to run once:
  b1.init();
  b2.init();
  b3.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  b1.reg_next_state();
  b2.reg_next_state();
  b3.reg_next_state();
}
