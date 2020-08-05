/***************************************************
 HUSKYLENS An Easy-to-use AI Machine Vision Sensor
 <https://www.dfrobot.com/product-1922.html>
 
 ***************************************************
 This example shows the general utility functions for the HuskyLens. 
 
 Created 2020-08-04
 By [Robert](robert@dfrobot.com)
 
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
SoftwareSerial mySerial(10, 11); // RX, TX
//HUSKYLENS green line >> Pin 10; blue line >> Pin 11

void setup()
{
    Serial.begin(115200);
    mySerial.begin(9600);
    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
    huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION); //Switch the algorithm to color recognition.

    /***** UTILITY FUNCTIONS  *******/

    bool testSDCardFunctions = false; // If you have an SD Card, set true

    if (testSDCardFunctions)
    {
        /***** Save Pictures & Screenshots to the SD Card  *******/
        Serial.println("Save a screenshot & picture to the SD Card");

        huskylens.saveScreenshotToSDCard();
        huskylens.savePictureToSDCard();
        delay(1000);

        /***** Saving and loading model file to SD Card  *******/
        Serial.println("Save and load a model file");

        huskylens.writeForget(); //Reset alg
        huskylens.writeLearn(1); //Learn a color with ID 1

        huskylens.saveModelToSDCard(1);   //Save the current model to "ColorTracking_Backup_1.conf" on the SD Card
        huskylens.writeForget();          //Reset alg
        huskylens.loadModelFromSDCard(1); //Load the model from "ColorTracking_Backup_1.conf"
        delay(5000);
    }

    /***** Set a custom name for an object  *******/
    Serial.println("Set a custom name for a learned object");

    huskylens.writeForget(); //Reset alg

    while (!huskylens.isLearned())
    {                            //Ensure we have a block that we can change the name of
        huskylens.writeLearn(1); //Learn a color with ID 1
        huskylens.request();     //Get the latest data from the HuskyLens
    }

    huskylens.setCustomName("TEST_NAME", 1); //Set "TEST_NAME"
    delay(5000);                             //Wait 5 seconds

    /***** Add custom text to the UI  *******/
    Serial.println("Add custom text to the HuskyLens");

    huskylens.customText("TEST_TEXT_1", 120, 120); //Add first custom text at (120,120)
    huskylens.customText("TEST_TEXT_2", 50, 200);  //Add second custom text at (50,200)
    delay(5000);
    huskylens.customText("TEST_TEXT_1_REV", 120, 120); //Change the first text to a new string
    delay(5000);
    huskylens.clearCustomText(); //Remove all custom texts
    delay(1000);

    /***** Check if the model is Pro or Standard  *******/
    bool isPro;
    isPro = huskylens.isPro();
    if (isPro)
    {
        Serial.println("Pro Model Detected");
    }
    else
    {
        Serial.println("Standard Model Detected");
    }

    huskylens.checkFirmwareVersion();
    Serial.println("Done");
}

void loop()
{
}
