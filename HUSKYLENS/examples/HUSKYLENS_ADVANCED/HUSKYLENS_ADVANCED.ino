/***************************************************
 HUSKYLENS An Easy-to-use AI Machine Vision Sensor
 <https://www.dfrobot.com/product-1922.html>
 
 ***************************************************
 This example shows the full function of library for HUSKYLENS via I2c.
 
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
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL
int ID0 = 0; //not learned results. Grey result on HUSKYLENS screen
int ID1 = 1; //first learned results. colored result on HUSKYLENS screen
int ID2 = 2; //second learned results. colored result on HUSKYLENS screen
// and so on.....

void printResult(HUSKYLENSResult result);

void setup() {
    Serial.begin(115200);
    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
    
    // Uncomment one of the following code to switch the algorithm on HUSKYLENS:
    // huskylens.writeAlgorithm(ALGORITHM_FACE_RECOGNITION);
    // huskylens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING);
    // huskylens.writeAlgorithm(ALGORITHM_OBJECT_RECOGNITION);
    // huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING);
    // huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION);
    // huskylens.writeAlgorithm(ALGORITHM_TAG_RECOGNITION);
}

void loop() {
    if (huskylens.request())                    //request all blocks and arrows from HUSKYLENS
    // if (huskylens.requestBlocks())           //request only blocks from HUSKYLENS
    // if (huskylens.requestArrows())           //request only arrows from HUSKYLENS
    // if (huskylens.requestLearned())          //request blocks and arrows tangged ID != 0 from HUSKYLENS
    // if (huskylens.requestBlocksLearned())    //request blocks tangged ID != ID0 from HUSKYLENS
    // if (huskylens.requestArrowsLearned())    //request arrows tangged ID != ID0 from HUSKYLENS
    // if (huskylens.request(ID1))              //request blocks and arrows tangged ID == ID1 from HUSKYLENS
    // if (huskylens.requestBlocks(ID1))        //request blocks tangged ID == ID1 from HUSKYLENS
    // if (huskylens.requestArrows(ID1))        //request arrows tangged ID == ID1 from HUSKYLENS
    // if (huskylens.request(ID2))              //request blocks and arrows tangged ID == ID2 from HUSKYLENS
    // if (huskylens.requestBlocks(ID2))        //request blocks tangged ID == ID2 from HUSKYLENS
    // if (huskylens.requestArrows(ID2))        //request arrows tangged ID == ID2 from HUSKYLENS
    {
        Serial.println("###################################");
        Serial.println(String()+F("Count of learned IDs:")+huskylens.countLearnedIDs());//The count of (faces, colors, objects or lines) you have learned on HUSKYLENS.
        Serial.println(String()+F("frame number:")+huskylens.frameNumber());//The number of frame HUSKYLENS have processed.

        Serial.println("#######");
        Serial.println(String()+F("Get all blocks and arrows. Count:")+huskylens.count());
        for (int i = 0; i < huskylens.count(); i++)
        {
            HUSKYLENSResult result = huskylens.get(i);
            printResult(result);
        }

        Serial.println("#######");
        Serial.println(String()+F("Get all blocks. Count:")+huskylens.countBlocks());
        for (int i = 0; i < huskylens.countBlocks(); i++)
        {
            HUSKYLENSResult result = huskylens.getBlock(i);
            printResult(result);
        }

        Serial.println("#######");
        Serial.println(String()+F("Get all arrows. Count:")+huskylens.countArrows());
        for (int i = 0; i < huskylens.countArrows(); i++)
        {
            HUSKYLENSResult result = huskylens.getArrow(i);
            printResult(result);
        }

        Serial.println("#######");
        Serial.println(String()+F("Get all blocks and arrows tagged ID0. Count:")+huskylens.count(ID0));
        for (int i = 0; i < huskylens.count(ID0); i++)
        {
            HUSKYLENSResult result = huskylens.get(ID0, i);
            printResult(result);
        }

        Serial.println("#######");
        Serial.println(String()+F("Get all blocks with learn ID equals ID0. Count:")+huskylens.countBlocks(ID0));
        for (int i = 0; i < huskylens.countBlocks(ID0); i++)
        {
            HUSKYLENSResult result = huskylens.getBlock(ID0, i);
            printResult(result);
        }

        Serial.println("#######");
        Serial.println(String()+F("Get all arrows tagged ID0. Count:")+huskylens.countArrows(ID0));
        for (int i = 0; i < huskylens.countArrows(ID0); i++)
        {
            HUSKYLENSResult result = huskylens.getArrow(ID0, i);  
            printResult(result);
        }

        Serial.println("#######");
        Serial.println(String()+F("Get all blocks and arrows tagged ID1. Count:")+huskylens.count(ID1));
        for (int i = 0; i < huskylens.count(ID1); i++)
        {
            HUSKYLENSResult result = huskylens.get(ID1, i);
            printResult(result);
        }

        Serial.println("#######");
        Serial.println(String()+F("Get all blocks and arrows tagged ID2. Count:")+huskylens.count(ID2));
        for (int i = 0; i < huskylens.count(ID2); i++)
        {
            HUSKYLENSResult result = huskylens.get(ID2, i);
            printResult(result);
        }
    }
    else
    {
        Serial.println("Fail to request objects from Huskylens!");
    }
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){//result is a block
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){//result is an arrow
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{//result is unknown.
        Serial.println("Object unknown!");
    }
}