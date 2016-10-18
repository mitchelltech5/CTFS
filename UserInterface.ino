/**
 * Copyright 2016; Mitchell Wagner, Adam Carlock and Kim Eagleston
 * as an unpublished work.  All rights reserved.
 * This Embedded Software was developed for CTFS or Chocolate 
 * Temper Flow System. CTFS Verison 1.0 Software Version 1.3
 * 
 * File Header:
 * This File contains all functions and data relating to the 
 * user interface
 * 
 * 
 * 
 */
 /**@file UserInterface.ino */

 #include "Arduino.h"
 #include <Time.h>   //time Libary

 //CURRENT STAGES OF PROCESSING
 #define OFF              0
 #define HEAT_T1          1
 #define HEAT_T2          2
 #define HEAT_SYS         3
 #define PURGE            4
 #define SD_READ          5
 #define FINAL_CHECK      6
 #define RELEASE_PRINTER  7
 #define PRINT_FINISHED   8
 #define FINISHED_QUESTION 9
 #define COOL_SYS         10
 #define COOL_T2          11
 #define COOL_T1          12
 #define CLEAN_TWV1       13
 #define CLEAN_TWV2       14

int CURRENT_STATUS=0;

 long UserInterfaceLast=now();
 boolean DoCommand=true;
 boolean AdvMenu=false;

/**
 * @brief SerialUserInterfaceStartup
 * @details startup for user frontend
 * @return none
 * @pre none
 * @post none
 */
 void SerialUserInterfaceStartup(){
  Serial.begin(9600);    //Usb Debug Start Serial Comm 9600 Baud
  Serial.println("WELCOME TO CTFS!");
  Serial.println("Copyright 2016; Mitchell Wagner, Adam Carlock and Kim Eagleston");
  Serial.println("as an unpublished work.  All rights reserved.");
  Serial.println("This Embedded Software was developed for CTFS or Chocolate");
  Serial.println("Temper Flow System. CTFS Verison 1.0 Software Version 1.3\n");
  Serial.println("               ***WARNING***WARNING***WARNING***               ");
  Serial.println("Please Note: Do NOT operate this machine unless you have had");
  Serial.println("training on operation. This system does get hot, WATCH where you");
  Serial.println("put your fingers, be very careful and follow all directions.");
  Serial.println("               ***WARNING***WARNING***WARNING***               \n\n");
 }

