/**************************************************************************/
/*!
 	@file DFRobotCar.cpp
 Romeo library of car Speed
 	Copyright (C) 2015 DFRobot
 */

#include <Arduino.h>
#include "DFMobile.h"

/*left_en: enable left pin
 right_en:enable right pin
 left_pwm:pwm speed of left wheel control
 right_pwm:pwm speed of right wheel control
 */
DFMobile::DFMobile (uint8_t left_en, uint8_t left_pwm, uint8_t right_en, uint8_t right_pwm) {
  _left_en = left_en;
  _left_pwm = left_pwm;
  _right_en = right_en;
  _right_pwm = right_pwm;
  pinMode (_left_en, OUTPUT);
  pinMode (_left_pwm, OUTPUT);
  pinMode (_right_en, OUTPUT);
  pinMode (_right_pwm, OUTPUT);

  _left_advance = LOW;
  _left_back = HIGH;
  _right_advance = HIGH;
  _right_back = LOW;
}

//Direction utilizes bool value to control the wheel direction: forward or go back
void DFMobile::Direction (bool left, bool right) {
  if (left) {
    _left_advance = !_left_advance;
    _left_back = !_left_back;
  }
  if (right) {
    _right_advance = !_right_advance;
    _right_back = !_right_back;
  }
}

/*Speed function is designed to specify left and right wheel's speed, 
  whose range is (-255 ~ 255)*/ 
void DFMobile::Speed (int16_t left_speed, int16_t right_speed) {
  if (left_speed < 0) {
    left_speed = -left_speed;
    digitalWrite (_left_en, _left_back);
  } 
  else {
    digitalWrite (_left_en, _left_advance);
  }
  if (right_speed < 0) {
    right_speed = -right_speed;
    digitalWrite (_right_en, _right_back);
  } 
  else {
    digitalWrite (_right_en, _right_advance);
  }
  if (left_speed > 255)
    left_speed = 255;
  if (right_speed > 255)
    right_speed = 255;
  analogWrite (_left_pwm, left_speed);
  analogWrite (_right_pwm, right_speed);
}



