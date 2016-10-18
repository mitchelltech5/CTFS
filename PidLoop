/**
 * Copyright 2016; Mitchell Wagner, Adam Carlock and Kim Eagleston
 * as an unpublished work.  All rights reserved.
 * This Embedded Software was developed for CTFS or Chocolate 
 * Temper Flow System. CTFS Verison 1.0 Software Version 1.3
 * 
 * File Header:
 * This File contains all functions and data relating to the PID
 * Loops for all heaters.   
 * 
 */
/**@file PidLoop.ino */
/**
 * Wiring for Heat and Cooling:
 * 
 * Sensors:
 * Pin A0 Tank 1 Inside
 * Pin A1 Tank 1 Element
 * Pin A2 Tank 2 Inside
 * Pin A3 Tank 3 Element
 * Pin A4 Syringe
 * Pin A5 Tube 1
 * Pin A6 Tube 2
 * Pin A7 Tube 3
 * Pin A8 Cooling
 * 
 * Control:
 * Pin 2 Tank 1 PID Output
 * Pin 3 Tank 2 PID Output
 * Pin 4 Syringe PID Output
 * Pin 5 Tube 1 PID Output
 * Pin 6 Tube 2 PID Output
 * Pin 7 Tube 3 PID Output
 * Pin 8 Cooling PID Output
 */
 
#include "Arduino.h"
#include <PID_v1.h> //PID Libary

/**Naming Convention
 * T1 -> Tank1
 * T2 -> Tank2
 * S  -> Syringe
 * L1 -> Line1
 * L2 -> Line2
 * L3 -> Line3
 * C  -> Chamber
 */

#define T1 2
#define T2 3
#define S  4
#define L1 5
#define L2 6
#define L3 7
#define C  8

//CONTROL VARIABLES

boolean PidControl[7];

//CONTROL VARIABLES


//PID VARIABLES

  //SETPOINTS for temp control
  int SETT1;
  int SETT2;
  int SETS;
  int SETL1;
  int SETL2;
  int SETL3;
  int SETC;  

  //PID Loop SetPoints and Variables
  double SetpointT1, InputT1, OutputT1;   //PID Loop Vars Tank 1
  double SetpointT2, InputT2, OutputT2;   //PID Loop Vars Tank 2
  double SetpointS, InputS, OutputS;       //PID Loop Vars Syringe
  double SetpointL1, InputL1, OutputL1;   //PID Loop Vars Tube 1 (L1)
  double SetpointL2, InputL2, OutputL2;    //PID Loop Vars Tube 2 (L2)
  double SetpointL3, InputL3, OutputL3;    //PID Loop Vars Tube 3 (L3)
  double SetpointC, InputC, OutputC;       //PID Loop Vars Cooling Chamber
  
  PID T1PID(&InputT1, &OutputT1, &SetpointT1,2,5,1, DIRECT);  //PID INPUTS Tank 1
  PID T2PID(&InputT2, &OutputT2, &SetpointT2,2,5,1, DIRECT);  //PID INPUTS Tank 2
  PID SPID(&InputS, &OutputS, &SetpointS,2,5,1, DIRECT);      //PID INPUTS Syringe
  PID L1PID(&InputL1, &OutputL1, &SetpointL1,2,5,1, DIRECT);  //PID INPUTS Tube 1 (L1)
  PID L2PID(&InputL2, &OutputL2, &SetpointL2,2,5,1, DIRECT);  //PID INPUTS Tube 2 (L2)
  PID L3PID(&InputL3, &OutputL3, &SetpointL3,2,5,1, DIRECT);  //PID INPUTS Tube 3 (L3)
  PID CPID(&InputC, &OutputC, &SetpointC,2,5,1, REVERSE);     //PID INPUTS Cooling Chamber

//PID VARIABLES

/**
 * @brief PidSetup
 * @details Pid Loop setup
 * @return none
 * @pre none
 * @post All Pid Loops will be initilized
 */
