/*!
 * @file HUSKYLENS.h
 * @brief HUSKYLENS - An Easy-to-use AI Machine Vision Sensor
 * @n Header file for HUSKYLENS
 *
 * @copyright	[DFRobot]( http://www.dfrobot.com ), 2016
 * @copyright	GNU Lesser General Public License
 *
 * @author [Angelo](Angelo.qiao@dfrobot.com)
 * @author [Robert](robert@dfrobot.com)
 * @version  V1.0.0
 * @date  2020-08-03
 */

#include "Arduino.h"
#include "Wire.h"
#include "HuskyLensProtocolCore.h"

#ifndef _HUSKYLENS_H
#define _HUSKYLENS_H

////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

enum protocolCommand
{
    COMMAND_REQUEST = 0x20,
    COMMAND_REQUEST_BLOCKS,
    COMMAND_REQUEST_ARROWS,
    COMMAND_REQUEST_LEARNED,
    COMMAND_REQUEST_BLOCKS_LEARNED,
    COMMAND_REQUEST_ARROWS_LEARNED,
    COMMAND_REQUEST_BY_ID,
    COMMAND_REQUEST_BLOCKS_BY_ID,
    COMMAND_REQUEST_ARROWS_BY_ID,

    COMMAND_RETURN_INFO,
    COMMAND_RETURN_BLOCK,
    COMMAND_RETURN_ARROW,

    COMMAND_REQUEST_KNOCK,
    COMMAND_REQUEST_ALGORITHM,

    COMMAND_RETURN_OK,
    COMMAND_REQUEST_CUSTOMNAMES,
    COMMAND_REQUEST_PHOTO,
    COMMAND_REQUEST_SEND_PHOTO,
    COMMAND_REQUEST_SEND_KNOWLEDGES,
    COMMAND_REQUEST_RECEIVE_KNOWLEDGES,
    COMMAND_REQUEST_CUSTOM_TEXT,
    COMMAND_REQUEST_CLEAR_TEXT,

    COMMAND_REQUEST_LEARN,
    COMMAND_REQUEST_FORGET,

    COMMAND_REQUEST_SEND_SCREENSHOT,
    COMMAND_REQUEST_SAVE_SCREENSHOT,
    COMMAND_REQUEST_LOAD_AI_FRAME_FROM_USB,
    COMMAND_REQUEST_IS_PRO,
    COMMAND_REQUEST_FIRMWARE_VERSION = 0x3C,

    COMMAND_REQUEST_SENSOR,
};

typedef struct
{
    uint8_t nameDataSize;
    uint8_t id;
    // uint8_t numOfNames;
    // uint8_t algorithim;
    char dataBuffer[21];
} ProtocolCustomNameHeader_t;

typedef struct
{
    uint8_t *dataBuffer;
} ProtocolReceivedKnowledges_t;

typedef struct
{
    uint16_t x;
    uint8_t y;
    uint8_t textSize;
    uint8_t text[21];
} ProtocolCustomText_t;

typedef struct
{
    uint8_t length;
    uint8_t * data;
} ProtocolFirmwareVersion_t;

typedef struct
{
    uint8_t command;
    union
    {
        int16_t first;
        int16_t xCenter;
        int16_t xOrigin;
        int16_t protocolSize;
        int16_t algorithmType;
        int16_t requestID;
    };
    union
    {
        int16_t second;
        int16_t yCenter;
        int16_t yOrigin;
        int16_t knowledgeSize;
    };
    union
    {
        int16_t third;
        int16_t width;
        int16_t xTarget;
        int16_t frameNum;
    };
    union
    {
        int16_t fourth;
        int16_t height;
        int16_t yTarget;
    };
    union
    {
        int16_t fifth;
        int16_t ID;
    };

    ProtocolCustomNameHeader_t customNameHeader;
    ProtocolReceivedKnowledges_t receivedKnowledges;
    ProtocolCustomText_t customText;
    ProtocolFirmwareVersion_t firmwareVersion;
} Protocol_t;