/**
 * @brief SerialUserInterface
 * @details main State Machine controled by user, reads input from user
 * @return none
 * @pre none
 * @post none
 */
 void SerialUserInterface(){
  if(!AdvMenu){
    switch(Serial.read()){
      case -1: break;
      case 48: {
                AdvMenu=true;
                break;
               }
      case 50: {
                DoCommand=true;
                break;
               }
      case 54: {
                if(CURRENT_STATUS!=RELEASE_PRINTER||CURRENT_STATUS!=CLEAN_TWV2){
                  DoCommand=true;
                  CURRENT_STATUS++; 
                }
                else if(CURRENT_STATUS==CLEAN_TWV2){
                  DoCommand=true;
                  CURRENT_STATUS=OFF;
                }
                  break;
               }
      case 52: {
                if(CURRENT_STATUS!=RELEASE_PRINTER||CURRENT_STATUS!=PRINT_FINISHED || CURRENT_STATUS!=FINISHED_QUESTION){
                  DoCommand=true;
                  CURRENT_STATUS--;
                  }
                else if(CURRENT_STATUS==RELEASE_PRINTER)
                  UserStopPrint();
                else if(CURRENT_STATUS==FINISHED_QUESTION)
                  CURRENT_STATUS=HEAT_SYS;
                break;
      }
    }
    Serial.println("Enter 6 to Start The Next Process");
    Serial.println("Enter 4 to go back to the Last Process");
    Serial.println("Enter 2 to Redo Current Process");
    Serial.println("Enter 0 to go to the ADV Menu Control");
  }
  else if(AdvMenu){
    Serial.println("ADVANCED MENU");
    Serial.println("Enter 0 to exit ADV Menu Control");
    Serial.println("Enter 1 to go to OFF STATE");
    Serial.println("Enter 2 to go to HEAT TANK 1 STATE");
    Serial.println("Enter 3 to go to HEAT TANK 2 STATE");
    Serial.println("Enter 4 to go to HEAT SYS STATE");
    Serial.println("Enter 5 to go to PURGE STATE");
    Serial.println("Enter 6 to go to SD READ STATE");
    Serial.println("Enter 7 to go to FINAL CHECK STATE");
    Serial.println("Enter 8 to go to RELEASE PRINTER STATE");
    Serial.println("Enter 9 to go to PRINT FINISHED STATE");
    Serial.println("Enter a to go to FINISHED QUESTION STATE");
    Serial.println("Enter b to go to COOL SYS STATE");
    Serial.println("Enter c to go to COOL T2 STATE");
    Serial.println("Enter d to go to COOL T1 STATE");
    Serial.println("Enter e to go to 3 WAY VALUE 0* STATE");
    Serial.println("Enter f to go to 3 WAY VALUE 90* STATE");
    switch(Serial.read()){
      case 48: {AdvMenu=false;                     break;}
      case 49: {CURRENT_STATUS=OFF;                break;}
      case 50: {CURRENT_STATUS=HEAT_T1;            break;}
      case 51: {CURRENT_STATUS=HEAT_T2;            break;}
      case 52: {CURRENT_STATUS=HEAT_SYS;           break;}
      case 53: {CURRENT_STATUS=PURGE;              break;}
      case 54: {CURRENT_STATUS=SD_READ;            break;}
      case 55: {CURRENT_STATUS=FINAL_CHECK;        break;}
      case 56: {CURRENT_STATUS=RELEASE_PRINTER;    break;}
      case 57: {CURRENT_STATUS=PRINT_FINISHED;     break;}
      case 97: {CURRENT_STATUS=FINISHED_QUESTION;  break;}
      case 98: {CURRENT_STATUS=COOL_SYS;           break;}
      case 99: {CURRENT_STATUS=COOL_T2;            break;}
      case 100: {CURRENT_STATUS=COOL_T1;           break;}
      case 101: {CURRENT_STATUS=CLEAN_TWV1;        break;}
      case 102: {CURRENT_STATUS=CLEAN_TWV2;        break;}
      
    }
  }

  
  if(now()>UserInterfaceLast+5)
  {
    switch(CURRENT_STATUS){
      case OFF:            {UserOff();            break;}
      case HEAT_T1:        {UserHeatT1();         break;}
      case HEAT_T2:        {UserHeatT2();         break;}
      case HEAT_SYS:       {UserHeatSys();        break;}
      case PURGE:          {UserPurge();          break;}
      case SD_READ:        {UserReadSd();         break;}
      case FINAL_CHECK:    {UserFinalCheck();     break;}
      case RELEASE_PRINTER:{UserReleasePrinter(); break;}
      case PRINT_FINISHED: {UserPrintFinished();  break;}
      case FINISHED_QUESTION: {UserFinishedQuestion(); break;}
      case COOL_SYS:       {UserCoolSys();        break;}
      case COOL_T2:        {UserCoolT2();         break;}
      case COOL_T1:        {UserCoolT1();         break;}
      case CLEAN_TWV1:     {UserTwvToOutput();    break;}
      case CLEAN_TWV2:     {UserTwvToTank();    break;}
            
      
    }
    
  }
  
 }



 /**
 * @brief UserCoolSys
 * @details Handles cool sys state 
 * @return none
 * @pre none
 * @post none
 */
 void UserCoolSys(){
  Serial.println("Cooling System...");
  Serial.println("Next Step -> Cool Tank 2");
  Serial.println("Prev Step <- Print Finished Question");
  if(!isTank1On())
    addError(TANK1_OFF);
  else
    removeError(TANK1_OFF);
  if(!isTank2On())
    addError(TANK2_OFF);
  else
    removeError(TANK2_OFF);
    
  if(DoCommand){
    setSyringe(false);
    setLine1(false);
    setLine2(false);
    setLine3(false);
    setPrinterRelease(false);
    DoCommand=false;
  }
 }

/**
 * @brief UserCoolT2
 * @details handles Cool T2 state
 * @return none
 * @pre none
 * @post none
 */
 void UserCoolT2(){
  Serial.println("Cooling Tank 2...");
  Serial.println("Next Step -> Cool Tank 1");
  Serial.println("Prev Step <- Cool System");
  if(!isTank1On())
    addError(TANK1_OFF);
  else
    removeError(TANK1_OFF);
  if(DoCommand){
    setTank2(false);
    setPrinterRelease(false);
    DoCommand=false;
  }
 }

 /**
 * @brief UserCoolT1
 * @details Handles Cool T1 state
 * @return none
 * @pre none
 * @post none
 */
 void UserCoolT1(){
  Serial.println("Cooling Tank 1...");
  Serial.println("Next Step -> Clean 3 way Value");
  Serial.println("Prev Step <- Cool Tank 2");
  if(DoCommand){
    setTank1(false);
    setPrinterRelease(false);
    DoCommand=false;
  }
  
 }

 /**
 * @brief UserTwvToOutput
 * @details Handles Cool T1 state
 * @return none
 * @pre none
 * @post none
 */
 void UserTwvToOutput(){
  Serial.println("Three Way Value Turned to Output (for Clean)");
  Serial.println("Next Step -> Clean 3 way Value (To Tank)");
  Serial.println("Prev Step <- Cool Tank 1");
  if(DoCommand){
    SwitchValueToOutput();
    DoCommand=false;
  }
  
 }
  /**
 * @brief UserTwvToTank
 * @details Handles Cool T1 state
 * @return none
 * @pre none
 * @post none
 */
 void UserTwvToTank(){
  Serial.println("Three Way Value Turned to Tank (for Clean)");
  Serial.println("Next Step -> OFF");
  Serial.println("Prev Step <- Clean 3 way Value (To Output)");
  if(DoCommand){
    SwitchValueToTank();
    DoCommand=false;
  }
  
 }

 void StatusFinishedPrint(){
  CURRENT_STATUS=PRINT_FINISHED;
 }
