# Pong-Arduino-Due-HT1632C
Pong for Arduino Due using an HT1632C LED Matrix and two Wii Nunchucks / Nunchuks
aka Pongduino

This is a Pong clone using an Arduino Due, 2 Wii Nunchuk controllers connected using the Nunchucky, and a 16x24 LED matrix from Adafruit https://www.adafruit.com/products/555. It takes advantage of the Nunchuk motion controls to move the paddles / racquets up and down (instead of the joystick). It can be played against another person, or solitaire using one controller in each hand. 

Parts Needed
TBA

This could be modified to use another Arduino Uno, Leonardo, Mega, etc., but would require slightly more complicated wiring and programming (with a couple of transistors passing control from one Nunchuk to the other) to get the single I2C bus to pass control quickly back and forth between the 2 Nunchuk controllers. Also (because they are 5 volt systems), other Arduinos would also technically require level shifters on SCL and SDA to convert to the preferred 3.3 volts for the Nunchuks. 
