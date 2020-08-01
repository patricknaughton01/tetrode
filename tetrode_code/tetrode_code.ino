#include "button.h"
#include "fservo.h"
#include "pre.h"
#include <Servo.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

#define HOLD_THRESH 300
#define MAX_CNT 120
#define MIN_CNT 0
#define MAX_SPD 500
#define MIN_SPD 50
#define NUM_PRE 5

const unsigned int button1_pin = 0;
const unsigned int button2_pin = 1;
const unsigned int button3_pin = 8;

const unsigned int fwd_pot_pin = A0;
const unsigned int rev_pot_pin = A1;
const unsigned int speed_pot_pin = A2;

const unsigned int servo_drive_pin = 3;
const unsigned int servo_feedback_pin = 2;
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
unsigned int preset_counter = 0;
Preset preset;

Button b1(button1_pin);
Button b2(button2_pin);
Button b3(button3_pin);

Fservo fs(servo, servo_drive_pin, servo_feedback_pin);

// LCD pins: RS, E, D4, D5, D6, D7
unsigned int lcd_p[6] = {12, 11, 5, 4, 7, 6};
LiquidCrystal lcd(lcd_p[0], lcd_p[1], lcd_p[2], lcd_p[3], lcd_p[4], lcd_p[5]);

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
  lcd.begin(16, 2);
  read_preset(&preset, 0);
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
        fs.go(NEUTRAL_US + preset.spd_count);
      }
      break;
    case FWD:
      if(b1.down()){
        fs.stop();
        millis_timer = millis();
        state = PAUSE_F1;
      }else if(fs.get_rotations() >= preset.fwd_count){
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
        fs.go(NEUTRAL_US + preset.spd_count);
        state = FWD;
      }
      break;
    case BUZZ:
      fs.stop();
      delay(500);
      fs.reset_flags();
      fs.go(NEUTRAL_US - preset.spd_count);
      state = REV;
      break;
    case REV:
      if(b1.down()){
        fs.stop();
        millis_timer = millis();
        state = PAUSE_R1;
      }else if(fs.get_rotations() <= ((preset.fwd_count - preset.rev_count) - 1)){
        fs.stop();
        fs.set_start();
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
        fs.go(NEUTRAL_US - preset.spd_count);
        state = FWD;
      }
      break;
    case PAUSE_STOP:
      if(b1.up()){
        fs.set_start();
        state = STOP;
      }
      break;
    default:
      break;
  }
  
  lcd.setCursor(0, 0);
  lcd.print("FWD REV CT  SP ");
  switch(edit_state){
    case PRESET:
      lcd.print(preset_counter);
      break;
    case EDIT:
      lcd.print("E");
      break;
    default:
      break;
  }
  lcd.setCursor(0, 1);
  lcd.print(pad_num(preset.fwd_count, 3));
  lcd.print(" ");
  lcd.print(pad_num(preset.rev_count, 3));
  lcd.print(" ");
  lcd.print(pad_num(fs.get_rotations(), 3));
  lcd.print(" ");
  lcd.print(pad_num(preset.spd_count, 3));
  lcd.print(" ");
  
  switch(edit_state){
    case PRESET:
      if(b3.released()){
        edit_state = EDIT;
      }else if(b2.released()){
        preset_counter = (preset_counter + 1) % NUM_PRE;
        read_preset(&preset, preset_counter * sizeof(Preset));
      }
      break;
    case EDIT:
      preset.fwd_count = ((MAX_CNT - MIN_CNT) * (analogRead(fwd_pot_pin)/1022.0)) + MIN_CNT;
      preset.rev_count = ((MAX_CNT - MIN_CNT) * (analogRead(rev_pot_pin)/1022.0)) + MIN_CNT;
      preset.spd_count = ((MAX_SPD - MIN_SPD) * (analogRead(speed_pot_pin)/1022.0)) + MIN_SPD;
      if(b3.released()){
        write_preset(&preset, preset_counter * sizeof(Preset));
        edit_state = PRESET;
      }
      break;
    default:
      break;
  }
}

String pad_num(int x, int width){
  String num = String(x);
  while(num.length() < width){
    num = String(' ') + num;
  }
  return num;
}
