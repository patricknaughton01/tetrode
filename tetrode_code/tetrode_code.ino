#include "button.h"
#include "fservo.h"
#include <Servo.h>

#define HOLD_THRESH 300

const unsigned int button1_pin = 0;
const unsigned int button2_pin = 1;
const unsigned int button3_pin = 8;

const unsigned int fwd_pot_pin = 0;
const unsigned int rev_pot_pin = 0;
const unsigned int speed_pot_pin = 0;

const unsigned int servo_drive_pin = 3;
const unsigned int servo_feedback_pin = 4;
Servo servo;

int fwd_count = 1;
int rev_count = 1;
unsigned int speed_count = 200;

enum State{
  FWD = 0,
  PAUSE_F1 = 1,
  PAUSE_F2 = 2,
  BUZZ = 3,
  REV = 4,
  PAUSE_R1 = 5,
  PAUSE_R2 = 6,
  PAUSE_STOP = 7,
  STOP = 8
};

State state;

enum EditState{
  PRESET,
  EDIT
};

EditState edit_state;

Button b1(button1_pin);
Button b2(button2_pin);
Button b3(button3_pin);

Fservo fs(servo, servo_drive_pin, servo_feedback_pin);

unsigned long millis_timer;

void setup() {
  // put your setup code here, to run once:
  b1.init();
  b2.init();
  b3.init();
  fs.init();
  fs.stop();
  state = STOP;
  fs.reg_next_pos();
  fs.set_start();
  edit_state = PRESET;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Sense all the inputs
  b1.reg_next_state();
  b2.reg_next_state();
  b3.reg_next_state();
  fs.reg_next_pos();

  switch(state){
    case STOP:
      if(b1.released()){
        state = FWD;
        fs.set_start();
        fs.go(NEUTRAL_US + speed_count);
      }
      break;
    case FWD:
      if(b1.down()){
        fs.stop();
        millis_timer = millis();
        state = PAUSE_F1;
      }else if(fs.get_rotations() >= fwd_count){
        fs.stop();
        state = BUZZ;
      }
      break;
    case PAUSE_F1:
      if(b1.up()){
        state = PAUSE_F2;
      }else if(b1.down() && (millis() - millis_timer > HOLD_THRESH)){
        fs.stop();
        fs.set_start();
        state = PAUSE_STOP;
      }
      break;
    case PAUSE_F2:
      if(b1.released()){
        fs.go(NEUTRAL_US + speed_count);
        state = FWD;
      }
      break;
    case BUZZ:
      fs.stop();
      delay(500);
      fs.reset_flags();
      fs.go(NEUTRAL_US - speed_count);
      state = REV;
      break;
    case REV:
      if(b1.down()){
        fs.stop();
        millis_timer = millis();
        state = PAUSE_R1;
      }else if(fs.get_rotations() <= ((fwd_count - rev_count) - 1)){
        fs.stop();
        state = STOP;
      }
      break;
    case PAUSE_R1:
      if(b1.up()){
        state = PAUSE_R2;
      }else if(b1.down() && (millis() - millis_timer > HOLD_THRESH)){
        fs.stop();
        fs.set_start();
        state = PAUSE_STOP;
      }
      break;
    case PAUSE_R2:
      if(b1.released()){
        fs.go(NEUTRAL_US - speed_count);
        state = FWD;
      }
      break;
    case PAUSE_STOP:
      if(b1.up()){
        state = STOP;
      }
      break;
    default:
      break;
  }
  Serial.print(fs.above_thresh);
  Serial.print(",");
  Serial.print(fs.below_thresh);
  Serial.print(",");
  Serial.print(fs.center_thresh);
  Serial.print(",");
  Serial.print(state);
  Serial.print(",");
  Serial.print(((fwd_count - rev_count) - 1));
  Serial.print(",");
  Serial.println(fs.get_rotations());
}
