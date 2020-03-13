#include "HUKSYLENS.h"
#include "SoftwareSerial.h"

HUKSYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL

void setup() {
    Serial.begin(115200);
    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUKSYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
}

int i = 0;

void loop() {
    huskylens.writeSensor(analogRead(A1),analogRead(A2),analogRead(A3));
    delay(10);
}