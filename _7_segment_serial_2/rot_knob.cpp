
/*************************************************************
 * Derek Schacht
 * 2015/01/15
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */


#include <Arduino.h>
#include "rot_knob.h"

#define ROT_STATE_A 12
#define ROT_STATE_B 13
#define ROT_STATE_C 14
#define ROT_STATE_D 15

Rotary_Knob::Rotary_Knob()
{
  this->switch_A = 2;
  this->switch_B = 3;

  this->rot_knob_dir   = CLOCKWISE;
  this->rot_knob_state = ROT_STATE_A;

}

Rotary_Knob::Rotary_Knob(int switch_A, int switch_B)
{
  this->switch_A = switch_A;
  this->switch_B = switch_B;
  
  this->rot_knob_dir   = CLOCKWISE;
  this->rot_knob_state = ROT_STATE_A;

}


/* This implements a state machine to track the position of a
 * grey coded rotary encoder. The normal state transition flows
 * from state A to state D wrapping to state A, or the reverse.
 * If somehow the statemachine is out of sync with the rotary
 * encoder the state machine will capture the position of the
 * rotary encoder in 2 steps.
 * This function is designed to be called by an ISR when one of 
 * the two pin states change. However, it can also work in polling
 * mode. In polling mode the maximum rotation speed of the knob
 * is limited by the polling rate.
 */

int Rotary_Knob::read()
{

  int value;
  value = digitalRead(switch_A) | (digitalRead(switch_B) << 1);

  /* State A processing the valid state from here are:
   * - State D
   * - State B
   */
  if (value == 2 && rot_knob_state == ROT_STATE_A)
  {
    rot_knob_dir = CLOCKWISE;
    rot_knob_state = ROT_STATE_D;
  }
  else if (value == 1 && rot_knob_state == ROT_STATE_A)
  {
    rot_knob_dir = ANTICLOCKWISE;
    rot_knob_state = ROT_STATE_B;
  }
  
  /* State B processing the valid states from here are:
   * - State A
   * - State C
   */
  else if (value == 0 && rot_knob_state == ROT_STATE_B)
  {
    rot_knob_dir = CLOCKWISE;
    rot_knob_state = ROT_STATE_A;
  }
  else if (value == 3 && rot_knob_state == ROT_STATE_B)
  {
    rot_knob_dir = ANTICLOCKWISE;
    rot_knob_state = ROT_STATE_C;
  }
  
  /* State C processing the valid states from here are:
   * - State B
   * - State D
   */
  else if (value == 1 && rot_knob_state == ROT_STATE_C)
  {
    rot_knob_dir = CLOCKWISE;
    rot_knob_state = ROT_STATE_B;
  }
  else if (value == 2 && rot_knob_state == ROT_STATE_C)
  {
    rot_knob_dir = ANTICLOCKWISE;
    rot_knob_state = ROT_STATE_D;
  }
  
  /* State D processing the valid states from here are:
   * - State C
   * - State A
   */
  else if (value == 3 && rot_knob_state == ROT_STATE_D)
  {
    rot_knob_dir = CLOCKWISE;
    rot_knob_state = ROT_STATE_C;
  }
  else if (value == 0 && rot_knob_state == ROT_STATE_D)
  {
    rot_knob_dir = ANTICLOCKWISE;
    rot_knob_state = ROT_STATE_A;
  }
  
  return rot_knob_dir;
  
}