/**
 * @brief UserFinshedQuestion
 * @details Handles finised question state
 * @return none
 * @pre none
 * @post none
 */
 void UserFinishedQuestion(){
  Serial.println("***Print is Finished***");
  Serial.println("Next Step -> Cool System");
  Serial.println("Prev Step <- Print Again");
 }

 
 /**
 * @brief UserPrintFinshed
 * @details Handles Print finised state
 * @return none
 * @pre none
 * @post none
 */
 void UserPrintFinished(){
  Serial.println("***Print is Finished***");
  Serial.println("Next Step -> FINISH QUESTION");
  Serial.println("Prev Step <- NO GOING BACK NOW!!");

  if(!isTank1On())
    addError(TANK1_OFF);
  else
    removeError(TANK1_OFF);
  if(!isTank2On())
    addError(TANK2_OFF);
  else
    removeError(TANK2_OFF);
  if(!isSyringeOn())
    addError(SYRINGE_OFF);
  else
    removeError(SYRINGE_OFF);
  if(!isLine1On())
    addError(LINE1_OFF);
  else
    removeError(LINE1_OFF);
  if(!isLine2On())
    addError(LINE2_OFF);
  else
    removeError(LINE2_OFF);
  if(!isLine3On())
    addError(LINE3_OFF);
  else
    removeError(LINE3_OFF);
  
  if(DoCommand){
    setPrinterRelease(false);
    resetCurrentLayer();
    DoCommand=false;
  }
 }

/**
 * @brief UserStopPrint
 * @details if user stops print printer will reset to be able to start a new print
 * @return none
 * @pre none
 * @post none
 */
 void UserStopPrint(){
  Serial.println("STOPPING PRINT");
  pumpstopNE();
  setPrinterRelease(false);
  resetCurrentLayer();
  DoCommand=true;
  CURRENT_STATUS=HEAT_SYS;
 }

/**
 * @brief UserReleasePrinter
 * @details handles Released printer 
 * @return none
 * @pre none
 * @post none
 */
 void UserReleasePrinter(){
  Serial.println("***Printer is Released!***");
  Serial.println("Next Step -> Wait For Print To Finish!");
  Serial.println("Prev Step <- Heat All (Will Stop Print **UNRECOVERABLE PRINT**)");

  if(!isTank1On())
    addError(TANK1_OFF);
  else
    removeError(TANK1_OFF);
  if(!isTank2On())
    addError(TANK2_OFF);
  else
    removeError(TANK2_OFF);
  if(!isSyringeOn())
    addError(SYRINGE_OFF);
  else
    removeError(SYRINGE_OFF);
  if(!isLine1On())
    addError(LINE1_OFF);
  else
    removeError(LINE1_OFF);
  if(!isLine2On())
    addError(LINE2_OFF);
  else
    removeError(LINE2_OFF);
  if(!isLine3On())
    addError(LINE3_OFF);
  else
    removeError(LINE3_OFF);
  if(getZLayer(0)==0)
    addError(SD_DATA_NF);
  else
    removeError(SD_DATA_NF);
  if(DoCommand){
    setPrinterRelease(true);
    Serial.println("Release Complete");
    DoCommand=false;
  }
 }

/**
 * @brief UserFinalCheck
 * @details Handles final check state
 * @return none
 * @pre none
 * @post none
 */
 void UserFinalCheck(){
  Serial.println("FINAL CHECK BEFORE PRINTER IS RELEASED");
  Serial.println("Check system make sure build plate is installed");
  Serial.println("and nozzile is in position, check the error log");
  Serial.println("and verify printer is ready for printing\n");
  Serial.println("Next Step -> Release Control to printer");
  Serial.println("Prev Step <- Read Sd Card");

   if(!isTank1On())
    addError(TANK1_OFF);
  else
    removeError(TANK1_OFF);
  if(!isTank2On())
    addError(TANK2_OFF);
  else
    removeError(TANK2_OFF);
  if(!isSyringeOn())
    addError(SYRINGE_OFF);
  else
    removeError(SYRINGE_OFF);
  if(!isLine1On())
    addError(LINE1_OFF);
  else
    removeError(LINE1_OFF);
  if(!isLine2On())
    addError(LINE2_OFF);
  else
    removeError(LINE2_OFF);
  if(!isLine3On())
    addError(LINE3_OFF);
  else
    removeError(LINE3_OFF);
  if(getZLayer(0)==0)
    addError(SD_DATA_NF);
  else
    removeError(SD_DATA_NF);
    
 
 }
 
