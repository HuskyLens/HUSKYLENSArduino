#include "HUKSYLENS.h"
#include "SoftwareSerial.h"
#include "PIDLoop.h"
#include "DFMobile.h"

#define ZUMO_FAST        255

DFMobile Robot (7,6,4,5);     // initiate the Motor pin
PIDLoop headingLoop(2000, 0, 0, false);
HUKSYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL
int ID1 = 1;

void setup() {
    Serial.begin(115200);
    Robot.Direction (HIGH, LOW);  // initiate the positive direction  

    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUKSYLENS (General Settings>>Protol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
    huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING);
}
int left = 0, right = 0;

void loop() {
    int32_t error; 
    if (!huskylens.request(ID1)) {Serial.println(F("Fail to request objects from HUSKYLENS!"));left = 0; right = 0;}
    else if(!huskylens.isLearned()) {Serial.println(F("Object not learned!"));left = 0; right = 0;}
    else if(!huskylens.arrows.available()) Serial.println(F("Object disappeared!"));
    else
    {
        HUSKYLENSResult result = huskylens.arrows.read(ID1);
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget);

        // Calculate heading error with respect to m_x1, which is the far-end of the vector,
        // the part of the vector we're heading toward.
        error = (int32_t)result.xTarget - (int32_t)160;

        // pixy.line.vectors->print();
        // Perform PID calcs on heading error.
        headingLoop.update(error);

        // separate heading into left and right wheel velocities.
        left = headingLoop.m_command;
        right = -headingLoop.m_command;

        left += ZUMO_FAST;
        right += ZUMO_FAST;
    }

    Serial.println(String()+left+","+right);
    Robot.Speed (left,right);
}