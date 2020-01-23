#include "HUKSYLENS.h"
#include "DFMobile.h"

DFMobile Robot (7,6,4,5);     // initiate the Motor pin

HUKSYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL

void setup() {
    Serial.begin(115200);
    Robot.Direction (HIGH, LOW);  // initiate the positive direction  

    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUKSYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }

    // while (true)
    // {Robot.Speed (200-50,200+50);
    // delay(2000);
    // Robot.Speed (0,0);
    // delay(2000);
    // Robot.Speed (200,200);
    // delay(2000);
    // Robot.Speed (0,0);
    // delay(2000);
    // Robot.Speed (200+50,200-50);
    // delay(2000);
    // Robot.Speed (0,0);
    // delay(2000);}

}

int widthLevel = 50;

int xLeft = 160-40;
int xRight = 160+40;

bool isTurning = false;
bool isTurningLeft = true;

bool isInside(int value, int min, int max){
    return (value >= min && value <= max);
}

void loop() {
    int32_t error;

    int left = 0, right = 0;

    if (!huskylens.request()) Serial.println(F("Fail to request objects from HUSKYLENS!"));
    else if(!huskylens.isLearned()) {Serial.println(F("Object not learned!")); Robot.Speed (0,0);}
    else if(!huskylens.blocks.available()) Serial.println(F("Object disappeared!"));
    else
    {
        HUSKYLENSResult result = huskylens.blocks.read();

        if (result.width < widthLevel)
        {
            widthLevel = 65;
            if (isInside(result.xCenter, 0, xLeft))
            {
                if (isTurningLeft)
                {
                    if (!isTurning)
                    {
                        Robot.Speed (200-50,200+50);
                    }
                }
                else
                {
                    if (isTurning)
                    {
                        isTurning = false;
                        isTurningLeft = !isTurningLeft;
                    }
                    Robot.Speed (200-50,200+50);
                }
            }
            else if (isInside(result.xCenter, xLeft, xRight))
            {
                if (isTurning)
                {
                    isTurning = false;
                    isTurningLeft = !isTurningLeft;
                }
                Robot.Speed (200,200);
            }
            else if (isInside(result.xCenter, xRight, 320))
            {
                if (isTurningLeft)
                {
                    if (isTurning)
                    {
                        isTurning = false;
                        isTurningLeft = !isTurningLeft;
                    }
                    
                    Robot.Speed (200+50,200-50);
                }
                else
                {
                    if (!isTurning)
                    {
                        Robot.Speed (200+50,200-50);
                    }
                }
            }
        }
        else
        {
            widthLevel = 55;
            isTurning = true;
            if (isTurningLeft)
            {
                Robot.Speed (0,200);
            }
            else
            {
                Robot.Speed (200,0);
            }
            
            
            
        }
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height);

    }
}