# arduino-hcsr04-park
Garage park distance visualizer based on HC-SR04 sonar sensor

* ![Alt Text](https://media.giphy.com/media/3BMlb84jMVmYVSHkPk/giphy.gif)
* Sits at the back of the garage and displays distance to the front bumper.
* Warns when too close.
* Display goes into simple mode if sensor is blocked more than 30 seconds.

# Hardware

* Arduino
* HC-SR04 sonar sensor
* MAX7219 based 8x8 LED matrix kit (similar to https://github.com/rwaldron/johnny-five/wiki/Led.Matrix)

# Issues

* I'm finding the MAX7219 is slow to refresh and even keeping the matrix steady seems to affect distance measurement.
* Switching to I2C based 7-segment display might resolve some of the abnormalities with distance measurement I'm seeing when refreshing the screen.

