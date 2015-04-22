
/*************************************************************
 * Derek Schacht
 * 2014/04/23
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */


#define MOVEOUT 1
#define MOVEIN  2

#define SPEED_ONE   1
#define SPEED_TWO   8
#define SPEED_THREE 16
#define SPEED_FOUR  64

#define MAX_STEPS 4800

class Focus_Motor
{
  public:
    Focus_Motor();
    Focus_Motor(char stepPin, char dirPin, char ms1Pin, char ms2Pin);

    void updateOrder(char dir);
    void stopAtCurrentPosition();

    void setSpeed(int speed);

    unsigned int getPosition();
    unsigned int getOrder();
    
    void processMotor();
    
  private:
    unsigned int currentPosition;
    unsigned int orderPosition;
    int currentSpeed;
    char stepPin;
    char dirPin;
    char ms1Pin;
    char ms2Pin;
};
