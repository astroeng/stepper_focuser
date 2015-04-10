# stepper_focuser

This project documents a home made stepper motor based focuser add on for an Orion 8” Newtonian telescope. The device consists of two parts the stepper motor driver and the handpaddle used to control the position of the stepper motor. The stepper motor driver is an EasyDriver from SparkFun and Brian Schmalz. Awesome board but the drive current adjustment is STILL reversed! The hand paddle is connected via a single RJ45 cable that provides power to the handpaddle and the control signals to the stepper driver.

12 volt power is provided to each of the components which in turn have a 5 volt regulator for powering the local electronics. The control signals are 5 volt, and the stepper motor runs off of the 12 volt rail.

The user interface consists of two 4 digit 7 segment displays, a rotary knob, and two buttons on the handpaddle. This is all driven with an ArduinoPro mini located on the hand paddle board.

As a secondary function the board was designed to also support a servo via a 3 pin connector. This was intended to be used to drive a mechanical intervalometer. But has since been changed over to an electronic intervalometer, which by happenstance this board will also support. 

The idea behind embedding basically two designs into this board was to save on cost. This is a hobby I try and be as frugal as possible.