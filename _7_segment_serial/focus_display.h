#ifndef FOCUS_DISPLAY_H
#define FOCUS_DISPLAY_H

class Focus_Display
{
  public:
    Focus_Display();
    Focus_Display(int dataClockPin, int dataPin, int latchPin);
  
    void setDisplayValue(unsigned long value);
  private:
    unsigned int decimalPoint;

    unsigned int numberArray[10];
};

#endif