void PidSetup(){
  Serial.println("Setting Up Pid Pins and Variables");
  //TEMPERTURE SETTINGS (exact):
  SETT1=105;
  SETT2=90;
  SETS=90;
  SETL1=105;
  SETL2=90;
  SETL3=90;
  SETC=70; 
  //TEMPERTURE SETTINGS (Pid-not exact)
  SetpointT1=120;
  SetpointT2=110;
  SetpointS=95;
  SetpointL1=110;
  SetpointL2=95;
  SetpointL3=95;
  SetpointC=65;
  //PIDCONTROL Booleans
  for(int x=0; x<7; x++)
  PidControl[x]=false;
  
  //Set PinModes
  pinMode(T1,OUTPUT);
  pinMode(T2,OUTPUT);
  pinMode(S,OUTPUT);
  pinMode(L1,OUTPUT);
  pinMode(L2,OUTPUT);
  pinMode(L3,OUTPUT);
  pinMode(C,OUTPUT);

  //PID Loop Setup

  //0->25 for tanks |||||   0->50  for tubes
  T1PID.SetMode(AUTOMATIC);      //PID Control Mode
  T1PID.SetOutputLimits(0, 255);  //Output Limits for PWM (MIN, MAX)

  T2PID.SetMode(AUTOMATIC);      //PID Control Mode
  T2PID.SetOutputLimits(0, 255);  //Output Limits for PWM (MIN, MAX)

  SPID.SetMode(AUTOMATIC);       //PID Control Mode
  SPID.SetOutputLimits(0, 255);   //Output Limits for PWM (MIN, MAX)

  L1PID.SetMode(AUTOMATIC);      //PID Control Mode
  L1PID.SetOutputLimits(0, 255);  //Output Limits for PWM (MIN, MAX)

  L2PID.SetMode(AUTOMATIC);      //PID Control Mode
  L2PID.SetOutputLimits(0, 255);  //Output Limits for PWM (MIN, MAX)

  L3PID.SetMode(AUTOMATIC);      //PID Control Mode
  L3PID.SetOutputLimits(0, 255);  //Output Limits for PWM (MIN, MAX)

  CPID.SetMode(AUTOMATIC);      //PID Control Mode
  CPID.SetOutputLimits(0, 255);  //Output Limits for PWM (MIN, MAX)

  
}

/**
 * @brief PidRun
 * @details All Pid Loops will be checked 
 * and changed according to temperature
 * @return none
 * @pre Pid Loops must be initilized with PidSetup
 * @post temperatures will be adjusted
 */
void PidRun(){
  Serial.println("PID Check and Run");
  if(isTank1On())
    PidTank1();
  else
    digitalWrite(T1,LOW);

  if(isTank2On())
    PidTank2();
  else
    digitalWrite(T2,LOW);

  if(isSyringeOn())
    PidSyringe();
  else
    digitalWrite(S,LOW);

  if(isLine1On())
    PidLine1();
  else
    digitalWrite(L1,LOW);

  if(isLine2On())
    PidLine2();
  else
    digitalWrite(L2,LOW);

  if(isLine3On())
    PidLine3();
  else
    digitalWrite(L3,LOW);

  if(isChamberOn())
    PidChamber();
  else
    digitalWrite(C,LOW);
}

/**
 * @brief PidTank1
 * @details checks Pid of Tank1 and adjusts accordingly 
 * @return none
 * @pre Pid Loops must be initilized with PidSetup
 * @post temperature will be adjusted
 */
void PidTank1(){
  InputT1 = tempRead("Tank1_Element");  //Tank 1 Element Sensor
  int Inside=tempRead("Tank1_Inside");  //Tank 1 Inside Sensor
  T1PID.Compute();    //PID Computation
  if(InputT1<150)     //SAFETY TEST
  {
    removeError(TANK1_SAFETY);
    if(Inside<SETT1)      //IF inside Temp is less than  TANK 1 SETPOINT
     {
      
      if(OutputT1>250)  //IF PID is has an output scale 250<x<=255 
      {
      SerialDisplayDevice("Tank 1","ON", Inside, OutputT1, InputT1, SETT1);
      digitalWrite( T1, HIGH);
      }
      else
      {
      SerialDisplayDevice("Tank 1","OFF (Element OverTemp)", Inside, OutputT1, InputT1, SETT1);
      digitalWrite(T1, LOW);
      }
    }
    else  //ELSE inside Temp is greater than or equal to SETPOINT
    {
      SerialDisplayDevice("Tank 1","OFF (At Temp)", Inside, OutputT1, InputT1, SETT1);
      digitalWrite(T1,LOW);
    }
  }
  else {
  digitalWrite(T1,LOW);  // SAFETY SHUTDOWN
  SerialDisplayDevice("Tank 1","OFF (SAFETY)", Inside, OutputT1, InputT1, SETT1);
  addError(TANK1_SAFETY);
  }
}