enum protocolAlgorithm
{
    ALGORITHM_FACE_RECOGNITION,
    ALGORITHM_OBJECT_TRACKING,
    ALGORITHM_OBJECT_RECOGNITION,
    ALGORITHM_LINE_TRACKING,
    ALGORITHM_COLOR_RECOGNITION,
    ALGORITHM_TAG_RECOGNITION,
    ALGORITHM_OBJECT_CLASSIFICATION,
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

typedef Protocol_t HUSKYLENSResult;

class HUSKYLENS
{
private:
    TwoWire *wire;
    Stream *stream;
    unsigned long timeOutDuration = 100;
    unsigned long timeOutTimer;
    int16_t currentIndex = 0;
    Protocol_t protocolCache;

    void protocolWrite(uint8_t *buffer, int length)
    {
        if (wire)
        {
            wire->beginTransmission(0x32);
            wire->write(buffer, length);
            wire->endTransmission();
        }
        else if (stream)
        {
            stream->write(buffer, length);
        }
    }

    void timerBegin()
    {
        timeOutTimer = millis();
    }

    bool timerAvailable()
    {
        return (millis() - timeOutTimer > timeOutDuration);
    }

    bool protocolAvailable()
    {
        if (wire)
        {
            if (!wire->available())
            {
                wire->requestFrom(0x32, 16);
            }
            while (wire->available())
            {
                int result = wire->read();
                if (husky_lens_protocol_receive(result))
                {
                    return true;
                }
            }
        }
        else if (stream)
        {
            while (stream->available())
            {
                int result = stream->read();
                if (husky_lens_protocol_receive(result))
                {
                    return true;
                }
            }
        }

        return false;
    }

    Protocol_t protocolInfo;
    Protocol_t *protocolPtr = NULL;

    bool processReturn()
    {
        currentIndex = 0;
        if (!wait(COMMAND_RETURN_INFO))
            return false;
        protocolReadReturnInfo(protocolInfo);
        protocolPtr = (Protocol_t *)realloc(protocolPtr, max((int)protocolInfo.protocolSize, 1) * sizeof(Protocol_t));

        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (!wait())
                return false;
            if (protocolReadReturnBlock(protocolPtr[i]))
                continue;
            else if (protocolReadReturnArrow(protocolPtr[i]))
                continue;
            else
                return false;
        }
        return true;
    }

    HUSKYLENSResult resultDefault;

