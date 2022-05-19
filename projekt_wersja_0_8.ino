/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   Olav Kallhovd sept2017
   -------------------------------------------------------------------------------------
*/

/*
   Settling time (number of samples) and data filtering can be adjusted in the config.h file
   For calibration and storing the calibration value in eeprom, see example file "Calibration.ino"

   The update() function checks for new data and starts the next conversion. In order to acheive maximum effective
   sample rate, update() should be called at least as often as the HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS.
   If you have other time consuming code running (i.e. a graphical LCD), consider calling update() from an interrupt routine,
   see example file "Read_1x_load_cell_interrupt_driven.ino".

   This is an example sketch on how to use this library
*/

// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
/// \author  Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2009 Mike McCauley
// $Id: ConstantSpeed.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <HX711_ADC.h>
#include <EEPROM.h>
#include <AccelStepper.h>
#define motorPin1  8                // IN1 pin on the ULN2003A driver
#define motorPin2  9                // IN2 pin on the ULN2003A driver
#define motorPin3  10               // IN3 pin on the ULN2003A driver
#define motorPin4  11    

int stepsPerRevolution = 64;        // steps per revolution
float degreePerRevolution = 5.625;  // degree per revolution
/*
 * AccelStepper::FULL2WIRE (2) means: 2 wire stepper (2 pins needed). 
 * AccelStepper::FULL3WIRE (3) means: 3 wire stepper, like a harddisk motor (3 pins needed). 
 * AccelStepper::FULL4WIRE (4) means: 4 wire stepper (4 pins needed). 
 * AccelStepper::HALF3WIRE (6) means: 3 wire half stepper, like a harddisk motor (3 pins needed) 
 * AccelStepper::HALF4WIRE (8) means: 4 wire half stepper (4 pins needed) 
 *
 * AccelStepper uses AccelStepper::FULL4WIRE (4 pins needed) by default.
 */
AccelStepper stepper(AccelStepper::HALF4WIRE, motorPin1, motorPin3, motorPin2, motorPin4);
//waga smoot
float total=0;


//LCD config
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> //Download here: https://electronoobs.com/eng_arduino_liq_crystal.php
LiquidCrystal_I2C lcd(0x27 ,20,4); //sometimes the LCD adress is not 0x3f. Change to 0x27 if it dosn't work.
// Include Keypad library
#include <Keypad.h>

//pins:

//pins:
const int HX711_dout = 4; //mcu > HX711 no 1 dout pin
const int HX711_sck = 5; //mcu > HX711 no 1 sck pin


//star wars 


const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;


//buzer
const int buzzerPin = 7;


//HX711 constructor (dout pin, sck pin)
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;

unsigned long t = 0;
unsigned long t2 = 0;
volatile boolean newDataReady;


// Length of password + 1 for null character
#define Password_Length 8
// Character to hold password input
char Data[Password_Length];
float wagaWprowadzona=0;
bool wprowadzanie =true;
bool startPressed=false;
bool grainy= true;
float wagaNaWadze1;
float wagaNaWadze2;
float wagaCalkowita;


// Counter for character entries
byte data_count = 0;

// Character to hold key input
char customKey;

// Constants for row and column sizes
const byte ROWS = 4;
const byte COLS = 4;

// Array to represent keys on keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'.', '0', '#', 'D'}
};

// Connections to Arduino
byte rowPins[ROWS] = {44, 45, 46, 47};
byte colPins[COLS] = {48, 49, 50, 51}; 


// Create keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


void setup() {
  pinMode(buzzerPin, OUTPUT);
  lcd.init();                         //Init the LCD
  lcd.backlight();    
    lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Startuje ");

  


 

stepper.setMaxSpeed(1000);
     stepper.setAcceleration(100.0); 
   stepper.setSpeed(1000);  





  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive


  float calibrationValue; // calibration value

  EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the value from eeprom

  unsigned long stabilizingtime = 5000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
        lcd.clear();
        lcd.setCursor(10, 1);
        lcd.print("GRAIN");
}