/**
 * @brief PidTank2
 * @details checks Pid of Tank2 and adjusts accordingly 
 * @return none
 * @pre Pid Loops must be initilized with PidSetup
 * @post temperature will be adjusted
 */
void PidTank2(){
  InputT2 = tempRead("Tank2_Element");  //Tank 2 Element Sensor
  int Inside=tempRead("Tank2_Inside");  //Tank 2 Inside Sensor
  T2PID.Compute();    //PID Computation
  if(InputT2<150)       //SAFETY If under 150*F OUTPUT HEAT
  {
    removeError(TANK2_SAFETY);
    if(Inside<SETT2)      //IF inside Temp is less than  TANK 2 SETPOINT
     {
      
      if(OutputT2>250)  //IF PID is "ON" Turn on otherwise off
      {
        SerialDisplayDevice("Tank 2","ON", Inside, OutputT2, InputT2, SETT2);
        digitalWrite( T2, HIGH);
      }
      else
      {
         SerialDisplayDevice("Tank 2","OFF (Element OverTemp)", Inside, OutputT1, InputT1, SETT2);
         digitalWrite(T2, LOW);
      }
    }
    else              //IF inside Temp is greater than or equal to SETPOINT
    {
      SerialDisplayDevice("Tank 2","OFF (At Temp)", Inside, OutputT2, InputT2, SETT2);
      digitalWrite(T2,LOW);
    }
  }
  else {
    digitalWrite(T2,LOW);         // Output for tank 2 to Digital Pin 3
    SerialDisplayDevice("Tank 2","OFF (SAFETY)", Inside, OutputT2, InputT2, SETT2);
    addError(TANK2_SAFETY);
  }
  
}

/**
 * @brief PidSyringe
 * @details checks Pid of Syringe and adjusts accordingly 
 * @return none
 * @pre Pid Loops must be initilized with PidSetup
 * @post temperature will be adjusted
 */
void PidSyringe(){
  InputS = tempRead("Syringe");  //Syringe Sensor
  SPID.Compute();    //PID Computation
  if(InputS<150)       //SAFETY If under 150*F OUTPUT HEAT
  {
    removeError(SYRINGE_SAFETY);
    if(InputS<SETS)      //IF inside Temp is less than  SYRINGE SETPOINT
     {
      
      if(OutputS>250)  //IF PID is "ON" Turn on otherwise off
      {
        SerialDisplayDevice("Syringe","ON", 0, OutputS, InputS, SETS);
        digitalWrite( S, HIGH);
      }
      else
      {
        SerialDisplayDevice("Syringe","OFF (Element OverTemp)", 0, OutputS, InputS, SETS);
        digitalWrite(S, LOW);
      }
     }
    else              //IF inside Temp is greater than or equal to SETPOINT
    {
      SerialDisplayDevice("Syringe","OFF (At Temp)", 0, OutputS, InputS, SETS);
      digitalWrite(S,LOW);
    }
  }
  else {
    digitalWrite(S,LOW);         // Output for Syringe to Digital Pin 4
    SerialDisplayDevice("Syringe","OFF (SAFETY)", 0, OutputS, InputS, SETS);
    addError(SYRINGE_SAFETY);

  }
  
}

/**
 * @brief PidLine1
 * @details checks Pid of Line1 and adjusts accordingly 
 * @return none
 * @pre Pid Loops must be initilized with PidSetup
 * @post temperature will be adjusted
 */
