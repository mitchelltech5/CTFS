/**
 * Copyright 2016; Mitchell Wagner, Adam Carlock and Kim Eagleston
 * as an unpublished work.  All rights reserved.
 * This Embedded Software was developed for CTFS or Chocolate 
 * Temper Flow System. CTFS Verison 1.0 Software Version 1.3
 * 
 * File Header:
 * This File contains all functions and data relating to the 
 * Three Way Value and Chocolate Transfer
 * 
 * Wiring for Three Way Value:
 * 
 * pin 32 3 way Servo
 * 
 */
 /**@file ThreeWayValue.ino */

  #include "Arduino.h"
  
  #define TWV  32
  
  Servo way3;        //Servo for 3 Way valve
  
/**
 * @brief ThreeWayValueSetup
 * @details setsp up three way value for startup
 * @return none
 * @pre none
 * @post none
 */
  void ThreeWayValueSetup(){
    SwitchValueToOutput();
  }


/**
 * @brief SwitchValueToOutput
 * @details switch 3 way value to 0 degrees
 * @return none
 * @pre none
 * @post none
 */
  void  SwitchValueToOutput(){
    way3.attach(TWV);
    way3.write(0);
    delay(1000);
    way3.detach();
    delay(15);
  }
/**
 * @brief SwitchValueToTank
 * @details switch 3 way value to 90 degrees
 * @return none
 * @pre none
 * @post none
 */
  void  SwitchValueToTank(){
    way3.attach(TWV);
    way3.write(90);
    delay(1000);
    way3.detach();
    delay(15);
  }
