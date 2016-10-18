/**
 * Copyright 2016; Mitchell Wagner, Adam Carlock and Kim Eagleston
 * as an unpublished work.  All rights reserved.
 * This Embedded Software was developed for CTFS or Chocolate 
 * Temper Flow System. CTFS Verison 1.0 Software Version 1.3
 * 
 * File Header:
 * This File contains all functions and data relating to the 
 * 3D Printer Control
 * 
 * Wiring To 3D Printer:
 * 
 * pin 22 R1 3d printer comm (Receive)
 * pin 23 R2 3d printer comm (Receive)
 * pin 24 R3 3d printer comm (Receive)
 * pin 25 tx 3d printer comm (Transmit)
 * 
 */
 /**@file PrinterControl.ino */

   #include "Arduino.h"

   //CONTROL VARIABLES

    #define RX1 22
    #define RX2 23
    #define RX3 24
    #define TX1 25

    boolean printerReleased=false;
    boolean autoHomeTrigger=false;
    int currentLayer=0;
   //CONTROL VARIABLES

/**
 * @brief resetCurrentLayer
 * @details In case of a printer reset or print reset, reset layer count
 * @return none
 * @pre none
 * @post none
 */
  void resetCurrentLayer(){
    currentLayer=0;
  }
  
/**
 * @brief PrinterInterface
 * @details Reads signal lines from printer determines message type and processes message
 * @code
 * 1-Printer startup hold
 * 2-Every Layer Pause
 * 3-Autohome (happens before print either manualy or automaticly in print)
 * @endcode
 * @return none
 * @pre none
 * @post none
 */
  void PrinterInterface(){
    switch(readPrinter()){
      case 1: { releasePrinter(); break;}
      case 2: { InterLayerComm(); break;}
      case 3: { if(printerReleased&&currentLayer<getMaxLayer()) releasePrinter(); else autoHomeTrigger=true; break;}
      case -1: break;
    }
    if(currentLayer==getMaxLayer())
      StatusFinishedPrint();
  }



  void setPrinterRelease(boolean value){
    printerReleased=value;
  }

  boolean isprinterReleased(){
    return printerReleased;
  }

/**
 * @brief AutoHome
 * @details allows printer to process an autohome 
 * @return none
 * @pre only if autohome signal has been sent and the printer is released
 * @post none
 */
  void AutoHome(){
    if(autoHomeTrigger)
      releasePrinter();
  }

/**
 * @brief releasePrinter
 * @details sends control signal to the printer to release it and continue movement/printing
 * @return none
 * @pre printer must be released
 * @post none
 */
  void releasePrinter(){

    if(printerReleased){
      digitalWrite(TX1, HIGH);
      while(digitalRead(RX3)==HIGH);
      digitalWrite(TX1, LOW);
    }
  }

/**
 * @brief readPrinter
 * @details Reads control signals from printer
 * @return int - signal received
 * @pre none
 * @post none
 */
  int readPrinter(){
    if(digitalRead(RX3)==HIGH){
    if((digitalRead(RX1)==HIGH)&&(digitalRead(RX2==LOW)))  return 1;
    if((digitalRead(RX1)==LOW)&&(digitalRead(RX2==HIGH)))  return 2;
    if((digitalRead(RX1)==HIGH)&&(digitalRead(RX2==HIGH))) return 3;
    }
    else
    return -1;
  }

/**
 * @brief InterLayerComm
 * @details Processes pump control on layer change, when finished with pump function will release printer
 * @return none
 * @pre none
 * @post none
 */
  void InterLayerComm(){
    if(printerReleased){

      delay(500);
      pumpstopNE();
      
      if(getCurrentVolume()<(getZLayer(currentLayer)+10)){    //If current Volume of Syringe is less than current layer amount needed +10 ul extra for space
      SwitchValueToTank();
      if(getZLayer(currentLayer)!=0)
      pumpchg(getZLayer(currentLayer)-getCurrentVolume());
  
  
      }
      while(isPumpDone()==false) {PumpWaitLoop(); waitcase();}
      
      SwitchValueToOutput();
      if(getZLayer(currentLayer)!=0)
      pumpdis(getZLayer(currentLayer));
  
      currentLayer++;
  
      releasePrinter();
    }
  }
  
