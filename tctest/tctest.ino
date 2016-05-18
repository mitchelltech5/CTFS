#include <stdlib.h>  //Standard Libary for C
#include <math.h>    //Standard Math Libary for C
#include <SoftwareSerial.h>  //Serial Pump Comm
#include <Servo.h>   //Servo Control
#include <SD.h>     //SD Card Libary
#include <SPI.h>    //Comm Libary for SD card
#include <ctype.h>  //Custom Serial Interface Libary
#include <PID_v1.h> //PID Libary
boolean tank1=false, tank2=false, syringe=false, purge=false, purge2=false, Sd=false, prnt=false, releas=false;  //Main Boolean Variables for tasks (run time Vars)
boolean afterprint=false, afterques=false, aftercontin=false, aftersleep=false, aftersleep1=false, clean=false;
boolean purgetemp=false, purge2temp=false, Sdtemp=false, prnttemp=false, autoh=false, gogo=false;                  //Secondary instance class Vars

int curlayer=0, maxlayer=0;    //Layer Height of Z
float Z[100];                    //Zarray for vol ussage

Servo way3;        //Servo for 3 Way valve  
Servo TK1;         //Servo for Tank 1
Servo TK2;         //Servo for Tank 2

SoftwareSerial mySerial =  SoftwareSerial(62,48, false);  //RX, TX  Serial Pump Comm
int curvol=0;      //Current Syringe VOl
int emergency=0;   //Emergency Stop condition
int maxvol=10000;  //MaxVol of Syringe

int firstline=0;

int SETT1=105, SETT2=90, SETS=90, SETL1=105, SETL2=90, SETL3=90, SETC=70;  //SETPOINTS for temp control
double SetpointT1=120, InputT1, OutputT1;    //PID Loop Vars Tank 1
double SetpointT2=110, InputT2, OutputT2;    //PID Loop Vars Tank 2
double SetpointS=95, InputS, OutputS;       //PID Loop Vars Syringe
double SetpointL1=110, InputL1, OutputL1;    //PID Loop Vars Tube 1 (L1)
double SetpointL2=95, InputL2, OutputL2;    //PID Loop Vars Tube 2 (L2)
double SetpointL3=95, InputL3, OutputL3;    //PID Loop Vars Tube 3 (L3)
double SetpointC=65, InputC, OutputC;       //PID Loop Vars Cooling Chamber

PID T1PID(&InputT1, &OutputT1, &SetpointT1,2,5,1, DIRECT);  //PID INPUTS Tank 1
PID T2PID(&InputT2, &OutputT2, &SetpointT2,2,5,1, DIRECT);  //PID INPUTS Tank 2
PID SPID(&InputS, &OutputS, &SetpointS,2,5,1, DIRECT);      //PID INPUTS Syringe
PID L1PID(&InputL1, &OutputL1, &SetpointL1,2,5,1, DIRECT);  //PID INPUTS Tube 1 (L1)
PID L2PID(&InputL2, &OutputL2, &SetpointL2,2,5,1, DIRECT);  //PID INPUTS Tube 2 (L2)
PID L3PID(&InputL3, &OutputL3, &SetpointL3,2,5,1, DIRECT);  //PID INPUTS Tube 3 (L3)
PID CPID(&InputC, &OutputC, &SetpointC,2,5,1, REVERSE);      //PID INPUTS Cooling Chamber

int R1=22, R2=23, R3=24, tx=25;

/*
Pin A0 Tank 1 Inside
Pin A1 Tank 1 Element
Pin A2 Tank 2 Inside
Pin A3 Tank 3 Element
Pin A4 Syringe
Pin A5 Tube 1
Pin A6 Tube 2
Pin A7 Tube 3
Pin A8 Cooling

Pin 2 Tank 1 PID Output
Pin 3 Tank 2 PID Output
Pin 4 Syringe PID Output
Pin 5 Tube 1 PID Output
Pin 6 Tube 2 PID Output
Pin 7 Tube 3 PID Output
Pin 8 Cooling PID Output

pin 22 R1 3d printer comm
pin 23 R2 3d printer comm
pin 24 R3 3d printer comm
pin 25 tx 3d printer comm

pin 26 Rx Pump comm 62
pin 27 Tx Pump comm

pin 32 3 way Servo
pin 33 Tank 1 Servo
pin 34 Tank 2 Servo

pin 50 MISO
pin 51 MOSI
pin 52 SCK
pin 53 SS Slave



*/

void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(53, OUTPUT);    //SD Card SS Pin
   pinMode(46, OUTPUT);
   digitalWrite(46, HIGH); 
   digitalWrite(53, HIGH); 
   pinMode(10, OUTPUT);
   digitalWrite(10, LOW); 
  pinMode(R1,INPUT);      //R1 Pin setup Serial Comm
  pinMode(R2,INPUT);      //R2 Pin setup Serial Comm
  pinMode(R3,INPUT);      //R3 Pin setup Serial Comm
  pinMode(tx,OUTPUT);     //TX Pin setup Serial Comm
  digitalWrite(tx, LOW);  //TX int setup
  Serial.begin(9600);    //Usb Debug Start Serial Comm 9600 Baud
  mySerial.begin(9600);  //Pump Comm Start at 9600 Baud
  Serial.println("STARTUP");
  intstart();          //Startup Rates For pump
  
  way3.attach(32);    //3 Way Value Servo motor start
  way3.write(0);      //Set to 0 Degrees
  delay(1000);        //Wait 1 sec
  way3.detach();      //Stop Servo Connection
  
  TK1.attach(33);    //Tank 1 Servo Motor Attach
  TK2.attach(34);    //Tank 2 Servo Motor Attach
 // Z[0]=0;
  
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

