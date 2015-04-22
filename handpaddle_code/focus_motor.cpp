
/*************************************************************
 * Derek Schacht
 * 2014/04/23
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */


#include <Arduino.h>
#include "focus_motor.h"

Focus_Motor::Focus_Motor()
{
    currentPosition = 0;
    orderPosition   = 0;
    
    stepPin = 0;
    dirPin  = 0;
    ms1Pin  = 0;
    ms2Pin  = 0;
}

Focus_Motor::Focus_Motor(char stepPin, char dirPin, char ms1Pin, char ms2Pin)
{
    currentPosition = 0;
    orderPosition   = 0;
    currentSpeed    = SPEED_ONE;
    
    this->stepPin = stepPin;
    this->dirPin  = dirPin;
    this->ms1Pin  = ms1Pin;
    this->ms2Pin  = ms2Pin;
    
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(ms1Pin, OUTPUT);
    pinMode(ms2Pin, OUTPUT);
}


void Focus_Motor::setSpeed(int speed)
{
  currentSpeed = speed;
}

/* This function allows the desired position to be updated. I choose to
 * only allow a direction input since the program internally tracks
 * current and desired position. If an absolute desired position function
 * were to be implemented nothing would break.
 */

void Focus_Motor::updateOrder(char dir)
{
  if (dir == MOVEOUT && orderPosition <= (MAX_STEPS - currentSpeed))
  {
    orderPosition += currentSpeed;
  }
  else if (dir == MOVEIN && orderPosition >= currentSpeed)
  {
    orderPosition -= currentSpeed;
  } 
}

void Focus_Motor::stopAtCurrentPosition()
{
  orderPosition = currentPosition;
}

/* Returns the desired position. Typically for display purposes.
 */

unsigned int Focus_Motor::getOrder()
{
  return orderPosition;
}

/* Returns the desired position. Typically for display purposes.
 */

unsigned int Focus_Motor::getPosition()
{
  return currentPosition;
}

/* This function needs to be called once per loop. It will step the
 * motor and then keep track of the position until the desired
 * position is reached.
 * Doing the motor stepping this way decouples the motor movement
 * from the rotary encoder and prevents missteps caused by spinning 
 * the encoder too quickly.
 */

void Focus_Motor::processMotor()
{
  if (orderPosition < currentPosition)
  {
    digitalWrite(dirPin, LOW);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
    currentPosition--;
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPin, LOW);
  }
  
  if (orderPosition > currentPosition)
  {
    digitalWrite(dirPin, HIGH);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
    currentPosition++;
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPin, LOW);
  }
}
