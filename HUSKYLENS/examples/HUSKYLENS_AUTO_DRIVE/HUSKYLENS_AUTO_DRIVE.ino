/***************************************************
 HUKSYLENS An Easy-to-use AI Machine Vision Sensor
 <https://www.dfrobot.com/product-1922.html>
 
 ***************************************************

 
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

void loop() {
    if (huskylens.writeLearn(1))
    {
        Serial.println("1");
    }
    delay(1000);

    if (huskylens.writeLearn(2))
    {
        Serial.println("2");
    }
    delay(1000);

    if (huskylens.writeLearn(3))
    {
        Serial.println("3");
    }
    delay(1000);

    if (huskylens.writeLearn(4))
    {
        Serial.println("4");
    }
    delay(1000);

    if (huskylens.writeForget())
    {
        Serial.println("F");
    }
    delay(1000);
}