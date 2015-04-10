
/*************************************************************
 * Derek Schacht
 * 2014/04/23
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use consent.
 *************************************************************
 */


#include "focus_display.h"
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

Focus_Display display;
Focus_Motor motor;
Rotary_Knob knob;

unsigned long motorUpdateTime;
unsigned long buttonCheckTime;
unsigned long debugPrintTime;

int decimalPlace;

void setup()
{ 
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
  pinMode(STATEA, INPUT_PULLUP);
  pinMode(STATEB, INPUT_PULLUP);

  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON, INPUT_PULLUP);
  
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  display = Focus_Display(SRCLK, SER, RCLK);
  motor   = Focus_Motor(STEP, DIR, MS1, MS2);
  knob    = Rotary_Knob(STATEA, STATEB);
  
  attachInterrupt(0, rot_knob, CHANGE);
  attachInterrupt(1, rot_knob, CHANGE);
  
  motorUpdateTime = millis();
  debugPrintTime = millis();
  decimalPlace = 0;
}

void loop()
{
  unsigned long time = millis();
  
  if ((motorUpdateTime + 10) < time)
  {
    motor.processMotor();
    display.setDisplayValue(motor.getOrder(), decimalPlace);
    display.setDisplayValue2(motor.getPosition(), 0);
    motorUpdateTime = time;
  }
  
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
    }
    
    buttonCheckTime = time;
  }
}

/* ISR for the rotary knob */

void rot_knob()
{
  /* Since the knob direction is statebased the function should only
   * be called once per event.
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
