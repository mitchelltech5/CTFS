/**
 * Copyright 2016; Mitchell Wagner, Adam Carlock and Kim Eagleston
 * as an unpublished work.  All rights reserved.
 * This Embedded Software was developed for CTFS or Chocolate 
 * Temper Flow System. CTFS Verison 1.0 Software Version 1.3
 * 
 * File Header:
 * This File contains all functions and data relating to the 
 * Stiring systems for Tanks 1 and 2 
 * 
 */
 /**@file Stiring.ino */

/**
 * Wiring for Motors:
 * 
 * pin 33 Tank 1 Servo
 * pin 34 Tank 2 Servo
 */

#include "Arduino.h"
#include <Servo.h>
#include <TimeLib.h>


#define Servo1 33
#define Servo2 34

//CONTROL VARIABLES

long lastTimeStir;              //timing for agitate
boolean agitate;            //control for agiiate
//CONTROL VARIABLES

Servo TK1;         //Servo for Tank 1
Servo TK2;         //Servo for Tank 2

/**
 * @brief StirSetup
 * @details Stiring setup
 * @return none
 * @pre none
 * @post All stiring servos will be initilized
 */
void StirSetup(){

  TK1.attach(33);    //Tank 1 Servo Motor Attach
  TK2.attach(34);    //Tank 2 Servo Motor Attach
  lastTimeStir=now();
  agitate=true;
}


/**
 * @brief agitateTanks
 * @details will periodicaly agitate tanks based on time
 * @return none
 * @pre none
 * @post All stiring servos will be agaited at timing intervals
 */
void agitateTanks(){
  if(now()>lastTimeStir+60)
  {
     agitate=!agitate;
     lastTimeStir=now();
  }

  if(agitate)
  {
    if(isTank1On())
      TK1.write(0);
    else
      TK1.write(90);

    if(isTank2On())
      TK2.write(0);
    else
      TK2.write(90);
  }
}

