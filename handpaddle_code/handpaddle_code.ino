
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
#include "config.h"
#include "focus_motor.h"

ShiftSegmentDisplay display;
Focus_Motor motor;
RotaryKnobDecoder knob;

unsigned long motorUpdateTime;
unsigned long buttonCheckTime;
unsigned long debugPrintTime;

int decimalPlace;

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
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  /* These buttons are normally open they are basically little micro switches. */
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);  

  /* Setup the classes that will be used to control the more complex devices. */
  display = ShiftSegmentDisplay(SRCLK, SER, RCLK);
  motor   = Focus_Motor(STEP, DIR, MS1, MS2);
  knob    = RotaryKnobDecoder(STATEA, STATEB);
  
  /* The Rotary Knob works best when attached to interrupts. */
  attachInterrupt(0, rot_knob, CHANGE);
  attachInterrupt(1, rot_knob, CHANGE);
  
  display.setDigits(DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4,
                    DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8);
                    
  display.setSegments(SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D,
                      SEGMENT_E, SEGMENT_F, SEGMENT_G, SEGMENT_DP);

  motorUpdateTime = millis();
  debugPrintTime = millis();
  decimalPlace = 0;
}

void loop()
{
  /* The Arduino framework gets pretty cranky if we perfrom lengthy delays.
   * To avoid delays but still update at regular intervals we setup a very
   * crude schedule using the millis function and if statements.
   */
  unsigned long time = millis();
  
  /* The motor processing should be done every 10ms... This is pretty
   * arbitrary and was selected by experimentation.
   */
  
  if ((motorUpdateTime + 10) < time)
  {
    motor.processMotor();
    display.setDisplayValue(motor.getOrder(), decimalPlace);
    display.setDisplayValue2(motor.getPosition(), 0);
    motorUpdateTime = time;
  }
  
  /* The sample rate for the non-interrupt inputs is 125ms */
  
  if ((buttonCheckTime + 125) < time)
  {
    digitalWrite(RED_LED,LOW);
    digitalWrite(GREEN_LED,LOW);
    
    if (digitalRead(BUTTON_LEFT) == LOW)
    {
      Serial.println("Button Left");
      digitalWrite(GREEN_LED,HIGH);
      decimalPlace++;
      if (decimalPlace > 3)
        decimalPlace = 0;
    }
    if (digitalRead(BUTTON_RIGHT) == LOW)
    {
      Serial.println("Button Right");
      digitalWrite(RED_LED,HIGH);
      decimalPlace--;
      if (decimalPlace < 0)
        decimalPlace = 3;
    }
    if (digitalRead(BUTTON) == LOW)
    {
      Serial.println("Button");
      digitalWrite(GREEN_LED,HIGH);
      digitalWrite(RED_LED,HIGH);
      motor.stopAtCurrentPosition();
    }
    
    buttonCheckTime = time;
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
  
  if (knob_direction == CLOCKWISE)
  {
    motor.updatePosition(MOVEIN);
  }
  else if (knob_direction == ANTICLOCKWISE)
  {
    motor.updatePosition(MOVEOUT);
  }

}
