/**
 * Copyright 2016; Mitchell Wagner, Adam Carlock and Kim Eagleston
 * as an unpublished work.  All rights reserved.
 * This Embedded Software was developed for CTFS or Chocolate 
 * Temper Flow System. CTFS Verison 1.0 Software Version 1.3
 * 
 * File Header:
 * This File contains all functions and data relating to the 
 * Error Logging Functionality
 * 
 * 
 */
/**@file ErrorLogging.ino */
 #include "Arduino.h"

//POSSIBLE ERRORS
#define TANK1_SAFETY      0
#define TANK2_SAFETY      1
#define SYRINGE_SAFETY    2
#define LINE1_SAFETY      3
#define LINE2_SAFETY      4
#define LINE3_SAFETY      5
#define CHAMBER_SAFETY    6
#define ABOVE_MAX_VOLUME  7
#define NEG_VOLUME        8
#define PUMP_NO_RESPONSE  9
#define FATAL_PUMP_STALL  10
#define FATAL_EMERGENCY_STOP 11
#define FATAL_SD_CARD_NF  12
#define SD_CARD_FILE_NF   13
#define TANK1_OFF         14
#define TANK2_OFF         15
#define SYRINGE_OFF       16
#define LINE1_OFF         17
#define LINE2_OFF         18
#define LINE3_OFF         19
#define SD_DATA_NF        20



boolean errors[21];

boolean isError(){
  boolean error=false;
  for(int x=0; x<sizeof(errors); x++)
  error |= errors[x];
  return error;
}


void addError(int error){
  errors[error]=true;
}

void removeError(int error){
 errors[error]=false;
}
