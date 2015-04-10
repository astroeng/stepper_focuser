#include <Arduino.h>
#include "focus_display.h"

/* With a timer clock multiplier of 1/1024 and a timeout of 160 this ISR will fire
 * about every 6144 microseconds. Which is a refresh rate of 40.69 hz at the display.
 * 130 gives about 31.00 hz Which is just a bit faster than the human eye's perception.
 * After looking at it for a minute, I'm convinced that I see flicker. Moving to 145
 * seems to have solved it and keeps the ISR from running excessivly.
 * 145 gives about 35.2 hz
 */
#define TIMERCOUNTLIMIT 145;

/* These variables are used by the ISR and need to be static globals for
 * that reason. Unless the ISR can be made to run within the scope of the
 * class. I'm not sure what the benifit would be since the class would 
 * still be restricted to a single instance given that the ISR is tied to
 * a hardware component on the AVR.
 */
static volatile unsigned char digit = 0;
static volatile unsigned int digitArray[4];
static volatile unsigned int displayValue[4];

/* Since the ISR needs the knowledge of these pins they need to be static
 * globals not held as part of the class. That is unless the ISR can be
 * made to run within the scope of the class.
 */
static int dataPinG       = 0;
static int dataClockPinG  = 0;
static int latchClockPinG = 0;

/* This ISR refreshes a sigle digit on the 7 segment display each time it
 * runs. The ISR needs to run four times to update the entire display.
 */
ISR(TIMER2_OVF_vect)
{

  /* Reset the timer value. */
  TCNT2 = TIMERCOUNTLIMIT;
  
  /* Make sure that this pin is low when this ISR starts. */
  
  digitalWrite(latchClockPinG, LOW);
  
  /* Shift out the firrst byte which is the digit selection. Then shift
   * out the byte that controls the displayed number. 
   */
  shiftOut(dataPinG, dataClockPinG, MSBFIRST, highByte(digitArray[digit] | (~(displayValue[digit]))));
  shiftOut(dataPinG, dataClockPinG, MSBFIRST, lowByte(digitArray[digit] | (~(displayValue[digit]))));
  
  /* Pulse the clock for the parallel load of the actual output latches. */
  digitalWrite(latchClockPinG, HIGH);
  digitalWrite(latchClockPinG, LOW);
  
  digit++;
  digit = digit % 4;
};

/* This frinction is used to setup timer 2 which is the source of the
 * interrupt that is refreshing the 7 segment display.
 */
void setupTimer()
{
  /* First disable the timer overflow interrupt while we're configuring */  
  TIMSK2 &= ~(1<<TOIE2);  
  /* Configure timer2 in normal mode (pure counting, no PWM etc.) */  
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));  
  TCCR2B &= ~(1<<WGM22);  
  /* Select clock source: internal I/O clock */  
  ASSR &= ~(1<<AS2);  
  /* Disable Compare Match A interrupt enable (only want overflow) */  
  TIMSK2 &= ~(1<<OCIE2A);

  /* Now configure the prescaler to CPU clock divided by 1024 */
  /* Page 158 in the chip manual. */
  TCCR2B |= ((1<<CS22) | (1<<CS21) | (1<<CS20)); // Set bits
  TCCR2B &= ~(0);                                // Clear bit

  /* Finally load end enable the timer */  
  TCNT2 = TIMERCOUNTLIMIT;  
  TIMSK2 |= (1<<TOIE2);
}

/* This function turns a number in int format into an array that can
 * be used to drive the 7 segment display.
 */
void Focus_Display::setDisplayValue(unsigned long value)
{
  /* Critical Section Start */
  noInterrupts();
  displayValue[3] = numberArray[value % 10];
  value = value / 10;
  displayValue[2] = numberArray[value % 10];
  value = value / 10;
  displayValue[1] = numberArray[value % 10];
  value = value / 10;
  displayValue[0] = numberArray[value % 10];
  interrupts();
  /* Critical Section End */
}

Focus_Display::Focus_Display()
{
  /* Dummy Constructor */
}

Focus_Display::Focus_Display(int dataClockPin, int dataPin, int latchPin)
{
  /* Save the pins so that the ISR has visibility to them. */
  dataClockPinG  = dataClockPin;
  dataPinG       = dataPin;
  latchClockPinG = latchPin;

  pinMode(dataClockPinG , OUTPUT);
  pinMode(latchClockPinG, OUTPUT);
  pinMode(dataPinG      , OUTPUT);  
  
  decimalPoint = 0xFF40;

  /* These values are active low so they are inverted when or'ed and written out to
   * the shift registers */
  numberArray[0] = 0xFFBB; // 0
  numberArray[1] = 0xFF12; // 1
  numberArray[2] = 0xFF8f; // 2
  numberArray[3] = 0xFF1F; // 3
  numberArray[4] = 0xFF36; // 4
  numberArray[5] = 0xFF3D; // 5
  numberArray[6] = 0xFFB5; // 6
  numberArray[7] = 0xFF1A; // 7
  numberArray[8] = 0xFFBF; // 8 
  numberArray[9] = 0xFF3E; // 9

  /* These values are active high. These are or'ed with the numberArray. */
  digitArray[0] = 0x0100;
  digitArray[1] = 0x0200;
  digitArray[2] = 0x0400;
  digitArray[3] = 0x0800;

  displayValue[0] = 0;
  displayValue[1] = 0;
  displayValue[2] = 0;
  displayValue[3] = 0;
  
  setupTimer();
}
