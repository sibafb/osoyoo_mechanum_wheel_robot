/**
 * @file  watchSensorTest.cpp
 * @brief 
 *         周囲検知のデバッグモジュール
 * @date 2020/12/24 まずcでファイル分割をかく
 */
#include <Arduino.h>

#include "./../lib/debugPrint.h"
#include "./../lib/pinAssignment.h"

#include "./../hardWareDriver/ultraSonicSensor.h"
#include "./../hardWareDriver/servoDriver.h"

#include "functionsApi.h"



void watchSensorTest(){
  //servoWrite(120);
  //delay(100);
  int scanval = ultraSonicSensorWatch();
  delay(500);
  //DebugLogPrintln("scanval:" + String(scanval) + "[cm]");
}
typedef struct {
     int AREA;
     int VALUE;
}SCAN_DATA;

static int servoPosition = 90;
static const int AREA_DIVISION = 9;
static SCAN_DATA ScanArray[AREA_DIVISION];
void watchSurroundTest(){
  /*0,20,40,60,80,100,120,140,160*/
  /* init */
  ScanArray[0].AREA = 10; 
  for(int i=0; i < AREA_DIVISION;i++){
    ScanArray[i].AREA = ScanArray[0].AREA + i*20;//by 20 degree.
  }
  /* scan */
  if(getServoPosition() <= 90){
    for(int i=0; i < AREA_DIVISION;i++){
        servoSmoothWrite(ScanArray[i].AREA,5,1);
        delay(20);
        ScanArray[i].VALUE = ultraSonicSensorWatch();//by  degree.
        delay(20);
    }
  }else{
    for(int i= AREA_DIVISION-1 ; i >=0 ;i--){
        servoSmoothWrite(ScanArray[i].AREA,5,1);
        delay(20);
        ScanArray[i].VALUE = ultraSonicSensorWatch();//by  degree.
        delay(20);  
    }
  }
  /* output */
  DebugLogPrint("ScanValArray:L[");
  for(int i=0; i< AREA_DIVISION-1;i++ ){
    DebugLogPrint("\t"+String(ScanArray[i].VALUE)+",");
  }
  DebugLogPrint("\t"+String(ScanArray[AREA_DIVISION-1].VALUE)+" ");
  DebugLogPrintln("]R");
}

int getWatchSurroundData(int index){
  if(index < 0 || index > AREA_DIVISION - 1){
    return 0;
  }
  return ScanArray[index];
}

int getWatchSurroundDataSize(){
  return AREA_DIVISION;
}

void watchNextAreaDistance(){
  int index = swingIndex();

  servoSmoothWrite(ScanArray[index].AREA,5,1);
  delay(20);
  ScanArray[index].VALUE = ultraSonicSensorWatch();//by  degree.
  delay(20);
}

static int swingDirection = 0;
static int areaIndex = 0;
int swingIndex(){
  if(swingDirection == 0){
    swingDirection = 1;
    areaIndex = 0;
  }
  if(swingDirection == 1){
    areaIndex++;

    if(areaIndex == AREA_DIVISION - 1){
       swingDirection = -1;
    }
  }
  if(swingDirection == -1){
    areaIndex--;

    if(areaIndex == 0){
       swingDirection = 1;
    }
  }

  return areaIndex
}