void PidLine1(){
  InputL1 = tempRead("Line1");  //Tube 1 (L1) Sensor
  L1PID.Compute();    //PID Computation
  if(InputL1<150)       //SAFETY If under 150*F OUTPUT HEAT
  {
    removeError(LINE1_SAFETY);
    if(InputL1<SETL1)      //IF inside Temp is less than  Tube 1 (L1) SETPOINT
     {
      
      if(OutputL1>250)  //IF PID is "ON" Turn on otherwise off
      {
        SerialDisplayDevice("Line 1","ON", 0, OutputL1, InputL1, SETL1);
        digitalWrite( L1, HIGH);
      }
      else
      {
        SerialDisplayDevice("Line 1","OFF (Element OverTemp)", 0, OutputL1, InputL1, SETL1);
        digitalWrite(L1, LOW);
      }
    }
    else              //IF inside Temp is greater than or equal to SETPOINT
    {
      SerialDisplayDevice("Line 1","OFF (At Temp)", 0, OutputL1, InputL1, SETL1);
      digitalWrite(L1,LOW);
    }
  }
  else {
    digitalWrite(L1,LOW);         // Output for Tube 1 (L1) to Digital Pin 5
    SerialDisplayDevice("Line 1","OFF (SAFETY)", 0, OutputL1, InputL1, SETL1);
    addError(LINE1_SAFETY);
  }
  
}

/**
 * @brief PidLine2
 * @details checks Pid of Line2 and adjusts accordingly 
 * @return none
 * @pre Pid Loops must be initilized with PidSetup
 * @post temperature will be adjusted
 */
void PidLine2(){
  InputL2 = tempRead("Line2");  //Tube 2 (L2) Sensor
  L2PID.Compute();    //PID Computation
  if(InputL2<150)       //SAFETY If under 150*F OUTPUT HEAT
  {
    removeError(LINE2_SAFETY);
    if(InputL2<SETL2)      //IF inside Temp is less than  Tube 2 (L2) SETPOINT
     {
      
      if(OutputL2>250)  //IF PID is "ON" Turn on otherwise off
      {
        SerialDisplayDevice("Line 2","ON", 0, OutputL2, InputL2, SETL2);
        digitalWrite( L2, HIGH);
      }
      else
      {
        SerialDisplayDevice("Line 2","OFF (Element OverTemp)", 0, OutputL2, InputL2, SETL2);
        digitalWrite(L2, LOW);
      }
    }
    else              //IF inside Temp is greater than or equal to SETPOINT
    {
      SerialDisplayDevice("Line 2","OFF (At Temp)", 0, OutputL2, InputL2, SETL2);
      digitalWrite(L2,LOW);
    }
  }
  else {
    digitalWrite(L2,LOW);         // Output for Tube 2 (L2) to Digital Pin 6
    SerialDisplayDevice("Line 2","OFF (SAFETY)", 0, OutputL2, InputL2, SETL2);
    addError(LINE2_SAFETY);
  }
  
}

/**
 * @brief PidLine3
 * @details checks Pid of Line3 and adjusts accordingly 
 * @return none
 * @pre Pid Loops must be initilized with PidSetup
 * @post temperature will be adjusted
 */
void PidLine3(){
  InputL3 = tempRead("Line3");  //Tube 3 (L3) Sensor
  L3PID.Compute();    //PID Computation
  if(InputL3<150)       //SAFETY If under 150*F OUTPUT HEAT
  {
    removeError(LINE3_SAFETY);
    if(InputL3<SETL3)      //IF inside Temp is less than  Tube 3 (L3) SETPOINT
     {
      
      if(OutputL3>250)  //IF PID is "ON" Turn on otherwise off
      {
        SerialDisplayDevice("Line 3","ON", 0, OutputL3, InputL3, SETL3);
        digitalWrite( L3, HIGH);
      }
      else
      {
        SerialDisplayDevice("Line 3","OFF (Element OverTemp)", 0, OutputL3, InputL3, SETL3);
        digitalWrite(L3, LOW);
      }
    }
    else  //IF inside Temp is greater than or equal to SETPOINT
    {
      SerialDisplayDevice("Line 3","OFF (At Temp)", 0, OutputL3, InputL3, SETL3);
      digitalWrite(L3,LOW);
    }
  }
  else {
    digitalWrite(L3,LOW);         // Output for Tube 3 (L3) to Digital Pin 7
    SerialDisplayDevice("Line 3","OFF (SAFETY)", 0, OutputL3, InputL3, SETL3);
    addError(LINE3_SAFETY);
  }
  
}

/**
 * @brief PidChamber
 * @details checks Pid of Chamber and adjusts accordingly 
 * @return none
 * @pre Pid Loops must be initilized with PidSetup
 * @post temperature will be adjusted
 */
