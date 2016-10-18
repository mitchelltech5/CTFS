/**
 * Copyright 2016; Mitchell Wagner, Adam Carlock and Kim Eagleston
 * as an unpublished work.  All rights reserved.
 * This Embedded Software was developed for CTFS or Chocolate 
 * Temper Flow System. CTFS Verison 1.0 Software Version 1.3
 * 
 * File Header:
 * This File contains all functions and data relating to the 
 * PurgeSequence
 * 
 */
 /**@file PurgeSequence.ino */

 #include "Arduino.h"

 #define PURGE_AMOUNT 2000  //ul

/**
 * @brief purgeSequence
 * @details purges the pump a specifed amount
 * @return none
 * @pre none
 * @post none
 */
 void purgeSequence(){

  Serial.println("purging now");
   SwitchValueToTank();
   pumpchg(PURGE_AMOUNT);
   delay(10);
   while(isPumpDone()==false) { PumpWaitLoop(); waitcase(); }

   
   SwitchValueToOutput();
   pumpdis(PURGE_AMOUNT);
   delay(10);
   while(isPumpDone()==false) { PumpWaitLoop(); waitcase(); }
   
 }




