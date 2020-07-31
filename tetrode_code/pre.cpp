#include "pre.h"
#include <EEPROM.h>

void read_preset(Preset *p, unsigned int addr){
  p->fwd_count = 0;
  p->rev_count = 0;
  p->spd_count = 0;
  for(int i = 0; i < sizeof(p->fwd_count); i++){
    p->fwd_count <<= 8;
    p->fwd_count += EEPROM.read(addr+i);
  }
  for(int i = 0; i < sizeof(p->rev_count); i++){
    p->rev_count <<= 8;
    p->rev_count += EEPROM.read(addr+i+sizeof(p->fwd_count));
  }
  for(int i = 0; i < sizeof(p->spd_count); i++){
    p->spd_count <<= 8;
    p->spd_count += EEPROM.read(addr+i+sizeof(p->fwd_count)+sizeof(p->rev_count));
  }
}

void write_preset(Preset *p, unsigned int addr){
  for(int i = 0; i < sizeof(p->fwd_count); i++){
    EEPROM.write(addr+i, (byte)((p->fwd_count >> (8 * (sizeof(p->fwd_count)-i-1))) & 0xFF));
  }
  for(int i = 0; i < sizeof(p->rev_count); i++){
    EEPROM.write(addr+i+sizeof(p->fwd_count), (byte)((p->rev_count >> (8 * (sizeof(p->rev_count)-i-1))) & 0xFF));
  }
  for(int i = 0; i < sizeof(p->fwd_count); i++){
    EEPROM.write(addr+i+sizeof(p->fwd_count)+sizeof(p->rev_count), (byte)((p->spd_count >> (8 * (sizeof(p->spd_count)-i-1))) & 0xFF));
  }
}
