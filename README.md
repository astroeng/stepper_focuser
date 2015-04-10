# stepper_focuser

This project documents a home made stepper motor based focuser add on for an Orion 8” Newtonian telescope. The device consists of two parts the stepper motor driver and the hand paddle used to control the position of the stepper motor. The stepper motor driver is an EasyDriver from SparkFun and Brian Schmalz. It is a great little board but the drive current adjustment is STILL reversed. The hand paddle is connected via a single RJ45 cable  that provides a path for power to the hand paddle and a path for control signals to the stepper driver.

So as a crude block diagram:

12 volt source -> stepper driver ->    12 volts     -> hand paddle
                  stepper driver <- control signals <- hand paddle

Each of the components has a 5 volt regulator for powering the local electronics. The control signals are also 5 volt.

The hand paddle has two 4 digit 7 segment displays, a rotary knob, and two buttons. This is all controlled with an ArduinoPro mini located on the hand paddle PCB.
