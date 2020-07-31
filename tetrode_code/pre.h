#ifndef PRE_H
#define PRE_H

#include "Arduino.h"

struct Preset{
  int fwd_count;
  int rev_count;
  unsigned int spd_count;
};

void read_preset(Preset *p, unsigned int addr);
void write_preset(Preset *p, unsigned int addr);

#endif // PRE_H
