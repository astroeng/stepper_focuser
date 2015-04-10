#ifndef FOCUS_KNOB_H
#define FOCUS_KNOB_H


#include "focus_motor.h"
#include "focus_setup.h"

#define STATEA 2
#define STATEB 3
#define BUTTON 4

#define ROT_STATE_A 12
#define ROT_STATE_B 13
#define ROT_STATE_C 14
#define ROT_STATE_D 15

class Focus_Knob
{
  public:
    Focus_Knob();
    unsigned char getButtonState();
    void setInterface(Focus_Motor* motor_in);
    void setInterface(Focus_Setup* setup_in);
  private:
};

#endif
