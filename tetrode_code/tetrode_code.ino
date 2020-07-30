#include "button.h"
#include "fservo.h"
#include <Servo.h>

const unsigned int button1_pin = 0;
const unsigned int button2_pin = 1;
const unsigned int button3_pin = 8;

const unsigned int fwd_pot_pin = 0;
const unsigned int rev_pot_pin = 0;
const unsigned int speed_pot_pin = 0;

const unsigned int servo_drive_pin = 3;
const unsigned int servo_feedback_pin = 4;
Servo servo;

unsigned int fwd_count = 0;
unsigned int rev_count = 0;
unsigned int speed_count = 0;

enum State{
  FWD,
  REV,
  STOP
};

Button b1(button1_pin);
Button b2(button2_pin);
Button b3(button3_pin);

Fservo fs(servo, servo_drive_pin, servo_feedback_pin);

long start_millis;
bool first;

void setup() {
  // put your setup code here, to run once:
  b1.init();
  b2.init();
  b3.init();
  fs.init();
  start_millis = millis();
  first = true;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  b1.reg_next_state();
  b2.reg_next_state();
  b3.reg_next_state();
  fs.reg_next_pos();
  if(first){
    fs.go(1200);
    first = false;
  }
  Serial.println(fs.get_rotations());
}
