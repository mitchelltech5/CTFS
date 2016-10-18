/**
 * Copyright 2016; Mitchell Wagner, Adam Carlock and Kim Eagleston
 * as an unpublished work.  All rights reserved.
 * This Embedded Software was developed for CTFS or Chocolate 
 * Temper Flow System. CTFS Verison 1.0 Software Version 1.3
 * 
 * File Header:
 * This File contains all functions and data relating to the 
 * Sd Card Reader
 * 
 * Wiring For Sd Card Reader:
 * 
 * pin 50 MISO
 * pin 51 MOSI
 * pin 52 SCK
 * pin 53 SS Slave
 * 
 */
 /**@file SdCardRead.ino */


   #include "Arduino.h"
   #include <SD.h>     //SD Card Libary
   #include <SPI.h>    //Comm Libary for SD 

   
 //CONTROL VARIABLES

  int maxLayer=0;
  float Z[100];
  int totalVolume=0;

 //CONTROL VARAIABLES

int getMaxLayer(){
  return maxLayer;
}

float getZLayer(int layer){
  return Z[layer];
}

/**
 * @brief processSd
 * @details processes SD card and reads a single file
 * @return none
 * @pre none
 * @post none
 */
void processSd(){
  maxLayer=0;
  for(int x=0; x<sizeof(Z); x++)
    Z[x]=0;

  totalVolume=0;
    
  analogWrite(2,0);  //Turn Heat off to do calc
  analogWrite(3,0);  //Turn Heat off to do calc
  analogWrite(4,0);  //Turn Heat off to do calc
  analogWrite(5,0);  //Turn Heat off to do calc
  analogWrite(6,0);  //Turn Heat off to do calc
  analogWrite(7,0);  //Turn Heat off to do calc
  analogWrite(8,0);  //Turn Heat off to do calc
  //TK1.write(90);      //Stop Rotation
  //TK2.write(90);      //Stop Rotation

     
  int Xpos=1;        //Xpos for distance formula array variables
  int Ypos=1;        //Ypos for distance formula
  int Zpos=0;        //Zpos for layer Height
  double  X[ 2 ], Y[ 2 ]; //Xpos and Ypos for distance formula
  X[0]=0;           //Set X to starting point of print
  Y[0]=0;           //Set Y to starting point of print
  int LayerTrigger=0;      //Enables Z layer to trigger
  float layersum=0;  //Layer by layer Sum
  char prevChar1='a', prevChar2='a', prevChar3='a';
  File myFile;        //SD card file for print
  File root;          //Root file of SD card root
  
  if (!SD.begin(53)) {    //initialize SD card CS pin 10
    addError(FATAL_SD_CARD_NF);
    return;
  }
  removeError(FATAL_SD_CARD_NF);
  
  myFile = SD.open("Part1.txt", FILE_READ);
  
  if (myFile) {                      //if file can be opened
    while (myFile.available()) {
      char c;
      //Serial.write(a);  /DEBUG
      c=myFile.read();  //Read next char
      
      if(c=='X')              //if X is Read
      {
        float xNum;
        char xNumCharArray[7];
        int pos=0;
        while(1)
        {                      //Read X # as STR
          c=myFile.read();
          if(c==' ') break;
          xNumCharArray[pos]=c;
          pos++;
        }
        xNum=atof(xNumCharArray);            //convert STR to float
        Serial.print("X=  ");
        Serial.println(xNum,3);
        X[Xpos]=xNum;            //Set X # to distance formula array
        Xpos++;              //Increment X distance formula array
      }

      if(c=='Y')          //if Y is Read
      {
        float yNum;
        char yNumCharArray[7];
        int pos=0;
        while(1)
        {                    //Read Y # as STR
          c=myFile.read();
          if(c==' ') break;
          yNumCharArray[pos]=c;
          pos++;
        }
        yNum=atof(yNumCharArray);          //convert STR to float
        Serial.print("Y=  ");
        Serial.println(yNum,3);
        Y[Ypos]=yNum;           //Set Y # to distance formula array
        Ypos++;              //Increment Y distance formula array
      }

      if((Ypos==2)&&(Xpos==2))    //If distance formula is ready to be used
      {
        double XCalc=((abs(X[1]-X[0]))*(abs(X[1]-X[0])));    //(X2-X1)^2
        //Serial.println(XCalc,5);  //DEBUG
        double YCalc=pow(abs(Y[1]-Y[0]), 2);                //(Y2-Y1)^2
        //Serial.println(YCalc,5); //DEBUG
        double SQRTCalc=sqrt(XCalc+YCalc);                            //SquareRoot( (X2-X1)^2 + (Y2-Y1)^2 )
        //Serial.println(XCalc+YCalc,5);  //DEBUG
        
        if(SQRTCalc<200){
          layersum=(float)layersum+(float)SQRTCalc;                //add answer to layersum
        }

        X[0]=X[1];                                        //reset distance formula array
        Y[0]=Y[1];                                        //reset distance formula array
        Xpos=1;                                           //reset Distance formula var
        Ypos=1;                                           //reset Distance formula var
        LayerTrigger=1;                                   //Allow Z layer to change
      }

      
      prevChar1=c;              //Save last char
      prevChar2=prevChar1;      //Save last-1 char
      prevChar3=prevChar2;      //Save last-2 char
      //if layer change (Z SEEN) or Home CMD (M84 SEEN)
      if(((c=='Z')&&(LayerTrigger==1))||((prevChar3=='M')&&(prevChar2=='8')&&(prevChar1=='4')&&LayerTrigger==1))      //If Z is Read
      {
        LayerTrigger=0;                    //reset Z trigger to trigger again
        
        float zNum;
        char zNumCharArray[7];
        int pos=0;
        while(1)
        {                            //Read Z in as a STR
          c=myFile.read();
          if(c==' ') break;
          zNumCharArray[pos]=c;
          pos++;
        }
        zNum=atof(zNumCharArray);                //Convert Z to Float
        Serial.print("Z=  ");                    //Display for debug
        Serial.println(zNum,3);                  //Next Layer

    
        Serial.print("LAYER TRAVEL: ");
        Serial.println(layersum);
        
        //CALC FOR VOLUME
        int MP=60;            //travel speed
        int FR=2000;          //Flow rate
        int MT= layersum/MP;
        Z[Zpos]=MT*FR/60;
        Serial.print("VOLUME: ");
        Serial.print(Z[Zpos]);
        Serial.println("ul");
        
        layersum=0;                //Set layer Sum to 0 for next layer
        totalVolume += Z[Zpos];       //Add number to total
        
        Zpos++;                    //increment Zpos to next layer
      }
      
    }
    myFile.close();        //Close file
    root.close();         //Close file Sd Card
    maxLayer=Zpos-1;      //Adjust Max Layer
    removeError(SD_CARD_FILE_NF);
  } 
  else 
  {
    // if the file didn't open
    addError(SD_CARD_FILE_NF);
  }
}
