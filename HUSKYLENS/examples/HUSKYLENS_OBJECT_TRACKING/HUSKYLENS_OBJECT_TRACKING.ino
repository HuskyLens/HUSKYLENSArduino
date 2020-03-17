/***************************************************
 HUSKYLENS An Easy-to-use AI Machine Vision Sensor
 <https://www.dfrobot.com/product-1922.html>
 
 ***************************************************
 This example shows how to play with object tracking.
 
 Created 2020-03-13
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
 <https://wiki.dfrobot.com/HUSKYLENS_V1.0_SKU_SEN0305_SEN0336#target_23>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/


#include "HUSKYLENS.h"
#include "DFMobile.h"

DFMobile Robot (7,6,4,5);     // initiate the Motor pin

HUSKYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL

void setup() {
    Serial.begin(115200);
    Robot.Direction (HIGH, LOW);  // initiate the positive direction  

    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }

    huskylens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING); //Switch the algorithm to object tracking.

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

void printResult(HUSKYLENSResult result);

void loop() {
    int32_t error;

    int left = 0, right = 0;

    if (!huskylens.request()) Serial.println(F("Fail to request objects from HUSKYLENS!"));
    else if(!huskylens.isLearned()) {Serial.println(F("Object not learned!")); Robot.Speed (0,0);}
    else if(!huskylens.available()) Serial.println(F("Object disappeared!"));
    else
    {
        HUSKYLENSResult result = huskylens.read();

        if (result.width < widthLevel){
            widthLevel = 65;
            if (isInside(result.xCenter, 0, xLeft)){
                if (isTurningLeft){
                    if (!isTurning){
                        Robot.Speed (200-50,200+50);
                    }
                }
                else{
                    if (isTurning){
                        isTurning = false;
                        isTurningLeft = !isTurningLeft;
                    }
                    Robot.Speed (200-50,200+50);
                }
            }
            else if (isInside(result.xCenter, xLeft, xRight)){
                if (isTurning){
                    isTurning = false;
                    isTurningLeft = !isTurningLeft;
                }
                Robot.Speed (200,200);
            }
            else if (isInside(result.xCenter, xRight, 320)){
                if (isTurningLeft){
                    if (isTurning){
                        isTurning = false;
                        isTurningLeft = !isTurningLeft;
                    }
                    Robot.Speed (200+50,200-50);
                }
                else{
                    if (!isTurning){
                        Robot.Speed (200+50,200-50);
                    }
                }
            }
        }
        else
        {
            widthLevel = 55;
            isTurning = true;
            if (isTurningLeft){
                Robot.Speed (0,200);
            }
            else{
                Robot.Speed (200,0);
            }
        }
        printResult(result);
    }
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}