void PidChamber(){
  InputC = tempRead("Chamber");  //Chamber Sensor
  CPID.Compute();    //PID Computation
  if(InputC<150)       //SAFETY If under 150*F OUTPUT HEAT
  {
    removeError(CHAMBER_SAFETY);
    if(InputC>SETC)      //IF inside Temp is Greater than  Chamber SETPOINT
     {
      
      if(OutputC>250)  //IF PID is "ON" Turn on otherwise off
      {
        SerialDisplayDevice("Chamber","ON", 0, OutputC, InputC, SETC);
        digitalWrite(C, HIGH);
      }
      else
      {
        SerialDisplayDevice("Chamber","OFF (Element UnderTemp)", 0, OutputC, InputC, SETC);
        digitalWrite(C, LOW);
      }
    }
    else              //IF inside Temp is greater than or equal to SETPOINT
    {
      SerialDisplayDevice("Chamber","OFF (At Temp)", 0, OutputC, InputC, SETC);
      digitalWrite(C,LOW);
    }
  }
  else {
    digitalWrite(C,LOW);         // Output for Chanber to Digital Pin 8
    SerialDisplayDevice("Chamber","OFF (SAFETY)", 0, OutputC, InputC, SETC);
    addError(CHAMBER_SAFETY);
  }
  
}



boolean isTank1On(){
  return PidControl[0];
}
boolean isTank2On(){
  return PidControl[1];
}
boolean isSyringeOn(){
  return PidControl[2];
}
boolean isLine1On(){
  return PidControl[3];
}
boolean isLine2On(){
  return PidControl[4];
}
boolean isLine3On(){
  return PidControl[5];
}
boolean isChamberOn(){
  return PidControl[6];
}

void setTank1(boolean control){
  PidControl[0]=control;
}
void setTank2(boolean control){
  PidControl[1]=control;
}
void setSyringe(boolean control){
  PidControl[2]=control;
}
void setLine1(boolean control){
  PidControl[3]=control;
}
void setLine2(boolean control){
  PidControl[4]=control;
}
void setLine3(boolean control){
  PidControl[5]=control;
}
void setChamber(boolean control){
  PidControl[6]=control;
}



/**
 * @brief tempRead
 * @details Reads the temperature of sensor in param
 * @param[in] sensor - sensor to be read
 * @return temperature in *F
 * @pre none
 * @post none
 */
float tempRead(String sensor) {
int  THERMISTORPIN;

// which analog pin to connect
if(sensor== "Tank1_Inside") THERMISTORPIN= A0;
else if(sensor=="Tank1_Element") THERMISTORPIN= A1;   
else if(sensor=="Tank2_Inside") THERMISTORPIN= A2;   
else if(sensor=="Tank2_Element") THERMISTORPIN= A3;   
else if(sensor=="Syringe") THERMISTORPIN= A4;   
else if(sensor=="Line1") THERMISTORPIN= A5;  
else if(sensor=="Line2") THERMISTORPIN= A6;    
else if(sensor=="Line3") THERMISTORPIN= A7;   
else if(sensor=="Chamber") THERMISTORPIN= A8; 
else THERMISTORPIN=A0;

// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000
  float reading;

  reading = analogRead(THERMISTORPIN);

  //Serial.print("Analog reading ");
  //Serial.println(reading);

  // convert the value to resistance
  reading = (1023 / reading)  - 1;
  reading = SERIESRESISTOR / reading;
  //Serial.print("Thermistor resistance ");
 // Serial.println(reading);

  float steinhart;
  steinhart = reading / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C


 /**switch(pin){
  case 0: Serial.print("A0  Temp ");    break;
  case 1: Serial.print("A1  Temp ");    break;
  case 2: Serial.print("A2  Temp ");    break;
  case 3: Serial.print("A3  Temp ");    break;
  case 4: Serial.print("A4  Temp ");    break;
  case 5: Serial.print("A5  Temp ");    break;
  case 6: Serial.print("A6  Temp ");    break;
  case 7: Serial.print("A7  Temp ");    break;
  case 8: Serial.print("A8  Temp ");    break;
  default: break;
 }*/


  float Temp=((steinhart*9)/5)+32;
  //Serial.print(Temp);
  //Serial.print(" *F");
 return Temp;

}


