#include "HUKSYLENSMindPlus.h"
#include "SoftwareSerial.h"

HUKSYLENSMindPlus huskylensMindPlus;

void setup() {
    Serial.begin(115200);
    huskylensMindPlus.beginI2CUntilSuccess();     //使用I2C初始化, 直到成功
    // huskylensMindPlus.beginSoftwareSerialUntilSuccess(10, 11);  //使用软串口RX(10) TX(11) 初始化, 直到成功

    huskylensMindPlus.writeAlgorithm(ALGORITHM_FACE_RECOGNITION);   //切换到[人脸识别]算法, 直到成功
    // huskylensMindPlus.writeAlgorithm(ALGORITHM_OBJECT_TRACKING);         //物体追踪
    // huskylensMindPlus.writeAlgorithm(ALGORITHM_OBJECT_RECOGNITION);      //物体识别
    // huskylensMindPlus.writeAlgorithm(ALGORITHM_LINE_TRACKING);           //巡线
    // huskylensMindPlus.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION);       //颜色识别
    // huskylensMindPlus.writeAlgorithm(ALGORITHM_TAG_RECOGNITION);         //标签识别
}

void loop() {
    huskylensMindPlus.request(); //请求一次数据存入结果
    Serial.println(F("###################################"));

    int ID1 = 1;
    if(huskylensMindPlus.isLearned(ID1)) //从结果中获取ID(1)是否已学习?
    { 
        if (huskylensMindPlus.isAppearDirect(HUSKYLENSResultBlock)) //从结果中获取[方框]是否在画面中?
        {
            HUSKYLENSBlockDirectInfo blockResultDirect= huskylensMindPlus.readBlockCenterParameterDirect(); //从结果中获取靠中心方框的参数[X中心, Y中心, 宽度, 高度, ID]
            Serial.println(String()+F("blockNearCenter:xCenter=")+blockResultDirect.xCenter+F(",yCenter=")+blockResultDirect.yCenter+F(",width=")+blockResultDirect.width+F(",height=")+blockResultDirect.height+F(",ID=")+blockResultDirect.ID);
        }
        else{
            Serial.println(F("Block not appeared!"));//如果不存在就读取, 返回-1
            HUSKYLENSBlockDirectInfo blockResultDirect= huskylensMindPlus.readBlockCenterParameterDirect(); //从结果中获取靠中心方框的参数[X中心, Y中心, 宽度, 高度, ID]
            Serial.println(String()+F("blockNearCenter:xCenter=")+blockResultDirect.xCenter+F(",yCenter=")+blockResultDirect.yCenter+F(",width=")+blockResultDirect.width+F(",height=")+blockResultDirect.height+F(",ID=")+blockResultDirect.ID);
        }

        if (huskylensMindPlus.isAppearDirect(HUSKYLENSResultArrow))//从结果中获取[箭头]是否在画面中?
        {
            HUSKYLENSArrowDirectInfo arrowResultDirect = huskylensMindPlus.readArrowCenterParameterDirect(); //从结果中获取靠中心箭头的参数[X起点, Y起点, X终点, Y终点, ID]
            Serial.println(String()+F("arrowNearCenter:xOrigin=")+arrowResultDirect.xOrigin+F(",yOrigin=")+arrowResultDirect.yOrigin+F(",xTarget=")+arrowResultDirect.xTarget+F(",yTarget=")+arrowResultDirect.yTarget+F(",ID=")+arrowResultDirect.ID);
        }
        else
        {
            Serial.println(F("Arrrow not appeared!"));//如果不存在就读取, 返回-1
            HUSKYLENSArrowDirectInfo arrowResultDirect = huskylensMindPlus.readArrowCenterParameterDirect(); //从结果中获取靠中心箭头的参数[X起点, Y起点, X终点, Y终点, ID]
            Serial.println(String()+F("arrowNearCenter:xOrigin=")+arrowResultDirect.xOrigin+F(",yOrigin=")+arrowResultDirect.yOrigin+F(",xTarget=")+arrowResultDirect.xTarget+F(",yTarget=")+arrowResultDirect.yTarget+F(",ID=")+arrowResultDirect.ID);
        }
        
        if (huskylensMindPlus.isAppear(ID1, HUSKYLENSResultBlock)) //从结果中获取ID(1)[方框]是否在画面中?
        {
            HUSKYLENSBlockInfo blockResult = huskylensMindPlus.readBlockParameter(ID1); //从结果中获取ID(1)方框的参数[X中心, Y中心, 宽度, 高度]
            Serial.println(String()+F("Block:xCenter=")+blockResult.xCenter+F(",yCenter=")+blockResult.yCenter+F(",width=")+blockResult.width+F(",height=")+blockResult.height);
        }
        else {
            Serial.println(F("Block not appeared!"));//如果不存在就读取, 返回-1
            HUSKYLENSBlockInfo blockResult = huskylensMindPlus.readBlockParameter(ID1); //从结果中获取ID(1)方框的参数[X中心, Y中心, 宽度, 高度]
            Serial.println(String()+F("Block:xCenter=")+blockResult.xCenter+F(",yCenter=")+blockResult.yCenter+F(",width=")+blockResult.width+F(",height=")+blockResult.height);
        }

        if (huskylensMindPlus.isAppear(ID1, HUSKYLENSResultArrow)) //从结果中获取ID(1)[箭头]是否在画面中?
        {
            HUSKYLENSArrowInfo arrowResult = huskylensMindPlus.readArrowParameter(ID1); //从结果中获取ID(1)箭头的参数[X起点, Y起点, X终点, Y终点]
            Serial.println(String()+F("Arrow:xOrigin=")+arrowResult.xOrigin+F(",yOrigin=")+arrowResult.yOrigin+F(",xTarget=")+arrowResult.xTarget+F(",yTarget=")+arrowResult.yTarget);
        }
        else {
            Serial.println(F("Arrow not appeared!"));//如果不存在就读取, 返回-1
            HUSKYLENSArrowInfo arrowResult = huskylensMindPlus.readArrowParameter(ID1); //从结果中获取ID(1)箭头的参数[X起点, Y起点, X终点, Y终点]
            Serial.println(String()+F("Arrow:xOrigin=")+arrowResult.xOrigin+F(",yOrigin=")+arrowResult.yOrigin+F(",xTarget=")+arrowResult.xTarget+F(",yTarget=")+arrowResult.yTarget);
        }
    
        float learnedIDcount = huskylensMindPlus.readLearnedIDCount(); //从结果中获取已学ID总数
        Serial.println(String("learnedIDcount:")+learnedIDcount);

        float blockCount = huskylensMindPlus.readCount(HUSKYLENSResultBlock);       //从结果中获取[方框]总数
        Serial.println(String("blockCount:")+blockCount);
        float arrowCount = huskylensMindPlus.readCount(HUSKYLENSResultArrow);                   //[箭头]
        Serial.println(String("arrowCount:")+arrowCount);

        for (int i = 1; i <= blockCount; i++)
        {
            HUSKYLENSBlockDirectInfo blockResultMultiDirect= huskylensMindPlus.readBlockParameterDirect(i); //从结果中获取第(1)个方框的参数[X中心, Y中心, 宽度, 高度, ID]
            Serial.println(String()+F("blockMulti:xCenter=")+blockResultMultiDirect.xCenter+F(",yCenter=")+blockResultMultiDirect.yCenter+F(",width=")+blockResultMultiDirect.width+F(",height=")+blockResultMultiDirect.height+F(",ID=")+blockResultMultiDirect.ID);
        }
        for (int i = 1; i <= arrowCount; i++)
        {
            HUSKYLENSArrowDirectInfo arrowResultMultiDirect = huskylensMindPlus.readArrowParameterDirect(i); //从结果中获取第(1)个箭头的参数[X起点, Y起点, X终点, Y终点, ID]
            Serial.println(String()+F("arrowMulti:xOrigin=")+arrowResultMultiDirect.xOrigin+F(",yOrigin=")+arrowResultMultiDirect.yOrigin+F(",xTarget=")+arrowResultMultiDirect.xTarget+F(",yTarget=")+arrowResultMultiDirect.yTarget+F(",ID=")+arrowResultMultiDirect.ID);
        }

        float blockID1Count = huskylensMindPlus.readCount(ID1, HUSKYLENSResultBlock);       //从结果中获取ID(1)[方框]总数
        Serial.println(String("blockID1Count:")+blockID1Count);
        float arrowID1Count = huskylensMindPlus.readCount(ID1, HUSKYLENSResultArrow);                       //[箭头]
        Serial.println(String("arrowID1Count:")+arrowID1Count);

        for (int i = 1; i <= blockID1Count; i++)
        {
            HUSKYLENSBlockInfo blockResultMulti= huskylensMindPlus.readBlockParameter(ID1, i); //从结果中获取ID(1)第(1)个方框的参数[X中心, Y中心, 宽度, 高度]
            Serial.println(String()+F("blockMulti:xCenter=")+blockResultMulti.xCenter+F(",yCenter=")+blockResultMulti.yCenter+F(",width=")+blockResultMulti.width+F(",height=")+blockResultMulti.height);
        }
        for (int i = 1; i <= arrowID1Count; i++)
        {
            HUSKYLENSArrowInfo arrowResultMulti = huskylensMindPlus.readArrowParameter(ID1, i); //从结果中获取ID(1)第(1)个箭头的参数[X起点, Y起点, X终点, Y终点]
            Serial.println(String()+F("arrowMulti:xOrigin=")+arrowResultMulti.xOrigin+F(",yOrigin=")+arrowResultMulti.yOrigin+F(",xTarget=")+arrowResultMulti.xTarget+F(",yTarget=")+arrowResultMulti.yTarget);
        }
    }
    else
    {
        Serial.println(F("Not learned!"));
    }
}
