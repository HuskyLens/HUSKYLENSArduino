#include "HUKSYLENS.h"
#include "SoftwareSerial.h"

HUKSYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL
int ID0 = 0; //not learned results. Grey result on HUSKYLENS screen
int ID1 = 1; //first learned results. colored result on HUSKYLENS screen
int ID2 = 2; //second learned results. colored result on HUSKYLENS screen
// and so on.....

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
    
    // huskylens.writeAlgorithm(ALGORITHM_FACE_RECOGNITION);
    // huskylens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING);
    // huskylens.writeAlgorithm(ALGORITHM_OBJECT_RECOGNITION);
    // huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING);
    // huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION);
    // huskylens.writeAlgorithm(ALGORITHM_TAG_RECOGNITION);
}

void loop() {
    if (huskylens.request())
    // if (huskylens.requestBlocks())
    // if (huskylens.requestArrows())
    // if (huskylens.requestLearned())
    // if (huskylens.requestBlocksLearned())
    // if (huskylens.requestArrowsLearned())
    // if (huskylens.request(ID1))
    // if (huskylens.requestBlocks(ID1))
    // if (huskylens.requestArrows(ID1))
    {
        Serial.println("###################################");
        Serial.println(String()+F("Number of learned IDs:")+huskylens.countLearnedIDs());
        Serial.println(String()+F("frame number:")+huskylens.frameNumber());

        Serial.println(String()+F("Number of results:")+huskylens.available());
        for (int i = 0; i < huskylens.available(); i++)
        {
            HUSKYLENSResult result = huskylens.readDirect(i);
            if (result.command == COMMAND_RETURN_BLOCK)
            {
                Serial.println(String()+i+F(":Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
            }
            else if (result.command == COMMAND_RETURN_ARROW)
            {
                Serial.println(String()+i+F(":Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
            }
        }
        Serial.println(String()+F("Number of Blocks:")+huskylens.blocks.available());
        for (int i = 0; i < huskylens.blocks.available(); i++)
        {
            HUSKYLENSResult result = huskylens.blocks.readDirect(i);
            Serial.println(String()+i+F(":Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
        }
        Serial.println(String()+F("Number of Arrows:")+huskylens.arrows.available());
        for (int i = 0; i < huskylens.arrows.available(); i++)
        {
            HUSKYLENSResult result = huskylens.arrows.readDirect(i);
            Serial.println(String()+i+F(":Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
        }

        Serial.println(String()+F("Number of results ID0:")+huskylens.available(ID0));
        if (huskylens.available(ID0))
        {
            HUSKYLENSResult result = huskylens.read(ID0);
            if (result.command == COMMAND_RETURN_BLOCK)
            {
                Serial.println(String()+0+F(":Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height);
            }
            else if (result.command == COMMAND_RETURN_ARROW)
            {
                Serial.println(String()+0+F(":Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget);
            }
        }
        for (int i = 0; i < huskylens.available(ID0); i++)
        {
            HUSKYLENSResult result = huskylens.read(ID0, i);
            if (result.command == COMMAND_RETURN_BLOCK)
            {
                Serial.println(String()+i+F(":Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height);
            }
            else if (result.command == COMMAND_RETURN_ARROW)
            {
                Serial.println(String()+i+F(":Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget);
            }
        }

        Serial.println(String()+F("Number of blocks ID0:")+huskylens.blocks.available(ID0));
        if (huskylens.blocks.available(ID0))
        {
            HUSKYLENSResult result = huskylens.blocks.read(ID0);
            Serial.println(String()+0+F(":Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height);
        }
        for (int i = 0; i < huskylens.blocks.available(ID0); i++)
        {
            HUSKYLENSResult result = huskylens.blocks.read(ID0, i);
            Serial.println(String()+i+F(":Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height);
        }

        Serial.println(String()+F("Number of arrows ID0:")+huskylens.arrows.available(ID0));
        if (huskylens.arrows.available(ID0))
        {
            HUSKYLENSResult result = huskylens.arrows.read(ID0);
            Serial.println(String()+0+F(":Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget);
        }
        for (int i = 0; i < huskylens.arrows.available(ID0); i++)
        {
            HUSKYLENSResult result = huskylens.arrows.read(ID0, i);  
            Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget);
        }

        Serial.println(String()+F("Number of results ID1:")+huskylens.available(ID1));
        if (huskylens.available(ID1))
        {
            HUSKYLENSResult result = huskylens.read(ID1);
            if (result.command == COMMAND_RETURN_BLOCK)
            {
                Serial.println(String()+0+F(":Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height);
            }
            else if (result.command == COMMAND_RETURN_ARROW)
            {
                Serial.println(String()+0+F(":Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget);
            }
        }
        for (int i = 0; i < huskylens.available(ID1); i++)
        {
            HUSKYLENSResult result = huskylens.read(ID1, i);
            if (result.command == COMMAND_RETURN_BLOCK)
            {
                Serial.println(String()+i+F(":Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height);
            }
            else if (result.command == COMMAND_RETURN_ARROW)
            {
                Serial.println(String()+i+F(":Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget);
            }
        }

        Serial.println(String()+F("Number of blocks ID1:")+huskylens.blocks.available(ID1));
        if (huskylens.blocks.available(ID1))
        {
            HUSKYLENSResult result = huskylens.blocks.read(ID1);
            Serial.println(String()+0+F(":Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height);
        }
        for (int i = 0; i < huskylens.blocks.available(ID1); i++)
        {
            HUSKYLENSResult result = huskylens.blocks.read(ID1, i);
            Serial.println(String()+i+F(":Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height);
        }

        Serial.println(String()+F("Number of arrows ID1:")+huskylens.arrows.available(ID1));
        if (huskylens.arrows.available(ID1))
        {
            HUSKYLENSResult result = huskylens.arrows.read(ID1);
            Serial.println(String()+0+F(":Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget);
        }
        for (int i = 0; i < huskylens.arrows.available(ID1); i++)
        {
            HUSKYLENSResult result = huskylens.arrows.read(ID1, i);  
            Serial.println(String()+i+F(":Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget);
        }
    }
    else
    {
        Serial.println("Fail to request objects from Huskylens!");
    }
}