/**
 * @brief UserReadSd
 * @details Reads sd card state
 * @return none
 * @pre none
 * @post none
 */
 void UserReadSd(){
  Serial.println("Reading Sd Card...");
  Serial.println("Next Step -> Final Check Before Print");
  Serial.println("Prev Step <- Purge");

  if(DoCommand){
    processSd();
    Serial.println("Read Complete");
    setPrinterRelease(false);
    DoCommand=false;
  }
  
 }

/**
 * @brief UserPurge
 * @details Handles purge state
 * @return none
 * @pre none
 * @post none
 */
 void UserPurge(){
  Serial.println("Purging...");
  Serial.println("Next Step -> Read Sd Card");
  Serial.println("Prev Step <- Heat Rest Of System");
  if(!isTank1On())
    addError(TANK1_OFF);
  else
    removeError(TANK1_OFF);
  if(!isTank2On())
    addError(TANK2_OFF);
  else
    removeError(TANK2_OFF);
  if(!isSyringeOn())
    addError(SYRINGE_OFF);
  else
    removeError(SYRINGE_OFF);
  if(!isLine1On())
    addError(LINE1_OFF);
  else
    removeError(LINE1_OFF);
  if(!isLine2On())
    addError(LINE2_OFF);
  else
    removeError(LINE2_OFF);
  if(!isLine3On())
    addError(LINE3_OFF);
  else
    removeError(LINE3_OFF);

  if(DoCommand){
    purgeSequence();
    Serial.println("Purge Complete");
    setPrinterRelease(false);
    DoCommand=false;
  }
 }

/**
 * @brief UserHeatSys
 * @details Handles heat sys state
 * @return none
 * @pre none
 * @post none
 */
 void UserHeatSys(){
  Serial.println("Heating System...");
  Serial.println("Next Step -> Purge");
  Serial.println("Prev Step <- Heat Tank 2 (Cool Rest)");
  if(!isTank1On())
    addError(TANK1_OFF);
  else
    removeError(TANK1_OFF);
  if(!isTank2On())
    addError(TANK2_OFF);
  else
    removeError(TANK2_OFF);
    
  if(DoCommand){
    setSyringe(true);
    setLine1(true);
    setLine2(true);
    setLine3(true);
    setPrinterRelease(false);
    DoCommand=false;
  }
 }

/**
 * @brief UserHeatT2
 * @details handles Heat T2 state
 * @return none
 * @pre none
 * @post none
 */
 void UserHeatT2(){
  Serial.println("Heating Tank 2...");
  Serial.println("Next Step -> Heat Rest of System");
  Serial.println("Prev Step <- Heat Tank 1 (Cool Rest)");
  if(!isTank1On())
    addError(TANK1_OFF);
  else
    removeError(TANK1_OFF);
  if(DoCommand){
    setTank2(true);
    setSyringe(false);
    setLine1(false);
    setLine2(false);
    setLine3(false);
    setChamber(false);
    setPrinterRelease(false);
    DoCommand=false;
  }
 }

 /**
 * @brief UserHeatT1
 * @details Handles Heat T1 state
 * @return none
 * @pre none
 * @post none
 */
 void UserHeatT1(){
  Serial.println("Heating Tank 1...");
  Serial.println("Next Step -> Heat Tank 2");
  Serial.println("Prev Step <- OFF (Cool All)");
  if(DoCommand){
    setTank1(true);
    setTank2(false);
    setSyringe(false);
    setLine1(false);
    setLine2(false);
    setLine3(false);
    setChamber(false);
    setPrinterRelease(false);
    DoCommand=false;
  }
  
 }

/**
 * @brief UserOff
 * @details Handles Off State
 * @return none
 * @pre none
 * @post none
 */
 void UserOff(){
  Serial.println("All Systems are Off Waiting for commands");
  Serial.println("Next Step -> Heat Tank 1");
  Serial.println("Prev Step <- None");
  if(DoCommand){
    setTank1(false);
    setTank2(false);
    setSyringe(false);
    setLine1(false);
    setLine2(false);
    setLine3(false);
    setChamber(false);
    setPrinterRelease(false);
    DoCommand=false;
  }

 }
   
