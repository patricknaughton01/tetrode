#ifndef FSERVO_H
#define FSERVO_H

#include "Arduino.h"
#include <Servo.h>

#define NEUTRAL_US 1500
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
    void reset_flags();
    void go(int speed);
    void stop();
    int get_rotations();
    bool below_thresh;
    bool above_thresh;
    bool center_thresh;
  private:
    const int pin;
    const int fpin;
    Servo servo;
    int rotations;
    int direction;
    int last_pos;
    int this_pos;
    int start_pos;
    
    int bound(int x, int high, int low);
    int center(int x, int center, int high, int low);
};

#endif // FSERVO_H
