#include "fservo.h"
#include <Servo.h>

#define MAX(x, y) ((x)>(y) ? (x) : (y))
#define MIN(x, y) ((x)<(y) ? (x) : (y))
#define ABS(x) ((x)>0 ? (x) : (-(x)))

Fservo::Fservo(Servo s, int pin, int fpin):servo(s), pin(pin), fpin(fpin), 
    last_pos(0), this_pos(0), start_pos(0), rotations(0), direction(0),
    below_thresh(false), above_thresh(false), center_thresh(true){};

void Fservo::init(){
    servo.attach(pin);
}

void Fservo::reg_next_pos(){
    last_pos = this_pos;
    this_pos = pulseIn(fpin, HIGH);
    int clp = center(last_pos, start_pos, PULSE_MAX, PULSE_MIN);
    int ctp = center(this_pos, start_pos, PULSE_MAX, PULSE_MIN);
    int mid = (PULSE_MAX - PULSE_MIN) / 2;
    if(ctp > PULSE_THRESH && ctp < mid){
      above_thresh = true;
      if(below_thresh && center_thresh){
        rotations--;
      }
      below_thresh = false;
      center_thresh = false;
    }else if((ctp < (PULSE_MAX - PULSE_THRESH)) && ctp > mid){
      below_thresh = true;
      if(above_thresh && center_thresh){
        rotations++;
      }
      above_thresh = false;
      center_thresh = false;
    }else if(ctp < PULSE_THRESH || ctp > (PULSE_MAX-PULSE_THRESH)){
      center_thresh = true;
    }
}

void Fservo::set_start(){
  reset_flags();
  start_pos = this_pos;
  rotations = 0;
}

void Fservo::reset_flags(){
  above_thresh = false;
  below_thresh = false;
  center_thresh = true;
}

void Fservo::go(int speed){
    speed = bound(speed, FWD_US, REV_US);
    direction = speed > NEUTRAL_US ? FWD_D : REV_D;
    servo.writeMicroseconds(speed);
}

void Fservo::stop(){
    direction = STOP_D;
    servo.writeMicroseconds(NEUTRAL_US);
}

int Fservo::get_rotations(){
  return rotations;
}

int Fservo::bound(int x, int high, int low){
    return MAX(MIN(x, high), low);
}

int Fservo::center(int x, int center, int high, int low){
  int diff = high - low;
  return ( ((x - center) + diff) % diff ) + low;
}
