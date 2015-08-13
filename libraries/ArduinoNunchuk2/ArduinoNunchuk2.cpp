/*
 * ArduinoNunchuk2.cpp - Improved Wii Nunchuk library for Arduino
 *
 * Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/
 *
 * Project URL: http://www.gabrielbianconi.com/projects/arduinonunchuk/
 *
 * Based on the following resources:
 *   http://www.windmeadow.com/node/42
 *   http://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/
 *   http://wiibrew.org/wiki/Wiimote/Extension_Controllers
 *
 */

#include <Arduino.h>
#include <Wire.h>
#include "ArduinoNunchuk2.h"

#define ADDRESS 0x52

void ArduinoNunchuk2::init()
{
  Wire1.begin();

  ArduinoNunchuk2::_sendByte(0x55, 0xF0);
  ArduinoNunchuk2::_sendByte(0x00, 0xFB);

  ArduinoNunchuk2::update();
}

void ArduinoNunchuk2::update()
{
  int count = 0;
  int values[6];

  Wire1.requestFrom(ADDRESS, 6);

  while(Wire1.available())
  {
    values[count] = Wire1.read();
    count++;
  }

  ArduinoNunchuk2::analogX = values[0];
  ArduinoNunchuk2::analogY = values[1];
  ArduinoNunchuk2::accelX = (values[2] << 2) | ((values[5] >> 2) & 3);
  ArduinoNunchuk2::accelY = (values[3] << 2) | ((values[5] >> 4) & 3);
  ArduinoNunchuk2::accelZ = (values[4] << 2) | ((values[5] >> 6) & 3);
  ArduinoNunchuk2::zButton = !((values[5] >> 0) & 1);
  ArduinoNunchuk2::cButton = !((values[5] >> 1) & 1);

  ArduinoNunchuk2::_sendByte(0x00, 0x00);
}

void ArduinoNunchuk2::_sendByte(byte data, byte location)
{
  Wire1.beginTransmission(ADDRESS);

  Wire1.write(location);
  Wire1.write(data);

  Wire1.endTransmission();

  delay(10);
}