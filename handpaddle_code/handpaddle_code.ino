
/*************************************************************
 * Derek Schacht
 * 2014/04/23
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */

/* The focuser at startup should be racked all the way in since 
 * this code does not allow negative displacement. If negative
 * displacement is desired focus motor will need to be updated
 * and potentially focus display to allow signed values.
 *
 * At startup the motor is enabled and will not ever be put into
 * the sleep state. This prevents the focuser from drifting since
 * the stepper motor effectively locks the adjustment.
 *
 * Additional Planned features:
 *   Use the button on the rotary knob to stop focuser movement.
 *   Use the Left and Right buttons to recall focus positions.
 *   Add a temperature sensor and perform temperature based adjustments.
 */

#include <shift_segment_display.h>
#include <rotary_knob_decoder.h>
#include <primitive_scheduler.h>
#include "config.h"
#include "focus_motor.h"

ShiftSegmentDisplay display;
Focus_Motor         motor;
RotaryKnobDecoder   knob;
PrimitiveScheduler  schedule;


void setup()
{ 
  Serial.begin(9600);
  
  /* The classic user LED on an arduino. */
  pinMode(13, OUTPUT);
  
  /* The Rotary Knob is intended to be a red/green knob from SparkFun. */
  /* Rotary Knob Inputs */
  pinMode(STATEA, INPUT_PULLUP); /* INPUT_PULLUP needed for the rotary knob */
  pinMode(STATEB, INPUT_PULLUP); 
  pinMode(BUTTON, INPUT_PULLUP);

  /* Rotary Knob Outputs */
  pinMode(RED_LED,   OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  /* These buttons are normally open they are basically little micro switches. */
  pinMode(BUTTON_LEFT,  INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);  

  /* Setup the classes that will be used to control the more complex devices. */
  display = ShiftSegmentDisplay(SRCLK, SER, RCLK);
  motor   = Focus_Motor(STEP, DIR, MS1, MS2);
  knob    = RotaryKnobDecoder(STATEA, STATEB);
  
  display.setDigits(DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4,
                    DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8);
                    
  display.setSegments(SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D,
                      SEGMENT_E, SEGMENT_F, SEGMENT_G, SEGMENT_DP);

  /* The Rotary Knob works best when attached to interrupts. */
  attachInterrupt(0, rot_knob, CHANGE); /* Arduino Pin #2 */
  attachInterrupt(1, rot_knob, CHANGE); /* Arduino Pin #3 */

  schedule.addTask(motor_task,    10);
  schedule.addTask(display_task,  25);
  schedule.addTask(button_task,  125);
}

void loop()
{
  schedule.run();
}

/* Task to update the motor position. */
void motor_task()
{
  motor.processMotor();
}

/* Task to update the displays. */
void display_task()
{
  display.setDisplayValue(motor.getOrder());
  display.setDisplayValue2(motor.getPosition());
}

/* Task to read the bottons and then perform actions. */
void button_task()
{
  digitalWrite(RED_LED,LOW);
  digitalWrite(GREEN_LED,LOW);
  
  if (digitalRead(BUTTON_LEFT) == LOW)
  {
    digitalWrite(GREEN_LED,HIGH);
  }
  if (digitalRead(BUTTON_RIGHT) == LOW)
  {
    digitalWrite(RED_LED,HIGH);
  }
  if (digitalRead(BUTTON) == LOW)
  {
    digitalWrite(GREEN_LED,HIGH);
    digitalWrite(RED_LED,HIGH);
  }  
}

/* ISR for the rotary knob */

void rot_knob()
{
  /* Since the knob direction is statebased the function should only
   * be called once per event. There a filter caps on the inputs for
   * this interrupt.
   */
  
  int knob_direction = knob.read();
  motor.setSpeed(knob.getSpeed());
  
  if (knob_direction == CLOCKWISE)
  {
    motor.updateOrder(MOVEIN);
  }
  else if (knob_direction == ANTICLOCKWISE)
  {
    motor.updateOrder(MOVEOUT);
  }

}
