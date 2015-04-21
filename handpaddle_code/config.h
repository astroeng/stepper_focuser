#ifndef CONFIG_H
#define CONFIG_H

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

#endif
