
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
#include "focus_motor.h"
#include "rot_knob.h"

#define STATEA    2
#define STATEB    3
#define BUTTON    4
#define RED_LED   5
#define GREEN_LED 6

#define BUTTON_LEFT  7
#define BUTTON_RIGHT 8

#define SERVO_PWM 9

#define SRCLK A1
#define RCLK  10
#define SER   11

#define RJ7 12
#define RJ6 13
#define RJ5 A2
#define RJ4 A4

#define DIR  A5
#define STEP A3
#define MS1  A6
#define MS2  A7

#define SEGMENT_A  0x0004
#define SEGMENT_B  0x0020
#define SEGMENT_C  0x0001
#define SEGMENT_D  0x2000
#define SEGMENT_E  0x1000
#define SEGMENT_F  0x0100
#define SEGMENT_G  0x0010
#define SEGMENT_DP 0x0400

#define DIGIT_1 0x0080 // Left most bottom display (display 2)
#define DIGIT_2 0x0040
#define DIGIT_3 0x0008
#define DIGIT_4 0x0002
#define DIGIT_5 0x8000 // Left most top display (display 1)
#define DIGIT_6 0x4000
#define DIGIT_7 0x0800
#define DIGIT_8 0x0200

ShiftSegmentDisplay display;
Focus_Motor motor;
Rotary_Knob knob;

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
  knob    = Rotary_Knob(STATEA, STATEB);
  
  /* The Rotary Knob works best when attached to interrupts. */
  attachInterrupt(0, rot_knob, CHANGE);
  attachInterrupt(1, rot_knob, CHANGE);
  
  motorUpdateTime = millis();
  debugPrintTime = millis();
  decimalPlace = 0;
  
  display.setDigits(DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4,
                    DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8);
                    
  display.setSegments(SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D,
                      SEGMENT_E, SEGMENT_F, SEGMENT_G, SEGMENT_DP);
  
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