void loop() {
   
  waitcase();
  int temp;
  int input=SWread();
 // Serial.println("\n\n\n\n\n\n\n\n\n");
  Serial.println(input);
  Serial.print(digitalRead(R1));  Serial.print("\t"); Serial.print(digitalRead(R2));  Serial.print("\t");Serial.println(digitalRead(R3)); 
  switch(input)
  {
   case 1: {if(releas==true) SDread(); break;}
   case 2: {if(releas==true) sysOK(); break;}   
   case 3: {autoh=true; if(releas==true&&(curlayer>=maxlayer-2)) {afterprint=true; break;}
             if((releas==true)&&(curlayer<maxlayer-1))
             {
                digitalWrite(tx, HIGH);
                while(digitalRead(R3)==HIGH);
                digitalWrite(tx, LOW);
               autoh=false;
               break;
             }
 
         }  
  }
  while(1){
  if((tank1!=true)&&(tank2!=true)&&(syringe!=true)&&(releas!=true)&&(aftercontin!=true))  //UI FOR TANK 1
  {
    Serial.println("Check Tank 1");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){
    tank1=true;}
    break;
  }
  if((tank1==true)&&(tank2!=true)&&(syringe!=true)&&(releas!=true)&&(aftercontin!=true)) //UI FOR TANK 2
  {
    Serial.println("Check Tank 2");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){
    tank2=true;}
    
    
   
    break;
  }
  if((tank1==true)&&(tank2==true)&&(syringe!=true)&&(releas!=true)&&(aftercontin!=true)) //UI FOR Syringe 
  {
    Serial.println("Check Syringe");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){
    syringe=true;}
    
    
    break;
  }
  if((tank1==true)&&(tank2==true)&&(syringe==true)&&(releas!=true)&&(aftercontin!=true))  
  {
    if((purgetemp!=true)&&(Sdtemp!=true)&&(prnttemp!=true))    //UI FOR Purge Sequence
    {
     Serial.println("Start FIRST Purge?");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){
    
      
      purgetemp=true;
      purge=true;}
      break;
    }
    if((purgetemp==true)&&(purge2temp!=true)&&(Sdtemp!=true)&&(prnttemp!=true))    //UI FOR Purge Sequence
    {
     Serial.println("Start SECOND Purge?");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){
    
      
      purge2temp=true;
      purge=true;}
      break;
    }
    if((purgetemp==true)&&(purge2temp==true)&&(Sdtemp!=true)&&(prnttemp!=true)) //UI FOR SD Card Read
    {
      Serial.println("Read Sd Card?");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){
    
      
      
      Sdtemp=true;
      Sd=true;}
      break;
    }
    if((purgetemp==true)&&(purge2temp==true)&&(Sdtemp==true)&&(prnttemp!=true))  //UI FOR Check system and release
    {
      Serial.println("Check Build Plate and Nozzile");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){
    
      
      prnt=true;
      purgetemp=false;
      purge2temp=false;
      Sdtemp=false;
      prnttemp=false;}
      break;
    }
    break;
  }
  break;
  }
  
  if(tank1==true)
  {
    Serial.println("TANK 1 ON");
     TK1.write(0);      //Rotate in 1 direction
    InputT1 = tempread(0);  //Tank 1 Element Sensor
    int Inside=tempread(1);  //Tank 1 Inside Sensor
    T1PID.Compute();    //PID Computation
    if(InputT1<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(Inside<SETT1)      //IF inside Temp is less than  TANK 1 SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputT1>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite( 2, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(2, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to 100 *F
      {
      Serial.println("   LOW");
      digitalWrite(2,LOW);
      } 
    }
    else {
    digitalWrite(2,LOW);         // Output for tank 1 to Digital Pin 2
    Serial.println("TEMP SAFETY TANK1");
    }
    //Debug Display
    Serial.print("Tank 1 Inside: "); Serial.print(Inside); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputT1); Serial.print("PWM  Element Temp:"); Serial.print(InputT1); Serial.println("*F");
    
  }
  if(tank2==true)
  {
    Serial.println("TANK 2 ON");
     TK2.write(0);      //Rotate in 1 direction
    InputT2 = tempread(2);  //Tank 2 Element Sensor
    int Inside=tempread(3);  //Tank 2 Inside Sensor
    T2PID.Compute();    //PID Computation
    if(InputT2<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(Inside<SETT2)      //IF inside Temp is less than  TANK 2 SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputT2>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite( 3, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(3, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to SETPOINT
      {
      Serial.println("   LOW");
      digitalWrite(3,LOW);
      } 
    }
    else {
    digitalWrite(3,LOW);         // Output for tank 2 to Digital Pin 3
    Serial.println("TEMP SAFETY TANK2");
    }
    //Debug Display
    Serial.print("Tank 2 Inside: "); Serial.print(Inside); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputT2); Serial.print("PWM  Element Temp:"); Serial.print(InputT2); Serial.println("*F");
    
  }
  if(syringe==true)
  {
    Serial.println("Syringe and Lines ON");
    InputS = tempread(4);  //Syringe Sensor
    SPID.Compute();    //PID Computation
    if(InputS<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(InputS<SETS)      //IF inside Temp is less than  SYRINGE SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputS>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite( 4, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(4, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to SETPOINT
      {
      Serial.println("   LOW");
      digitalWrite(4,LOW);
      } 
    }
    else {
    digitalWrite(4,LOW);         // Output for Syringe to Digital Pin 4
    Serial.println("TEMP SAFETY Syringe");
    }
    //Debug Display
    Serial.print("Syringe Temp: "); Serial.print(InputS); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputS); Serial.println("PWM"); 
    
    
    InputL1 = tempread(5);  //Tube 1 (L1) Sensor
    L1PID.Compute();    //PID Computation
    if(InputL1<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(InputL1<SETL1)      //IF inside Temp is less than  Tube 1 (L1) SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputL1>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite( 5, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(5, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to SETPOINT
      {
      Serial.println("   LOW");
      digitalWrite(5,LOW);
      } 
    }
    else {
    digitalWrite(5,LOW);         // Output for Tube 1 (L1) to Digital Pin 5
    Serial.println("TEMP SAFETY Line 1");
    }
    //Debug Display
    Serial.print("Tube 1 Temp: "); Serial.print(InputL1); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputL1); Serial.println("PWM"); 
    
    
    InputL2 = tempread(6);  //Tube 2 (L2) Sensor
    L2PID.Compute();    //PID Computation
    if(InputL2<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(InputL2<SETL2)      //IF inside Temp is less than  Tube 2 (L2) SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputL2>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite( 6, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(6, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to SETPOINT
      {
      Serial.println("   LOW");
      digitalWrite(6,LOW);
      } 
    }
    else {
    digitalWrite(6,LOW);         // Output for Tube 2 (L2) to Digital Pin 6
    Serial.println("TEMP SAFETY Tube 2");
    }
    //Debug Display
    Serial.print("Tube 2 Temp: "); Serial.print(InputL2); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputL2); Serial.println("PWM"); 
    
    
    
    InputL3 = tempread(7);  //Tube 3 (L3) Sensor
    L3PID.Compute();    //PID Computation
    if(InputL3<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(InputL3<SETL3)      //IF inside Temp is less than  Tube 3 (L3) SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputL3>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite( 7, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(7, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to SETPOINT
      {
      Serial.println("   LOW");
      digitalWrite(7,LOW);
      } 
    }
    else {
    digitalWrite(7,LOW);         // Output for Tube 3 (L3) to Digital Pin 7
    Serial.println("TEMP SAFETY Tube 3");
    }
    //Debug Display
    Serial.print("Tube 3 Temp: "); Serial.print(InputL3); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputL3); Serial.println("PWM"); 
    
    
    InputC = tempread(8);  //Chamber Sensor
    CPID.Compute();    //PID Computation
   if(InputC<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(InputC>SETC)      //IF inside Temp is Greater than  Chamber SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputC>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite(8, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(8, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to SETPOINT
      {
      Serial.println("   LOW");
      digitalWrite(8,LOW);
      } 
    }
    else {
    digitalWrite(8,LOW);         // Output for Chanber to Digital Pin 8
    Serial.println("TEMP SAFETY Chamber");
    }
    //Debug Display
    Serial.print("Chamber Temp: "); Serial.print(InputC); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputC); Serial.println("PWM"); 
    
    
  }
  
  if(purge==true)
  {
    if(purge2temp!=true){
   Serial.println("purging now"); 
//   dirchange(90);
//   for(int x=0; x<2; x++){
//   pumpchg(500);
//   while(gogo==false) {Serial.println("WAITING FOR CHARGE"); heat(); waitcase(); }
//   }
//   dirchange(0);
//   gogo=false;
//   for(int x=0; x<2; x++){
//   pumpdis(500);
//   while(gogo==false) {Serial.println("WAITING FOR DISCHARGE"); heat(); waitcase(); }
//   }
    }
    else{
       Serial.println("purging  SECOND  now"); 
//   dirchange(90);
//   for(int x=0; x<18; x++){
//   pumpchg(500);
//   while(gogo==false) {Serial.println("WAITING FOR CHARGE"); heat(); waitcase(); }
//   }
//   dirchange(0);
//   gogo=false;
//   for(int x=0; x<18; x++){
//   pumpdis(500);
//   while(gogo==false) {Serial.println("WAITING FOR DISCHARGE"); heat(); waitcase(); }
//   }
//   
    }
  
  purge=false;  
  }
  
  if(Sd==true)
  {
   Serial.println("READ SD NOW"); 
    analogWrite(2,0);  //Turn Heat off to do calc 
    analogWrite(3,0);  //Turn Heat off to do calc 
    analogWrite(4,0);  //Turn Heat off to do calc 
    analogWrite(5,0);  //Turn Heat off to do calc 
    analogWrite(6,0);  //Turn Heat off to do calc 
    analogWrite(7,0);  //Turn Heat off to do calc 
    analogWrite(8,0);  //Turn Heat off to do calc 
     TK1.write(90);      //Stop Rotation
     TK2.write(90);      //Stop Rotation
   int Xpos=1;        //Xpos for distance formula array variables
   int Ypos=1;        //Ypos for distance formula
   int Zpos=0;        //Zpos for layer Height
   double  X[ 2 ], Y[ 2 ]; //Xpos and Ypos for distance formula
    X[0]=0;           //Set X to starting point of print 
    Y[0]=0;           //Set Y to starting point of print
    int quack=0;      //Enables Z layer to trigger
    int firstdistlayer=1;
    int firstlayer=1;
   float layersum=0;  //Layer by layer Sum
   char e='a', f='a', g='a';
   File myFile;        //SD card file for print
   File root;          //Root file of SD card root
   if (!SD.begin(53)) {    //initialize SD card CS pin 10
   Serial.println("initialization failed!");
   return;
   }
   Serial.println("initialization done.");
  // root = SD.open("/");              //open Root
  // myFile= root.openNextFile(); //open first .txt file on card
   myFile = SD.open("Part1.txt", FILE_READ);
   if (myFile) {                      //if file can be opened
    Serial.print("READING");
   while (myFile.available()) {
        char a;
    	//Serial.write(a);
            a=myFile.read();
           
            if(a=='X')              //if X is Read
            {
            float V;
            char VV[7];
            int pos=0;
            while(1)
            {                      //Read X # as STR
              a=myFile.read();    
              if(a==' ') break;
              VV[pos]=a;
              pos++;
            }
            V=atof(VV);            //convert STR to float
          Serial.print("X=  ");
          Serial.println(V,3);
            X[Xpos]=V;            //Set X # to distance formula array
            Xpos++;              //Increment X distance formula array
            }
            
            if(a=='Y')          //if Y is Read
            {
            float V;
            char VV[7];
            int pos=0;
            while(1)
            {                    //Read Y # as STR
              a=myFile.read();
              if(a==' ') break;
              VV[pos]=a;
              pos++;
            }
            V=atof(VV);          //convert STR to float
           Serial.print("Y=  ");
           Serial.println(V,3);
            Y[Ypos]=V;           //Set Y # to distance formula array
            Ypos++;              //Increment Y distance formula array
            }
            
            if((Ypos==2)&&(Xpos==2))    //If distance formula is ready to be used
            {
               
                double o=((abs(X[1]-X[0]))*(abs(X[1]-X[0])));    //(X2-X1)^2
                //Serial.println(o,5);
                double oo=pow(abs(Y[1]-Y[0]), 2);                //(Y2-Y1)^2
                //Serial.println(oo,5);
               // Serial.println(o+oo,5);
                double ooo=sqrt(o+oo);                            //SquareRoot( (X2-X1)^2 + (Y2-Y1)^2 )
                if(ooo<200){
             layersum=(float)layersum+(float)ooo;                //add answer to layersum
          //   if(firstdistlayer==1) {firstdistlayer=(float)ooo; firstdistlayer=0; firstline=firstdistlayer;}
            // Serial.println("HAVE 2 VALUES");
            }
            //Serial.println(ooo,5);
                
              X[0]=X[1];                                        //reset distance formula array
              Y[0]=Y[1];                                        //reset distance formula array
            
             Xpos=1;                                            //reset Distance formula var
            Ypos=1;                                             //reset Distance formula var
            quack=1;                                            //Allow Z layer to change
            }
            e=a;
            f=e;
            g=f;
            if(((a=='Z')&&(quack==1))||((g=='M')&&(f=='8')&&(e=='4')&&quack==1))      //If Z is Read
            {
              quack=0;                    //reset Z allowable-ness to change
            float V;
            char VV[7];
            int pos=0;
            while(1)
            {                            //Read Z in as a STR
              a=myFile.read();
              if(a==' ') break;
              VV[pos]=a;
              pos++;
            }
            V=atof(VV);                //Convert Z to Float
            Serial.print("Z=  ");      //Display for debug
            Serial.println(V,3);

          //  if(firstlayer==1){layersum=layersum-firstlayer; firstlayer=0;}
          Serial.print("LAYER TRAVEL: ");
            Serial.println(layersum);
            Z[Zpos]=layersum*.515;          //Add current layer sum to Z array 
            layersum=0;                //Set layer Sum to 0 for next layer
            
            Serial.println(Z[Zpos]);
            Zpos++;                    //increment Zpos to next layer
            
            
    }
    
    
  }
    myFile.close();        //Close files
     root.close();         //Close files
   maxlayer=Zpos-1;
   Serial.println("done With SD Card");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening File On SD Card");
  }
   
   
  Sd=false;  
  }
  
  
  if(prnt==true)
  {
   Serial.println("RELEASING PRINTER"); 
   if(autoh==true)
   {
      digitalWrite(tx, HIGH);
  while(digitalRead(R3)==HIGH);
  digitalWrite(tx, LOW);
     
     
   }
   
   
   releas=true;
   autoh=false;
  prnt=false;  
  }
  //if((gogo==true)&&(releas==true)) afterprint=true;
  if(temp==50) afterprint=true;
  
  if(afterprint==true)
  {
    Serial.println("Print Finished");
    autoh=false;
     digitalWrite(tx, HIGH);
  while(digitalRead(R3)==HIGH);
  digitalWrite(tx, LOW);
   
   Serial.println("Removed Build Plate?");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){
      afterques=true;
      afterprint=false;
             
        } 
      
 }
  if(afterques==true)
  {
    Serial.println("Continue Printing? (1= Yes)");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){
       aftersleep=true;
      afterques=false;
    }
    if(temp==49){      
      
      aftercontin=true;
      afterques=false;
             
        } 
   
    
  }
  
  if(aftercontin==true)
  {
    tank1=false;
    tank2=false;
    syringe=false;
    analogWrite(2,0);
    analogWrite(3,0);
    analogWrite(4,0);
    analogWrite(5,0);
    analogWrite(6,0);
    analogWrite(7,0);
    analogWrite(8,0);
    
     TK1.write(90);      //Stop Rotation
     TK2.write(90);      //Stop Rotation
    
    
    if(tempread(0)>100)
    Serial.println("Cooling tanks for access");
    else
    {
    Serial.println("Tanks Cool....Refill Complete?");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){
       releas=false;
      aftercontin=false;
    }
    }
  }
  
  if(aftersleep==true)
  {
    TK1.write(90);      //Stop Rotation
    TK2.write(90);      //Stop Rotation
    Serial.println("Start Clean....Change 3 way pos?");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){
       aftersleep1=true;
       aftersleep=false;
    }

  }
  
  if(aftersleep1==true)
  {
    Serial.println("Clean Complete?");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){
       aftersleep1=false;
       analogWrite(2,0);
       analogWrite(3,0);
       analogWrite(4,0);
       analogWrite(5,0);
       analogWrite(6,0);
       analogWrite(7,0);
       analogWrite(8,0);
       
       
       tank1=false;
       tank2=false;
       syringe=false;
       
       clean=true;
    }
    
  }
  if(clean==true)
  {
    Serial.println("Exit Sleep Mode?");
    temp=Serial.read();
    Serial.println(temp);
    if(temp==48){ releas=false;
    }
    
  }
    
  
  
  

}














