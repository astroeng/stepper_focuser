/*************************************************************
 * Derek Schacht
 * 2015/01/13
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 * Initial Code based on a function I wrote in college for a microblaze CPU.
 */

#define ANTICLOCKWISE 16
#define CLOCKWISE 17

class Rotary_Knob
{
public:
  Rotary_Knob();
  Rotary_Knob(int switch_A, int switch_B);
  
  int read();
  
private:
  int switch_A;
  int switch_B;

  unsigned int rot_knob_dir;
  unsigned int rot_knob_state;
};
