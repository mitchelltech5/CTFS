/**
 * Copyright 2016; Mitchell Wagner, Adam Carlock and Kim Eagleston
 * as an unpublished work.  All rights reserved.
 * This Embedded Software was developed for CTFS or Chocolate 
 * Temper Flow System. CTFS Verison 1.0 Software Version 1.3
 * 
 * File Header:
 * This File contains all functions and data relating to the 
 * Output of data via Serial Display at 9600 baud 
 * 
 */
/**@file SerialDisplay.ino */
 #include "Arduino.h"
 #include <TimeLib.h>

 
  //CONTROL VARIABLES
  long lastTimeDisplay;              //timing display
  String DeviceBuffer[7];
  String VolumeToMoveBuffer;
  //CONTROL VARIABLES

/**
 * @brief SerialSetup
 * @details Setup serial connection
 * @return none
 * @pre none
 * @post none
 */
 void SerialSetup(){
    Serial.begin(9600);
    lastTimeDisplay=now();
    for(int x=0; x<7; x++)
    DeviceBuffer[x]="";
    VolumeToMoveBuffer="";
 }

/**
 * @brief DisplayInfo
 * @details Displays all information at specifed time intervals
 * @return none
 * @pre none
 * @post none
 */
 void SerialDisplayInfo(){
  if(now()>lastTimeDisplay+5){
    lastTimeDisplay=now();

    if(VolumeToMoveBuffer!=""){
      Serial.print("Volume To Move:  ");
      Serial.println(VolumeToMoveBuffer);
      VolumeToMoveBuffer="";
    }
    if(isCharging())
      Serial.println("SYRINGE IS FILLING");
    if(isDischarging())
      Serial.println("SYRINGE IS EMPTYING");

      
    for(int x=0; x<7; x++)
      Serial.println(DeviceBuffer[x]);
      
    SerialDisplayErrors();
  }
 }

/**
 * @brief SerialDisplayDevice
 * @details Captures information about specific heating 
 * device to display info
 * @return none
 * @pre none
 * @post none
 */
 void SerialDisplayDevice(String x,String control, float inside, double output, double element, int target){
  int device=0;
  if(x=="Tank 1") device=0;
  else if (x=="Tank 2") device=1;
  else if (x=="Syringe") device=2;
  else if (x=="Line 1") device=3;
  else if (x=="Line 2") device=4;
  else if (x=="Line 3") device=5;
  else if (x=="Chamber") device=6;
  DeviceBuffer[device]="";
  DeviceBuffer[device]=x+"    ELEMENT:  "+control+"\n"+x;
 

  char Buffer[100];
  if(device<1)
  sprintf(Buffer, "        Inside: %f*F  Target: %f*F  Output: %dPWM  Element Temp: %f*F\n", 
  inside, target, output, element);
  else
  sprintf(Buffer, "  Element Temp: %f*F  Target: %f*F  Output: %dPWM  \n", 
  element, target,output);

  DeviceBuffer[device] +=Buffer;

 }

/**
 * @brief SerialDisplayErrors
 * @details Displays information about any and all errors
 * device to display info
 * @return none
 * @pre none
 * @post none
 */
 void SerialDisplayErrors(){
  boolean ErrorFound=false;
  int errorsbuffer[sizeof(errors)];
  for(int x=0; x<sizeof(errors); x++){
    if(errors[x]){
      ErrorFound=true;
      errorsbuffer[x]=x;
    }
    else
      errorsbuffer[x]=-1;
  }

  if(ErrorFound){
  Serial.println("***WARNING***WARNING***WARNING***");
  for(int x=0; x<sizeof(errorsbuffer); x++)
    switch(errorsbuffer[x]){
      case 0: Serial.println("**TANK 1 ELEMENT SAFETY**"); break;
      case 1: Serial.println("**TANK 2 ELEMENT SAFETY**"); break;
      case 2: Serial.println("**SYRINGE ELEMENT SAFETY**"); break;
      case 3: Serial.println("**LINE 1 ELEMENT SAFETY**"); break;
      case 4: Serial.println("**LINE 2 ELEMENT SAFETY**"); break;
      case 5: Serial.println("**LINE 3 ELEMENT SAFETY**"); break;
      case 6: Serial.println("**CHAMBER ELEMENT SAFETY**"); break;
      case 7: Serial.println("**ABOVE MAX VOLUME COMMAND PROCESSED FOR SYRINGE**"); break;
      case 8: Serial.println("**BELOW ZERO VOLUME COMMAND PROCESSED FOR SYRINGE**"); break;
      case 9: Serial.println("**NO PUMP RESPONSE**"); break;
      case 10: Serial.println("**[[FATAL]] PUMP STALL**"); break;
      case 11: Serial.println("**[[FATAL]] EMERGENCY STOP**"); break;
      case 12: Serial.println("**[[FATAL]] SD CARD NOT FOUND**"); break;
      case 13: Serial.println("**SD CARD FILE NOT FOUND**"); break;
      case 14: Serial.println("**TANK 1 HEAT IS OFF**"); break;
      case 15: Serial.println("**TANK 2 HEAT IS OFF**"); break;
      case 16: Serial.println("**SYRINGE HEAT IS OFF**"); break;
      case 17: Serial.println("**LINE 1 HEAT IS OFF**"); break;
      case 18: Serial.println("**LINE 2 HEAT IS OFF**"); break;
      case 19: Serial.println("**LINE 3 HEAT IS OFF**"); break;
      case 20: Serial.println("**SD DATA IS NOT FOUND RE-READ SD CARD**"); break;
      default: Serial.println("UNKNOWN ERROR");  break;
    }
  }
  Serial.println("***WARNING***WARNING***WARNING***");
}

/**
 * @brief SerialDisplayVolumeToMove
 * @details saves Volume that pump will charge or discharge to a buffer to be displayed later
 * @param [in] numbers - volume to move 
 * @return none
 * @pre none
 * @post none
 */
void SerialDisplayVolumeToMove(int numbers[4]){

  char Buffer[100];
  sprintf(Buffer, "%d %d %d %d", 
  numbers[3], numbers[2], numbers[1], numbers[0]);

  VolumeToMoveBuffer=Buffer;
}

/**
 * @brief SerialDisplayCurrentVolume
 * @details saves Volume to a buffer to be displayed later
 * @param [in] volume - current volume in syringe
 * @return none
 * @pre none
 * @post none
 */
void SerialDisplayCurrentVolume(int volume){

  char Buffer[100];
  sprintf(Buffer, "Current Volume After Move:  %d", 
  volume);

  VolumeToMoveBuffer +=Buffer;
}