//#define bit9600Delay 84  
//#define halfBit9600Delay 42
//#define bit4800Delay 188 
//#define halfBit4800Delay 94 
//
//
//byte SWval;
//int pos1=-1;
//int OK=1;
//
//void SWprint(int data)
//{
//  byte mask;
//  //startbit
//  digitalWrite(tx,LOW);
//  delayMicroseconds(bit9600Delay);
//  for (mask = 0x01; mask>0; mask <<= 1) {
//    if (data & mask){ // choose bit
//     digitalWrite(tx,HIGH); // send 1
//    }
//    else{
//     digitalWrite(tx,LOW); // send 0
//    }
//    delayMicroseconds(bit9600Delay);
//  }
//  //stop bit
//  digitalWrite(tx, HIGH);
//  delayMicroseconds(bit9600Delay);
//}

int SWread()
{
  if(digitalRead(R3)==HIGH){
    if((digitalRead(R1)==HIGH)&&(digitalRead(R2==LOW))) return 1;
    if((digitalRead(R1)==LOW)&&(digitalRead(R2==HIGH))) return 2;
    if((digitalRead(R1)==HIGH)&&(digitalRead(R2==HIGH))) return 3;
  }
  else
  return -1;
  
}






void SDread()
{
  digitalWrite(tx, HIGH);
  while(digitalRead(R3)==HIGH);
  digitalWrite(tx, LOW);
}

