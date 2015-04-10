
/*************************************************************
 * Derek Schacht
 * 2014/04/23
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use consent.
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

    void updatePosition(char dir);
    void updateSpeed(int newSpeed);
    void processMotor();
    unsigned int getPosition();
    unsigned int getOrder();
  private:
    unsigned int currentPosition;
    unsigned int desiredPosition;
    int currentSpeed;
    char stepPin;
    char dirPin;
    char ms1Pin;
    char ms2Pin;
};
