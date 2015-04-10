
#ifndef FOCUS_MOTOR_H
#define FOCUS_MOTOR_H

#define MOVEOUT 1
#define MOVEIN  2

#define SPEED_ONE   1
#define SPEED_TWO   8
#define SPEED_THREE 16
#define SPEED_FOUR  64

class Focus_Motor
{
  public:
    Focus_Motor();
    Focus_Motor(char stepPin, char dirPin, char ms1Pin, char ms2Pin);

    void updatePosition(char dir);
    void updateSpeed(int newSpeed);
    void processMotor();
    unsigned int getPosition();
  private:
    unsigned int currentPosition;
    unsigned int desiredPosition;
    int currentSpeed;
    char stepPin;
    char dirPin;
    char ms1Pin;
    char ms2Pin;
};

#endif