//void Preheat()
//{
//  for(int i=0; i<100;i++) {SWprint(15);//ACK
//  delay(20);}
//  
//  
//  //Serial.println("PREHEAT HERE");
//  /*
//    digitalWrite(11,HIGH);
//  while(digitalRead(4)==LOW);
//  digitalWrite(11,LOW);*/
//if(releas==true){
//  int ans=0;
//  //delay(5000);
//  do{
//   SWprint(30);
//   delay(10);
//   SWprint(30);
//   delay(10);
//   if(SWread()>5)
//   ans=SWreadT();
// }while(ans!=15);
//}
//  
//}


void sysOK()
{
  if(emergency==0){
   // curlayer++;
    analogWrite(2,0);
    analogWrite(3,0);
    analogWrite(4,0);
    analogWrite(5,0);
    analogWrite(6,0);
    analogWrite(7,0);
    analogWrite(8,0);
    
    TK1.write(90);      //Stop Rotation
    TK2.write(90);      //Stop Rotation
    
   // if(curlayer==0) {delay(firstline*.033); Serial.println(firstline);}
    delay(3000);
    pumpstopNE();
    if(curvol<(Z[curlayer]+10)){    //If current Volume of Syringe is less than current layer amount needed +10 ul extra for space
    dirchange(90);
    if(Z[curlayer]!=0)
    pumpchg(Z[curlayer]-curvol);
    
    
    }
    while(gogo==false) {Serial.println("WAITING FOR CHARGE"); heat(); waitcase();}
    if(gogo==true){
    dirchange(0);
    if(Z[curlayer]!=0)
    pumpdis(Z[curlayer]);
    }
    
    
    
    curlayer++;
    
    
  digitalWrite(tx, HIGH);
  while(digitalRead(R3)==HIGH);
  digitalWrite(tx, LOW);
  
  
  
  }
}
  
  




