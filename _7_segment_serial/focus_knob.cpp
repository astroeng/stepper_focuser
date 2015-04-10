#include <Arduino.h>
#include "focus_knob.h"

static unsigned char rot_knob_dir;
static unsigned char rot_knob_state;
static Focus_Motor* motor;

static unsigned char setupState;

/* This implements a state machine to track the position of a
 * grey coded rotary encoder. The normal state transition flows
 * from state A to state D wrapping to state A, or the reverse.
 * If somehow the statemachine is out of sync with the rotary
 * encoder the state machine will capture the position of the
 * rotary encoder in 2 steps.
 * This function is designed to be called as an ISR when one of 
 * the two pin states change. However, it can also work in polling
 * mode. In polling mode the maximum rotation speed of the knob
 * is limited by the polling rate.
 */
void rot_knob()
{
  int value;
  value = digitalRead(STATEA) | (digitalRead(STATEB) << 1);
  
  /* State A processing the valid state from here are:
   * - State D
   * - State B
   */
  if (value == 2 && rot_knob_state == ROT_STATE_A)
  {
    rot_knob_dir = MOVEIN;
    rot_knob_state = ROT_STATE_D;
  }
  else if (value == 1 && rot_knob_state == ROT_STATE_A)
  {
    rot_knob_dir = MOVEOUT;
    rot_knob_state = ROT_STATE_B;
  }
  
  /* State B processing the valid states from here are:
   * - State A
   * - State C
   */
  else if (value == 0 && rot_knob_state == ROT_STATE_B)
  {
    rot_knob_dir = MOVEIN;
    rot_knob_state = ROT_STATE_A;
  }
  else if (value == 3 && rot_knob_state == ROT_STATE_B)
  {
    rot_knob_dir = MOVEOUT;
    rot_knob_state = ROT_STATE_C;
  }
  
  /* State C processing the valid states from here are:
   * - State B
   * - State D
   */
  else if (value == 1 && rot_knob_state == ROT_STATE_C)
  {
    rot_knob_dir = MOVEIN;
    rot_knob_state = ROT_STATE_B;
  }
  else if (value == 2 && rot_knob_state == ROT_STATE_C)
  {
    rot_knob_dir = MOVEOUT;
    rot_knob_state = ROT_STATE_D;
  }
  
  /* State D processing the valid states from here are:
   * - State C
   * - State A
   */
  else if (value == 3 && rot_knob_state == ROT_STATE_D)
  {
    rot_knob_dir = MOVEIN;
    rot_knob_state = ROT_STATE_C;
  }
  else if (value == 0 && rot_knob_state == ROT_STATE_D)
  {
    rot_knob_dir = MOVEOUT;
    rot_knob_state = ROT_STATE_A;
  }

  motor->updatePosition(rot_knob_dir);
}


void setupKnob()
{

}

Focus_Knob::Focus_Knob()
{
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(STATEA, INPUT_PULLUP);
  pinMode(STATEB, INPUT_PULLUP);

  rot_knob_dir   = MOVEIN;
  rot_knob_state = ROT_STATE_A;
}

void Focus_Knob::setInterface(Focus_Motor* motor_in)
{
  attachInterrupt(0, rot_knob, CHANGE);
  attachInterrupt(1, rot_knob, CHANGE);  
  
  motor = motor_in;
}

void Focus_Knob::setInterface(Focus_Setup* setup_in)
{

}

unsigned char Focus_Knob::getButtonState()
{
  return digitalRead(BUTTON);
}
