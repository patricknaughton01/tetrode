#ifndef FSERVO_H
#define FSERVO_H

#include "Arduino.h"
#include <Servo.h>

#define NEUTRAL_US 1550
#define REV_US 1000
#define FWD_US 2000
#define FWD_D 1
#define REV_D -1
#define STOP_D 0
#define PULSE_MIN 0
#define PULSE_MAX 1100
#define PULSE_THRESH 50

class Fservo{
  public: 
    Fservo(Servo s, int pin, int fpin);
    void init();
    void reg_next_pos();
    void set_start();
    void go(int speed);
    void stop();
    float get_rotations();
  private:
    const int pin;
    const int fpin;
    Servo servo;
    int rotations;
    float rot_frac;
    int direction;
    int last_pos;
    int this_pos;
    int start_pos;
    
    int bound(int x, int high, int low);
    int center(int x, int center, int high, int low);
};

#endif // FSERVO_H