float tempread(int pin) {
int  THERMISTORPIN;
  
// which analog pin to connect
switch(pin){
  case 0: THERMISTORPIN= A0;    break;
  case 1: THERMISTORPIN= A1;    break;
  case 2: THERMISTORPIN= A2;    break;
  case 3: THERMISTORPIN= A3;    break; 
  case 4: THERMISTORPIN= A4;    break; 
  case 5: THERMISTORPIN= A5;    break; 
  case 6: THERMISTORPIN= A6;    break; 
  case 7: THERMISTORPIN= A7;    break; 
  case 8: THERMISTORPIN= A8;    break; 
  default: break;
}  
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
 
 
 /*switch(pin){
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
void  dirchange(int angle){
  way3.attach(32); 
  way3.write(angle);
  delay(1000);
  way3.detach();
  delay(15);
  
  
  
}
int pumpchg(int vol)
{
  
  
  int ones = (vol%10);
  int tens = ((vol/10)%10);
  int hundreds =  ((vol/100)%10);
  int thousands =  (vol/1000);
  Serial.print(thousands);Serial.print("  ");Serial.print(hundreds);
  Serial.print("  "); Serial.print(tens); Serial.print("  ");
  Serial.println(ones);
  
  if(curvol+vol>maxvol){Serial.println("ABOVE MAX"); return 0;}
delay(200);
  mySerial.write(0x6d);
  mySerial.write(0x6f);
  mySerial.write(0x64);
  mySerial.write(0x65);
  mySerial.write(0x20);
  mySerial.write(0x77);
  mySerial.write(0x0D);
  mySerial.write(0x0A);
  delay(200);


/*

  mySerial.write(0x76);
  mySerial.write(0x6f);
  mySerial.write(0x6c);
  mySerial.write(0x77);
  mySerial.write(0x20);
  mySerial.write(0x32);
  mySerial.write(0x20);
  mySerial.write(0x6d);
  mySerial.write(0x6c);
  mySerial.write(0x0D);
  mySerial.write(0x0A);
*/
  /* DEBUG ONLY
  while (mySerial.available()==false);

  while (mySerial.available()) {
    char in=mySerial.read();
    Serial.print(in);} */
  delay(200);

  
  
  mySerial.write(0x76);
  mySerial.write(0x6f);
  mySerial.write(0x6c);
  mySerial.write(0x77);
  mySerial.write(0x20);
    if(thousands==0) mySerial.write(0x30);
    if(thousands==1) mySerial.write(0x31); 
    if(thousands==2) mySerial.write(0x32);
    if(thousands==3) mySerial.write(0x33);
    if(thousands==4) mySerial.write(0x34);
    if(thousands==5) mySerial.write(0x35);
    if(thousands==6) mySerial.write(0x36);
    if(thousands==7) mySerial.write(0x37);
    if(thousands==8) mySerial.write(0x38);
    if(thousands==9) mySerial.write(0x39);

    if(hundreds==0) mySerial.write(0x30);
    if(hundreds==1) mySerial.write(0x31);
    if(hundreds==2) mySerial.write(0x32);
    if(hundreds==3) mySerial.write(0x33);
    if(hundreds==4) mySerial.write(0x34);
    if(hundreds==5) mySerial.write(0x35);
    if(hundreds==6) mySerial.write(0x36);
    if(hundreds==7) mySerial.write(0x37);
    if(hundreds==8) mySerial.write(0x38);
    if(hundreds==9) mySerial.write(0x39);

    if(tens==0)  mySerial.write(0x30);
    if(tens==1)  mySerial.write(0x31);
    if(tens==2)  mySerial.write(0x32);
    if(tens==3)  mySerial.write(0x33);
    if(tens==4)  mySerial.write(0x34);
    if(tens==5)  mySerial.write(0x35);
    if(tens==6)  mySerial.write(0x36);
    if(tens==7)  mySerial.write(0x37);
    if(tens==8)  mySerial.write(0x38);
    if(tens==9)  mySerial.write(0x39);

    if(ones==0) mySerial.write(0x30);
    if(ones==1) mySerial.write(0x31);
    if(ones==2) mySerial.write(0x32);
    if(ones==3) mySerial.write(0x33);
    if(ones==4) mySerial.write(0x34);
    if(ones==5) mySerial.write(0x35);
    if(ones==6) mySerial.write(0x36);
    if(ones==7) mySerial.write(0x37);
    if(ones==8) mySerial.write(0x38);
    if(ones==9) mySerial.write(0x39);

  mySerial.write(0x20);
  mySerial.write(0x75);
  mySerial.write(0x6c);  
  mySerial.write(0x0D);
  mySerial.write(0x0A);


  
  gogo=false;

  
  curvol=curvol+vol;
  Serial.println(curvol);
  //DEBUG ONLY
  while (mySerial.available()==false);

  while (mySerial.available()) {
    char in=mySerial.read();
    Serial.print(in);}
    
    
  delay(200);
  if(pumprun()==0) return 0;
  int check=0;
        if(check==0){
       mySerial.write(0x65);
       mySerial.write(0x72);
       mySerial.write(0x72);
       mySerial.write(0x6f);
       mySerial.write(0x72);
       mySerial.write(0x3f);
       mySerial.write(0x0D);
       mySerial.write(0x0A);
       delay(200);
      while (mySerial.available()==false);
      int ch=0;
      while (mySerial.available()) {
        char in=mySerial.read(); 
        Serial.print(in);
         if((in=='6')||(in=='7')){
          Serial.println("STALL ERROR!!");pumpstop(); return 0;}
        if((in=='<')||(in=='>')){Serial.println("PUMP RUNNING");
          ch=1;}
        if(ch==1)
        check=0;
        else check=1;
      }Serial.println("WAITING");
     }
    delay(200);
    
 

  
  Serial.println(curvol);
return 1;
}


