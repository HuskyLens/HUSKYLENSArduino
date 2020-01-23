/**************************************************************************/
/*!
 	@file    DFMobile.h
 heading library file of Romeo DFMobile Speed
 	Copyright (C) 2015 DFRobot
 */

#include <Arduino.h>

class DFMobile {
private:
  bool _left_advance;// = LOW;
  bool _left_back;// = HIGH;
  bool _right_advance;// = HIGH;
  bool _right_back;// = LOW;

  uint8_t _left_en;// = 4;
  uint8_t _left_pwm;// = 5;
  uint8_t _right_en;// = 7;
  uint8_t _right_pwm;// = 6;

public:
  DFMobile (uint8_t left_en, uint8_t left_pwm, uint8_t right_en, uint8_t right_pwm);

  void Direction (bool left, bool right);

  void Speed (int16_t left, int16_t right);
};


