#include "focus_display.h"
#include "focus_motor.h"
#include "focus_knob.h"
#include "focus_setup.h"
#include "focus_modes.h"

#define SRCLK 5
#define RCLK  6
#define SER   7

#define STEP  8
#define DIR   9
#define MS1  10
#define MS2  11

Focus_Display display;
Focus_Motor motor;
Focus_Knob knob;
Focus_Setup focusSetup;

char mode = MODE_NORMAL;

void setup()
{ 
  pinMode(13, OUTPUT);

  display = Focus_Display(SRCLK, SER, RCLK);
  motor = Focus_Motor(STEP, DIR, MS1, MS2);
  if (knob.getButtonState() == 0)
  {
    knob.setInterface(&motor);
    mode = MODE_NORMAL;
  }
  else
  {
    knob.setInterface(&focusSetup);
    mode = MODE_SETUP;
  }
  
}

void loop()
{
  switch(mode)
  {
  case MODE_NORMAL:
    motor.processMotor();
    display.setDisplayValue(motor.getPosition());
    break;
  case MODE_SETUP:
    display.setDisplayValue(focusSetup.getDisplay());
    break;
  };

  delay(5);
}