int pumpdis(int vol)
{ 
  gogo=false;
  int ones = (vol%10);
  int tens = ((vol/10)%10);
  int hundreds =  ((vol/100)%10);
  int thousands =  (vol/1000);
  Serial.print(thousands);Serial.print("  ");Serial.print(hundreds);
  Serial.print("  "); Serial.print(tens); Serial.print("  ");
  Serial.println(ones);
  
  if(curvol-vol<0){Serial.println("NEG VOLUME"); return 0;}

   mySerial.write(0x6d);
  mySerial.write(0x6f);
  mySerial.write(0x64);
  mySerial.write(0x65);
  mySerial.write(0x20);
  mySerial.write(0x69);
  mySerial.write(0x0D);
  mySerial.write(0x0A);

  /* DEBUG ONLY
  while (mySerial.available()==false);

  while (mySerial.available()) {
    char in=mySerial.read();
    Serial.print(in);} */
  delay(200);

  
  
  mySerial.write(0x76);
  mySerial.write(0x6f);
  mySerial.write(0x6c);
  mySerial.write(0x69);
  mySerial.write(0x20);
    if(thousands==0) mySerial.write(0x30);
    if(thousands==1) mySerial.write(0x31); 
    if(thousands==2) mySerial.write(0x32);
    if(thousands==3) mySerial.write(0x33);
    if(thousands==4) mySerial.write(0x34);
    if(thousands==5) mySerial.write(0x35);
    if(thousands==6) mySerial.write(0x36);
    if(thousands==7) mySerial.write(0x37);
    if(thousands==8) mySerial.write(0x38);
    if(thousands==9) mySerial.write(0x39);

    if(hundreds==0) mySerial.write(0x30);
    if(hundreds==1) mySerial.write(0x31);
    if(hundreds==2) mySerial.write(0x32);
    if(hundreds==3) mySerial.write(0x33);
    if(hundreds==4) mySerial.write(0x34);
    if(hundreds==5) mySerial.write(0x35);
    if(hundreds==6) mySerial.write(0x36);
    if(hundreds==7) mySerial.write(0x37);
    if(hundreds==8) mySerial.write(0x38);
    if(hundreds==9) mySerial.write(0x39);

    if(tens==0)  mySerial.write(0x30);
    if(tens==1)  mySerial.write(0x31);
    if(tens==2)  mySerial.write(0x32);
    if(tens==3)  mySerial.write(0x33);
    if(tens==4)  mySerial.write(0x34);
    if(tens==5)  mySerial.write(0x35);
    if(tens==6)  mySerial.write(0x36);
    if(tens==7)  mySerial.write(0x37);
    if(tens==8)  mySerial.write(0x38);
    if(tens==9)  mySerial.write(0x39);

    if(ones==0) mySerial.write(0x30);
    if(ones==1) mySerial.write(0x31);
    if(ones==2) mySerial.write(0x32);
    if(ones==3) mySerial.write(0x33);
    if(ones==4) mySerial.write(0x34);
    if(ones==5) mySerial.write(0x35);
    if(ones==6) mySerial.write(0x36);
    if(ones==7) mySerial.write(0x37);
    if(ones==8) mySerial.write(0x38);
    if(ones==9) mySerial.write(0x39);

  mySerial.write(0x20);
  mySerial.write(0x75);
  mySerial.write(0x6c);  
  mySerial.write(0x0D);
  mySerial.write(0x0A);
  curvol=curvol-vol;
  Serial.println(curvol);
  //DEBUG ONLY
  while (mySerial.available()==false);

  while (mySerial.available()) {
    char in=mySerial.read();
    Serial.print(in);}
    
    
  delay(200);
  if(pumprun()==0) return 0;
  else {Serial.println(curvol);  return 1; }
}