    bool wait(uint8_t command = 0)
    {
        timerBegin();
        while (!timerAvailable())
        {
            if (protocolAvailable())
            {
                if (command)
                {
                    if (husky_lens_protocol_read_begin(command))
                        return true;
                }
                else
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool readKnock()
    {
        for (int i = 0; i < 5; i++)
        {
            protocolWriteRequestKnock();
            if (wait(COMMAND_RETURN_OK))
            {
                return true;
            }
        }
        return false;
    }

public:
    HUSKYLENS(/* args */)
    {
        wire = NULL;
        stream = NULL;
        resultDefault.command = -1;
        resultDefault.first = -1;
        resultDefault.second = -1;
        resultDefault.third = -1;
        resultDefault.fourth = -1;
        resultDefault.fifth = -1;
    }

    ~HUSKYLENS()
    {
    }

    bool begin(Stream &streamInput)
    {
        stream = &streamInput;
        wire = NULL;
        return readKnock();
    }

    bool begin(TwoWire &streamInput)
    {
        stream = NULL;
        wire = &streamInput;
        return readKnock();
    }

    void setTimeOutDuration(unsigned long timeOutDurationInput)
    {
        timeOutDuration = timeOutDurationInput;
    }

    bool request()
    {
        protocolWriteRequest();
        return processReturn();
    }
    bool request(int16_t ID)
    {
        Protocol_t protocol;
        protocol.requestID = ID;
        protocolWriteRequestByID(protocol);
        return processReturn();
    }

    bool requestBlocks()
    {
        protocolWriteRequestBlocks();
        return processReturn();
    }
    bool requestBlocks(int16_t ID)
    {
        Protocol_t protocol;
        protocol.requestID = ID;
        protocolWriteRequestBlocksByID(protocol);
        return processReturn();
    }

    bool requestArrows()
    {
        protocolWriteRequestArrows();
        return processReturn();
    }
    bool requestArrows(int16_t ID)
    {
        Protocol_t protocol;
        protocol.requestID = ID;
        protocolWriteRequestArrowsByID(protocol);
        return processReturn();
    }
    bool requestLearned()
    {
        protocolWriteRequestLearned();
        return processReturn();
    }
    bool requestBlocksLearned()
    {
        protocolWriteRequestBlocksLearned();
        return processReturn();
    }
    bool requestArrowsLearned()
    {
        protocolWriteRequestArrowsLearned();
        return processReturn();
    }

    int available()
    {
        int result = count();
        currentIndex = min((int)currentIndex, result);
        return result - currentIndex;
    }

    HUSKYLENSResult read()
    {
        return (get(currentIndex++));
    }

    bool isLearned()
    {
        return countLearnedIDs();
    }

    bool isLearned(int ID)
    {
        return (ID <= countLearnedIDs());
    }

    int16_t frameNumber()
    {
        return protocolInfo.frameNum;
    }

    int16_t countLearnedIDs()
    {
        return protocolInfo.knowledgeSize;
    }

    int16_t count()
    {
        return protocolInfo.protocolSize;
    }
    int16_t count(int16_t ID)
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].ID == ID)
                counter++;
        }
        return counter;
    }

    int16_t countBlocks()
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].command == COMMAND_RETURN_BLOCK)
                counter++;
        }
        return counter;
    }
    int16_t countBlocks(int16_t ID)
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].command == COMMAND_RETURN_BLOCK && protocolPtr[i].ID == ID)
                counter++;
        }
        return counter;
    }

    int16_t countArrows()
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].command == COMMAND_RETURN_ARROW)
                counter++;
        }
        return counter;
    }
    int16_t countArrows(int16_t ID)
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].command == COMMAND_RETURN_ARROW && protocolPtr[i].ID == ID)
                counter++;
        }
        return counter;
    }

    int16_t countLearned()
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].ID)
                counter++;
        }
        return counter;
    }
    int16_t countBlocksLearned()
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].command == COMMAND_RETURN_BLOCK && protocolPtr[i].ID)
                counter++;
        }
        return counter;
    }
    int16_t countArrowsLearned()
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].command == COMMAND_RETURN_ARROW && protocolPtr[i].ID)
                counter++;
        }
        return counter;
    }

    HUSKYLENSResult get(int16_t index)
    {
        if (index < count())
        {
            return protocolPtr[index];
        }
        return resultDefault;
    }
    HUSKYLENSResult get(int16_t ID, int16_t index)
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].ID == ID)
                if (index == counter++)
                    return protocolPtr[i];
        }
        return resultDefault;
    }

    HUSKYLENSResult getBlock(int16_t index)
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].command == COMMAND_RETURN_BLOCK)
                if (index == counter++)
                    return protocolPtr[i];
        }
        return resultDefault;
    }
    HUSKYLENSResult getBlock(int16_t ID, int16_t index)
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].command == COMMAND_RETURN_BLOCK && protocolPtr[i].ID == ID)
                if (index == counter++)
                    return protocolPtr[i];
        }
        return resultDefault;
    }

    HUSKYLENSResult getArrow(int16_t index)
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].command == COMMAND_RETURN_ARROW)
                if (index == counter++)
                    return protocolPtr[i];
        }
        return resultDefault;
    }
    HUSKYLENSResult getArrow(int16_t ID, int16_t index)
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].command == COMMAND_RETURN_ARROW && protocolPtr[i].ID == ID)
                if (index == counter++)
                    return protocolPtr[i];
        }
        return resultDefault;
    }

    HUSKYLENSResult getLearned(int16_t index)
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].ID)
                if (index == counter++)
                    return protocolPtr[i];
        }
        return resultDefault;
    }
    HUSKYLENSResult getBlockLearned(int16_t index)
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].command == COMMAND_RETURN_BLOCK && protocolPtr[i].ID)
                if (index == counter++)
                    return protocolPtr[i];
        }
        return resultDefault;
    }
    HUSKYLENSResult getArrowLearned(int16_t index)
    {
        int16_t counter = 0;
        for (int i = 0; i < protocolInfo.protocolSize; i++)
        {
            if (protocolPtr[i].command == COMMAND_RETURN_ARROW && protocolPtr[i].ID)
                if (index == counter++)
                    return protocolPtr[i];
        }
        return resultDefault;
    }

    bool writeAlgorithm(protocolAlgorithm algorithmType)
    {
        Protocol_t protocol;
        protocol.algorithmType = algorithmType;
        protocolWriteRequestAlgorithm(protocol);
        return wait(COMMAND_RETURN_OK);
    }

    bool writeLearn(int ID)
    {
        Protocol_t protocol;
        protocol.requestID = ID;
        protocolWriteRequestLearn(protocol);
        return wait(COMMAND_RETURN_OK);
    }

    bool writeForget()
    {
        protocolWriteRequestForget();
        return wait(COMMAND_RETURN_OK);
    }

    bool writeSensor(int sensor0, int sensor1, int sensor2)
    {
        Protocol_t protocol;
        protocol.first = sensor0;
        protocol.second = sensor1;
        protocol.third = sensor2;
        protocolWriteRequestSensor(protocol);
        return wait(COMMAND_RETURN_OK);
    }

    bool setCustomName(String name,uint8_t id)
    {   
        const char* nameC = name.c_str();
        Protocol_t protocol;
        protocol.customNameHeader.nameDataSize=strlen(nameC);
        protocol.customNameHeader.id=id;
        if(protocol.customNameHeader.nameDataSize > 20){
            return false;
        }
        memcpy(protocol.customNameHeader.dataBuffer,nameC,protocol.customNameHeader.nameDataSize);
        protocolWriteRequestCustomNames(protocol);
        return wait(COMMAND_RETURN_OK);
    }

    bool savePictureToSDCard(){
        Protocol_t protocol;
        protocolWriteRequestPhoto(protocol);
        return wait(COMMAND_RETURN_OK);
    }

    bool saveModelToSDCard(int fileNum){
        Protocol_t protocol;
        protocol.first=fileNum;
        protocolWriteRequestSendKnowledges(protocol);
        return wait(COMMAND_RETURN_OK);
    }

    bool loadModelFromSDCard(int fileNum){
        Protocol_t protocol;
        protocol.first=fileNum;
        protocolWriteRequestReceiveKnowledges(protocol);
        return wait(COMMAND_RETURN_OK);
    }

    bool clearCustomText(){
        Protocol_t protocol;
        protocolWriteRequestClearText(protocol);
        return wait(COMMAND_RETURN_OK);
    }

    bool customText(String text,uint16_t x,uint8_t y){
        const char* textC = text.c_str();
        Protocol_t protocol;
        protocol.customText.textSize=strlen(textC);
        if(protocol.customText.textSize>20){
            return false;
        }
        protocol.customText.x=x;
        protocol.customText.y=y;
        memcpy(protocol.customText.text,textC,protocol.customText.textSize);
        protocolWriteRequestCustomText(protocol);
        return wait(COMMAND_RETURN_OK);
    }
    

    bool saveScreenshotToSDCard(){
        Protocol_t protocol;
        protocolWriteRequestSaveScreenshot(protocol);
        return wait(COMMAND_RETURN_OK);
    }

    bool isPro(){
        Protocol_t protocolRequest;
        Protocol_t protocolResonse;
        protocolWriteRequestIsPro(protocolRequest);
        wait(COMMAND_RETURN_INFO);
        if(protocolReadRequestIsPro(protocolResonse)){
            return protocolResonse.first;
        }else{
            return false;
        }
    }

    #define HUSKYLENS_FIRMWARE_VERSION "0.4.1"
    bool checkFirmwareVersion(){
       writeFirmwareVersion(HUSKYLENS_FIRMWARE_VERSION);
    }

     bool writeFirmwareVersion(String version)
    {
        Protocol_t protocol;
        uint8_t length = version.length();
        uint8_t data[length + 2] = {length};
        version.toCharArray((char *)data + 1, length + 1);
        protocol.firmwareVersion.length = length + 1;
        protocol.firmwareVersion.data = data;
        protocolWriteRequestFirmwareVersion(protocol);
        return wait(COMMAND_RETURN_OK);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

    void protocolWriteCommand(Protocol_t &protocol, uint8_t command)
    {
        protocol.command = command;
        uint8_t *buffer = husky_lens_protocol_write_begin(protocol.command);
        int length = husky_lens_protocol_write_end();
        protocolWrite(buffer, length);
    }

    bool protocolReadCommand(Protocol_t &protocol, uint8_t command)
    {
        if (husky_lens_protocol_read_begin(command))
        {
            protocol.command = command;
            husky_lens_protocol_read_end();
            return true;
        }
        else
        {
            return false;
        }
    }

    void protocolWriteFiveInt16(Protocol_t &protocol, uint8_t command)
    {
        protocol.command = command;
        uint8_t *buffer = husky_lens_protocol_write_begin(protocol.command);
        husky_lens_protocol_write_int16(protocol.first);
        husky_lens_protocol_write_int16(protocol.second);
        husky_lens_protocol_write_int16(protocol.third);
        husky_lens_protocol_write_int16(protocol.fourth);
        husky_lens_protocol_write_int16(protocol.fifth);
        int length = husky_lens_protocol_write_end();
        protocolWrite(buffer, length);
    }

    bool protocolReadFiveInt16(Protocol_t &protocol, uint8_t command)
    {
        if (husky_lens_protocol_read_begin(command))
        {
            protocol.command = command;
            protocol.first = husky_lens_protocol_read_int16();
            protocol.second = husky_lens_protocol_read_int16();
            protocol.third = husky_lens_protocol_read_int16();
            protocol.fourth = husky_lens_protocol_read_int16();
            protocol.fifth = husky_lens_protocol_read_int16();
            husky_lens_protocol_read_end();
            return true;
        }
        else
        {
            return false;
        }
    }

    void protocolWriteOneInt16(Protocol_t &protocol, uint8_t command)
    {
        protocol.command = command;
        uint8_t *buffer = husky_lens_protocol_write_begin(protocol.command);
        husky_lens_protocol_write_int16(protocol.first);
        int length = husky_lens_protocol_write_end();
        protocolWrite(buffer, length);
    }

    bool protocolReadOneInt16(Protocol_t &protocol, uint8_t command)
    {
        if (husky_lens_protocol_read_begin(command))
        {
            protocol.command = command;
            protocol.first = husky_lens_protocol_read_int16();
            husky_lens_protocol_read_end();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool protocolReadCustomNameHeader(Protocol_t &protocol, uint8_t command)
    {

        if (husky_lens_protocol_read_begin(command))
        {
            protocol.command = command;
            protocol.customNameHeader.id = husky_lens_protocol_read_uint8();
            protocol.customNameHeader.nameDataSize = husky_lens_protocol_read_uint8();
            for (int i = 0; i < protocol.customNameHeader.nameDataSize; i++)
            {
                if (i > 20)
                {
                    break;
                }
                protocol.customNameHeader.dataBuffer[i] = husky_lens_protocol_read_uint8();
            }
            protocol.customNameHeader.dataBuffer[20] = 0x00;
            husky_lens_protocol_read_end();
            return true;
        }
        else
        {
            return false;
        }
    }

    void protocolWriteCustomNameHeader(Protocol_t &protocol, uint8_t command)
    {
        protocol.command = command;
        uint8_t *buffer = husky_lens_protocol_write_begin(protocol.command);
        husky_lens_protocol_write_uint8(protocol.customNameHeader.id);
        husky_lens_protocol_write_uint8(protocol.customNameHeader.nameDataSize);
        for(int i=0;i<20;i++){
            husky_lens_protocol_write_uint8(protocol.customNameHeader.dataBuffer[i]);
        }
        husky_lens_protocol_write_uint8(0x0);
        int length = husky_lens_protocol_write_end();
        protocolWrite(buffer, length);
    }

    bool protocolReadReceivedKnowledges(Protocol_t &protocol, uint8_t command)
    {

        if (husky_lens_protocol_read_begin(command))
        {

            return true;
        }
        else
        {
            return false;
        }
    }

    bool protocolWriteReceivedKnowledges(Protocol_t &protocol, uint8_t command)
    {
        if (husky_lens_protocol_read_begin(command))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool protocolReadCustomTextRecv(Protocol_t &protocol, uint8_t command)
    {

        if (husky_lens_protocol_read_begin(command))
        {
            protocol.command = command;
            protocol.customText.textSize = husky_lens_protocol_read_uint8();
            protocol.customText.x = husky_lens_protocol_read_uint8() + husky_lens_protocol_read_uint8();
            protocol.customText.y = husky_lens_protocol_read_uint8();
            char buf[protocol.customText.textSize];
            for (int i = 0; i < protocol.customText.textSize; i++)
            {
                if (i > 20)
                {
                    break;
                }
                protocol.customText.text[i] = husky_lens_protocol_read_uint8();
            }
            protocol.customText.text[20] = 0x00;
            //Null terminate the string
            protocol.customText.text[protocol.customText.textSize] = 0x00;
            husky_lens_protocol_read_end();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool protocolWriteCustomTextRecv(Protocol_t &protocol, uint8_t command)
    {
        protocol.command = command;
        uint8_t *buffer = husky_lens_protocol_write_begin(protocol.command);
        husky_lens_protocol_write_uint8(protocol.customText.textSize);
        if(protocol.customText.x >= 255){
            husky_lens_protocol_write_uint8(0xFF);
        }else{
            husky_lens_protocol_write_uint8(0x00);
        }
        husky_lens_protocol_write_uint8(protocol.customText.x & 0xFF);
        husky_lens_protocol_write_uint8(protocol.customText.y);
        for(int i=0;i<protocol.customText.textSize;i++){
            husky_lens_protocol_write_uint8(protocol.customText.text[i]);
        }
        husky_lens_protocol_write_uint8(0x0);
        int length = husky_lens_protocol_write_end();
        protocolWrite(buffer, length);
        return true;
    }

    bool protocolReadFirmwareVersion(Protocol_t &protocol, uint8_t command)
    {

        if (husky_lens_protocol_read_begin(command))
        {
            protocol.command = command;
            protocol.firmwareVersion.length = husky_lens_protocol_read_uint8();
            char buf[protocol.firmwareVersion.length];
            for (int i = 0; i < protocol.firmwareVersion.length; i++)
            {
                if (i > 20)
                {
                    break;
                }
                protocol.firmwareVersion.data[i] = husky_lens_protocol_read_uint8();
            }
            protocol.firmwareVersion.data[20] = 0x00;
            protocol.firmwareVersion.data[protocol.firmwareVersion.length] = 0x00;
            husky_lens_protocol_read_end();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool protocolWriteFirmwareVersion(Protocol_t &protocol, uint8_t command)
    {
       protocol.command = command;
        uint8_t *buffer = husky_lens_protocol_write_begin(protocol.command);
        husky_lens_protocol_write_buffer_uint8(protocol.firmwareVersion.data, protocol.firmwareVersion.length);
        int length = husky_lens_protocol_write_end();
        for(int i=0; i<protocol.firmwareVersion.length; i++){
           Serial.println(protocol.firmwareVersion.data[i]);
        }
        protocolWrite(buffer, length);
        return true;
    }

#define PROTOCOL_CREATE(function, type, command)       \
    void protocolWrite##function(Protocol_t &protocol) \
    {                                                  \
        protocolWrite##type(protocol, command);        \
    }                                                  \
    void protocolWrite##function()                     \
    {                                                  \
        Protocol_t protocol;                           \
        protocolWrite##type(protocol, command);        \
    }                                                  \
    bool protocolRead##function(Protocol_t &protocol)  \
    {                                                  \
        return protocolRead##type(protocol, command);  \
    }                                                  \
    bool protocolRead##function()                      \
    {                                                  \
        Protocol_t protocol;                           \
        return protocolRead##type(protocol, command);  \
    }

    PROTOCOL_CREATE(Request, Command, COMMAND_REQUEST)
    PROTOCOL_CREATE(RequestBlocks, Command, COMMAND_REQUEST_BLOCKS)
    PROTOCOL_CREATE(RequestArrows, Command, COMMAND_REQUEST_ARROWS)

    PROTOCOL_CREATE(RequestLearned, Command, COMMAND_REQUEST_LEARNED)
    PROTOCOL_CREATE(RequestBlocksLearned, Command, COMMAND_REQUEST_BLOCKS_LEARNED)
    PROTOCOL_CREATE(RequestArrowsLearned, Command, COMMAND_REQUEST_ARROWS_LEARNED)

    PROTOCOL_CREATE(RequestByID, OneInt16, COMMAND_REQUEST_BY_ID)
    PROTOCOL_CREATE(RequestBlocksByID, OneInt16, COMMAND_REQUEST_BLOCKS_BY_ID)
    PROTOCOL_CREATE(RequestArrowsByID, OneInt16, COMMAND_REQUEST_ARROWS_BY_ID)

    PROTOCOL_CREATE(ReturnInfo, FiveInt16, COMMAND_RETURN_INFO)
    PROTOCOL_CREATE(ReturnBlock, FiveInt16, COMMAND_RETURN_BLOCK)
    PROTOCOL_CREATE(ReturnArrow, FiveInt16, COMMAND_RETURN_ARROW)

    PROTOCOL_CREATE(RequestKnock, Command, COMMAND_REQUEST_KNOCK)
    PROTOCOL_CREATE(RequestAlgorithm, OneInt16, COMMAND_REQUEST_ALGORITHM)

    PROTOCOL_CREATE(ReturnOK, Command, COMMAND_RETURN_OK)

    PROTOCOL_CREATE(RequestLearn, OneInt16, COMMAND_REQUEST_LEARN)
    PROTOCOL_CREATE(RequestForget, Command, COMMAND_REQUEST_FORGET)

    PROTOCOL_CREATE(RequestSensor, FiveInt16, COMMAND_REQUEST_SENSOR)

    PROTOCOL_CREATE(RequestCustomNames, CustomNameHeader, COMMAND_REQUEST_CUSTOMNAMES)
    PROTOCOL_CREATE(RequestPhoto, Command, COMMAND_REQUEST_PHOTO)
    PROTOCOL_CREATE(RequestSendKnowledges, OneInt16, COMMAND_REQUEST_SEND_KNOWLEDGES)
    PROTOCOL_CREATE(RequestReceiveKnowledges, OneInt16, COMMAND_REQUEST_RECEIVE_KNOWLEDGES)
    PROTOCOL_CREATE(RequestCustomText, CustomTextRecv, COMMAND_REQUEST_CUSTOM_TEXT)
    PROTOCOL_CREATE(RequestClearText, Command, COMMAND_REQUEST_CLEAR_TEXT)
    PROTOCOL_CREATE(RequestSaveScreenshot, Command, COMMAND_REQUEST_SAVE_SCREENSHOT)
    PROTOCOL_CREATE(RequestIsPro, OneInt16, COMMAND_REQUEST_IS_PRO)
    PROTOCOL_CREATE(RequestFirmwareVersion, FirmwareVersion, COMMAND_REQUEST_FIRMWARE_VERSION)

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
};

#endif