void loop() {

  
  static boolean newDataReady = 0;
  const int serialPrintInterval = 500; //increase value to slow down serial print activity
  const int serialPrintInterval2 = 0;
  

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;
  
  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {

  wagaCalkowita = LoadCell.getData(); 
  
  if(wagaCalkowita<0){
   wagaCalkowita=0; 
   }
   
newDataReady=0;


    lcd.setCursor(0,1);
    
if(grainy){
wagaCalkowita=wagaCalkowita*15.43;
     lcd.print(wagaCalkowita);
            lcd.print("   ");
  }else{
     lcd.print(wagaCalkowita);
          lcd.print("   ");
    }
      newDataReady = 0;
      t = millis();
    }
  }
    if (millis() > t2 + serialPrintInterval2) {
      
          
// Look for keypress
  customKey = customKeypad.getKey();

  if (customKey&&customKey!='A'&&customKey!='B'&&customKey!='C'&&customKey!='D'&&customKey!='#'&&wprowadzanie) {
     beep(a, 250); 
if(data_count<=5){
    // Enter keypress into array and increment counter
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 0);
    lcd.print(Data[data_count]);
     //  lcd.print(" g"); 
    data_count++;
}
  }
  else if(customKey=='A'){
     beep(f, 250); 
    //start stop
    
        if(!wprowadzanie) {
          startPressed=!startPressed;
          

        
          }
    
  }else if(customKey=='B'){
         beep(f, 250); 
//Tarowanie
if(!startPressed){
LoadCell.tareNoDelay();

  //check if last tare operation is complete
  if (LoadCell.getTareStatus() == true) {
    
 lcd.setCursor(7, 0);
    lcd.print("TAROWANIE");
    delay(1000);
         lcd.setCursor(7, 0);
        lcd.print("         ");
  }
  }





        

  }
  
  
  else if(customKey=='C'){
         beep(f, 250); 
grainy=!grainy;

if(grainy&&!startPressed){
  
      lcd.setCursor(10, 1);
    lcd.print("GRAIN");
  }else{
          lcd.setCursor(10, 1);
    lcd.print("Gram   ");

    if(!grainy&&!startPressed){
   if( wagaWprowadzona>  50){
        lcd.setCursor(0, 0);
         lcd.print("MAX TO 50");
         delay(1000);
             lcd.setCursor(0, 0);
         wagaWprowadzona=50;}
          lcd.print(wagaWprowadzona);
      lcd.print("               ");
      }

    
    }

   }
   
   
   else if(customKey=='D'){
         beep(f, 250); 
          //clear
               //lcd.print("     d"); 

    
clearAll();
                  
   }else if(customKey=='#'){
         beep(f, 250); 
          //eter  

       wprowadzanie=false;

            //     lcd.clear();
                 
  Data[Password_Length] = '\0';    
  wagaWprowadzona = atof(Data);  
    lcd.setCursor(0, 0);
    if(grainy){
   if( wagaWprowadzona>  771.5){
        lcd.setCursor(0, 0);
         lcd.print("MAX TO 771.5     ");
         delay(1000);
             lcd.setCursor(0, 0);
         wagaWprowadzona=771.5;}
      }else{
           if( wagaWprowadzona>  50){
        lcd.setCursor(0, 0);
         lcd.print("MAX TO 50        ");
         
              delay(1000);
             lcd.setCursor(0, 0);
         wagaWprowadzona=50;
        }
        }
 lcd.print(wagaWprowadzona);
      lcd.print("               ");
          }
      
      
      }


if(startPressed&&wagaWprowadzona>=wagaCalkowita){  
  
  if(wagaWprowadzona-wagaCalkowita>2){
stepper.runSpeed();
}else{
     stepper.setSpeed(300);  
stepper.runSpeed();

     
  }

}else if(startPressed&&wagaWprowadzona<=wagaCalkowita){
                                lcd.clear();
                  lcd.print("Zrobione"); 
         
shortSong();
  clearAll();
  }

  //tarowanie zrobione
  if (LoadCell.getTareStatus() == true) {
    
 lcd.setCursor(7, 0);
    lcd.print("TAROWANIE");
    delay(1000);
         lcd.setCursor(7, 0);
        lcd.print("         ");
  }
}


void clearData() {
  // Go through array and clear data
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}

void beep(int note, int duration)
{
 //Play tone on buzzerPin
 tone(buzzerPin, note, duration);
  delay(duration);

 
 //Stop tone on buzzerPin
 noTone(buzzerPin);
 
 delay(50);

}

void firstSection()
{
 beep(a, 500);
 beep(a, 500); 
 beep(a, 500);
 beep(f, 350);
 beep(cH, 150); 
 beep(a, 500);
 beep(f, 350);
 beep(cH, 150);
 beep(a, 650);
 
 delay(500);
 
 beep(eH, 500);
 beep(eH, 500);
 beep(eH, 500); 
 beep(fH, 350);
 beep(cH, 150);
 beep(gS, 500);
 beep(f, 350);
 beep(cH, 150);
 beep(a, 650);
 
 delay(500);
}

void shortSong()
{
 beep(a, 500);
 beep(a, 500); 
 beep(a, 500);
 beep(f, 350);
 beep(cH, 150); 
 beep(a, 500);
 beep(f, 350);
 beep(cH, 150);
 beep(a, 650);
 
}

 
void secondSection()
{
 beep(aH, 500);
 beep(a, 300);
 beep(a, 150);
 beep(aH, 500);
 beep(gSH, 325);
 beep(gH, 175);
 beep(fSH, 125);
 beep(fH, 125); 
 beep(fSH, 250);
 
 delay(325);
 
 beep(aS, 250);
 beep(dSH, 500);
 beep(dH, 325); 
 beep(cSH, 175); 
 beep(cH, 125); 
 beep(b, 125); 
 beep(cH, 250); 
 
 delay(350);
}

void secondSectionV2()
{
 //Variant 1
 beep(f, 250); 
 beep(gS, 500); 
 beep(f, 350); 
 beep(a, 125);
 beep(cH, 500);
 beep(a, 375); 
 beep(cH, 125);
 beep(eH, 650);
 
 delay(500);
}


void secondSectionV21()
{
 //Variant 2
 beep(f, 250); 
 beep(gS, 500); 
 beep(f, 375); 
 beep(cH, 125);
 beep(a, 500); 
 beep(f, 375); 
 beep(cH, 125);
 beep(a, 650); 
 
 delay(650);
}
void clearAll(){

                 wprowadzanie=true;
               startPressed=false;
                 lcd.clear();
                  lcd.print("Clear"); 
                   clearData();
         //Data[Password_Length] = '\0';    
  //wagaWprowadzona = atof(Data);
memset(Data, 0, sizeof(Data));
  wagaWprowadzona=0;
 delay(600);                 
 lcd.clear();                   
      lcd.setCursor(10, 1);
    lcd.print("GRAIN");
  grainy=true;

  }