int pumprun()
{
  Serial.println("RUN");
  mySerial.write(0x72);
  
  mySerial.write(0x75);
  
  mySerial.write(0x6E);
  
  mySerial.write(0x0D);
  
  mySerial.write(0x0A);
  delay(100);
  
  while (mySerial.available()==false);

  while (mySerial.available()) {
    char in=mySerial.read();
    Serial.print(in);
    if(in=='E') 
    { delay(10); Serial.println("\nERROR DETECT");
       mySerial.write(0x65);
       mySerial.write(0x72);
       mySerial.write(0x72);
       mySerial.write(0x6f);
       mySerial.write(0x72);
       mySerial.write(0x3f);
       mySerial.write(0x0D);
       mySerial.write(0x0A);
       delay(100);
      while (mySerial.available()==false);
      int check=0;
      while (mySerial.available()) {
        char in=mySerial.read();
        Serial.print(in);
        check++; 
        if((check==0)&&((in=='2')||(in=='3')||(in=='6')||(in=='7'))){
          Serial.println("STALL ERROR!!");
           mySerial.write(0x73);
           mySerial.write(0x74);
          mySerial.write(0x6f);
          mySerial.write(0x70);
          mySerial.write(0x0D);
        return 0;
        }
      }
    }
  }
  return 1;
}



int pumpstop()
{
  emergency=1;
  Serial.println("!EMERGENCY STOP!");
  delay(200);
   mySerial.write(0x73);
   mySerial.write(0x74);
   mySerial.write(0x6f);
   mySerial.write(0x70);
   mySerial.write(0x0D);
   mySerial.write(0x0A);
   delay(200);
   return 1;

  
}
int pumpstopNE()
{
  
  delay(200);
   mySerial.write(0x73);
   mySerial.write(0x74);
   mySerial.write(0x6f);
   mySerial.write(0x70);
   mySerial.write(0x0D);
   mySerial.write(0x0A);
   delay(200);
   return 1;

  
}
void waitcase(){
   int check=0;
       
       mySerial.write(0x65);
       mySerial.write(0x72);
       mySerial.write(0x72);
       mySerial.write(0x6f);
       mySerial.write(0x72);
       mySerial.write(0x3f);
       mySerial.write(0x0D);
       mySerial.write(0x0A);
       delay(200);
      while (mySerial.available()==false);
      int ch=0;
      while (mySerial.available()) {
        char in=mySerial.read();
        Serial.print(in);
         if((in=='6')||(in=='7')){
          Serial.println("STALL ERROR!!");pumpstop();}
        if((in=='<')||(in=='>')){Serial.println("PUMP RUNNING");
          ch=1; gogo=false;}
        if(in==':') {Serial.println("PUMP STOPPED"); gogo=true;}
        if(ch==1)
        check=0;
        else check=1;
      }//Serial.println("WAITING");
     
}

void intstart(){
  delay(100);
  
 mySerial.write(0x72);
 mySerial.write(0x61);
 mySerial.write(0x74);
 mySerial.write(0x65);
 mySerial.write(0x77);
 mySerial.write(0x20);
 mySerial.write(0x35);
 mySerial.write(0x30);
 mySerial.write(0x30);
 mySerial.write(0x20);
 mySerial.write(0x75);
 mySerial.write(0x6c);
 mySerial.write(0x2f);
 mySerial.write(0x6d);
 mySerial.write(0x0D);
 mySerial.write(0x0A);
 
 delay(500);
 
 mySerial.write(0x72);
 mySerial.write(0x61);
 mySerial.write(0x74);
 mySerial.write(0x65);
 mySerial.write(0x69);
 mySerial.write(0x20);
 mySerial.write(0x32);
 mySerial.write(0x30);
 //mySerial.write(0x20);
 mySerial.write(0x30);
 mySerial.write(0x30);
 mySerial.write(0x20);
 mySerial.write(0x75);
 mySerial.write(0x6c);
 mySerial.write(0x2f);
 mySerial.write(0x6d);
 mySerial.write(0x0D);
 mySerial.write(0x0A);
  

  
}
  
void heat(){

   if(tank1==true)
  {
    Serial.println("TANK 1 ON");
     TK1.write(0);      //Rotate in 1 direction
    InputT1 = tempread(0);  //Tank 1 Element Sensor
    int Inside=tempread(1);  //Tank 1 Inside Sensor
    T1PID.Compute();    //PID Computation
    if(InputT1<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(Inside<SETT1)      //IF inside Temp is less than  TANK 1 SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputT1>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite( 2, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(2, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to 100 *F
      {
      Serial.println("   LOW");
      digitalWrite(2,LOW);
      } 
    }
    else {
    digitalWrite(2,LOW);         // Output for tank 1 to Digital Pin 2
    Serial.println("TEMP SAFETY TANK1");
    }
    //Debug Display
    Serial.print("Tank 1 Inside: "); Serial.print(Inside); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputT1); Serial.print("PWM  Element Temp:"); Serial.print(InputT1); Serial.println("*F");
    
  }
  if(tank2==true)
  {
    Serial.println("TANK 2 ON");
     TK2.write(0);      //Rotate in 1 direction
    InputT2 = tempread(2);  //Tank 2 Element Sensor
    int Inside=tempread(3);  //Tank 2 Inside Sensor
    T2PID.Compute();    //PID Computation
    if(InputT2<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(Inside<SETT2)      //IF inside Temp is less than  TANK 2 SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputT2>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite( 3, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(3, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to SETPOINT
      {
      Serial.println("   LOW");
      digitalWrite(3,LOW);
      } 
    }
    else {
    digitalWrite(3,LOW);         // Output for tank 2 to Digital Pin 3
    Serial.println("TEMP SAFETY TANK2");
    }
    //Debug Display
    Serial.print("Tank 2 Inside: "); Serial.print(Inside); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputT2); Serial.print("PWM  Element Temp:"); Serial.print(InputT2); Serial.println("*F");
    
  }
  if(syringe==true)
  {
    Serial.println("Syringe and Lines ON");
    InputS = tempread(4);  //Syringe Sensor
    SPID.Compute();    //PID Computation
    if(InputS<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(InputS<SETS)      //IF inside Temp is less than  SYRINGE SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputS>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite( 4, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(4, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to SETPOINT
      {
      Serial.println("   LOW");
      digitalWrite(4,LOW);
      } 
    }
    else {
    digitalWrite(4,LOW);         // Output for Syringe to Digital Pin 4
    Serial.println("TEMP SAFETY Syringe");
    }
    //Debug Display
    Serial.print("Syringe Temp: "); Serial.print(InputS); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputS); Serial.println("PWM"); 
    
    
    InputL1 = tempread(5);  //Tube 1 (L1) Sensor
    L1PID.Compute();    //PID Computation
    if(InputL1<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(InputL1<SETL1)      //IF inside Temp is less than  Tube 1 (L1) SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputL1>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite( 5, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(5, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to SETPOINT
      {
      Serial.println("   LOW");
      digitalWrite(5,LOW);
      } 
    }
    else {
    digitalWrite(5,LOW);         // Output for Tube 1 (L1) to Digital Pin 5
    Serial.println("TEMP SAFETY Line 1");
    }
    //Debug Display
    Serial.print("Tube 1 Temp: "); Serial.print(InputL1); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputL1); Serial.println("PWM"); 
    
    
    InputL2 = tempread(6);  //Tube 2 (L2) Sensor
    L2PID.Compute();    //PID Computation
    if(InputL2<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(InputL2<SETL2)      //IF inside Temp is less than  Tube 2 (L2) SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputL2>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite( 6, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(6, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to SETPOINT
      {
      Serial.println("   LOW");
      digitalWrite(6,LOW);
      } 
    }
    else {
    digitalWrite(6,LOW);         // Output for Tube 2 (L2) to Digital Pin 6
    Serial.println("TEMP SAFETY Tube 2");
    }
    //Debug Display
    Serial.print("Tube 2 Temp: "); Serial.print(InputL2); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputL2); Serial.println("PWM"); 
    
    
    
    InputL3 = tempread(7);  //Tube 3 (L3) Sensor
    L3PID.Compute();    //PID Computation
    if(InputL3<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(InputL3<SETL3)      //IF inside Temp is less than  Tube 3 (L3) SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputL3>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite( 7, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(7, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to SETPOINT
      {
      Serial.println("   LOW");
      digitalWrite(7,LOW);
      } 
    }
    else {
    digitalWrite(7,LOW);         // Output for Tube 3 (L3) to Digital Pin 7
    Serial.println("TEMP SAFETY Tube 3");
    }
    //Debug Display
    Serial.print("Tube 3 Temp: "); Serial.print(InputL3); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputL3); Serial.println("PWM"); 
    
    
    InputC = tempread(8);  //Chamber Sensor
    CPID.Compute();    //PID Computation
   if(InputC<150)       //SAFETY If under 150*F OUTPUT HEAT
    {
      if(InputC>SETC)      //IF inside Temp is Greater than  Chamber SETPOINT
       {
        Serial.print("ELEMENT");
        if(OutputC>250)  //IF PID is "ON" Turn on otherwise off
          {
          Serial.println("   HIGH");
          digitalWrite(8, HIGH); 
          }
        else 
          {
        Serial.println("   LOW");
        digitalWrite(8, LOW);
          }
      }
      else              //IF inside Temp is greater than or equal to SETPOINT
      {
      Serial.println("   LOW");
      digitalWrite(8,LOW);
      } 
    }
    else {
    digitalWrite(8,LOW);         // Output for Chanber to Digital Pin 8
    Serial.println("TEMP SAFETY Chamber");
    }
    //Debug Display
    Serial.print("Chamber Temp: "); Serial.print(InputC); Serial.print(" *F"); Serial.print("Output: "); Serial.print(OutputC); Serial.println("PWM"); 
    
    
  }
}

