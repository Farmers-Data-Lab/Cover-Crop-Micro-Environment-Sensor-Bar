// Add this code to a .ino Arduino file for an Arduino Mega
// Script to combine all sensors and Data Logger
// 1x 6 sensor Quantum light bar for bottom light, Analog Pin A11
// 1x Light quantum PAR sensor for top light, Analg pin - A9
// 1x STEVENS Hydraprobe Soil Sensor (RS-485 version), Analog Pin A7
// 2x Potentiometer for feeding in the Plot number information - Analog pins A-12, A-10
// 1x SD Card Data Logger Shield
// 1x Adafruit Capacitive touchscreen
// 2x Pushbutton, ,write button(GREEN) - Digital Pin 2, delete button(RED) - Digital Pin 2
// 1x Buzzer Digital PIN - 24

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include <Arduino.h>
// Date and time functions using a PCF8523 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_PCF8523 rtc;

int sendreadpin = 22;
// Photocell Resistor Variables
int photocellPin = A11; // Bottom Light sensor PIN
int photocellReading = 0; //Bottom light sensor reading
int buzzer =24;
int top_lightPin = A9; // Top Light Sensor
int top_lightReading = 0; // Top Light Sensor reading

int number_of_readings=0; // number of readings for current plot number since device turned on
uint32_t prev_position=0;
int moisturePin = A7; // Define soil moisture analog pin
int moistureReading = 0; // Define soil moisture reading variable
//***********************************************keypad and potentio
int day =0;
int month = 0;
int year = 0;
String HH ="0";
String MM ="0";
int buttonStateA = 0;
int Plot_number = 0; //plot number from potentiometer
int Prev_Plot_number = 0; //plot number from potentiometer
float soil_moist = 0;
int top_light = 0;
int num_of_readings =0;
#define keyPressDelay 120
int analogPin = A12; // potentiometer wiper (middle terminal) connected to analog pin 12
int analogPin2 = A10; // potentiometer wiper (middle terminal) connected to analog pin 10
                    // outside leads to ground and +5V
int val = 0;  // variable to store the value read
int plot_subsection=0;
float temp_flo;
int prev_val[10];
unsigned int start_positions[750];
// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 ts = Adafruit_FT6206();
//Keyboard_ Keyboard;

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


#define button_1_X 0
#define button_1_Y 0
#define button_1_W FRAME_W
#define button_1_H FRAME_H

#define button_2_X 152
#define button_2_Y 0
#define button_2_W FRAME_W
#define button_2_H FRAME_H

//************************************************************soil moisture sensor stuff**************************
float SoilMoisture_wfv = 0.0;
float BulkEC_TempCorr =0.0;
float Temp_C =0.0;
float Temp_F =0.0;
float BulkEC =0.0;
float DielecPerm_real =0.0;
float DielecPerm_imag =0.0;
float PoreWaterEC =0.0;
float DielecLossTan =0.0;
//int Plot_number=0;
const int buttonPin = 30;          // Pin connected to the pushbutton
const int RS485ControlPin = 5;   // Pin to control RE and DE (if needed)
const int delete_buttonPin = 36;  
bool buttonPressed = false;

//****************************************************************************************************************


// Pushbutton variables
//const int button = 22; //not used anymore
int buttonFlag = 0;

// BH1750 Light Sensor Variables
//hp_BH1750 BH1750; // Create the BH1750 light sensor
float lux = 0.0;

// SD Card Writer
// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 8;
// the logging file
File logfile;

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);

  while(1);
}

String filename = "L00.CSV";



void setup() {
  pinMode(buzzer,OUTPUT);        
  digitalWrite(buzzer,LOW);

//******************************RTC stuff ***********************************

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    //
    // Note: allow 2 seconds after inserting battery or applying external power
    // without battery before calling adjust(). This gives the PCF8523's
    // crystal oscillator time to stabilize. If you call adjust() very quickly
    // after the RTC is powered, lostPower() may still return true.
  }
 rtc.start();
  float drift = 43; // seconds plus or minus over oservation period - set to 0 to cancel previous calibration.
  float period_sec = (7 * 86400);  // total obsevation period in seconds (86400 = seconds in 1 day:  7 days = (7 * 86400) seconds )
  float deviation_ppm = (drift / period_sec * 1000000); //  deviation in parts per million (μs)
  float drift_unit = 4.34; // use with offset mode PCF8523_TwoHours
  // float drift_unit = 4.069; //For corrections every min the drift_unit is 4.069 ppm (use with offset mode PCF8523_OneMinute)
  int offset = round(deviation_ppm / drift_unit);
  // rtc.calibrate(PCF8523_TwoHours, offset); // Un-comment to perform calibration once drift (seconds) and observation period (seconds) are correct
  // rtc.calibrate(PCF8523_TwoHours, 0); // Un-comment to cancel previous calibration

  Serial.print("Offset is "); Serial.println(offset); // Print to control offset

//****************************************************************************
//Wire.begin();
pinMode(22, OUTPUT);

  Serial.begin(9600);
number_of_readings=0;
//  pinMode(button, INPUT); // Setup push button pin
//************************************************************soil moisture sensor stuff**************************

 // Initialize Serial1 for RS485 communication
  Serial1.begin(9600); // Adjust baud rate as necessary for your sensor
Serial2.begin(9600);
Serial3.begin(9600);
  // Set up the button pin
  pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(delete_buttonPin, INPUT_PULLUP); // Use internal pull-up resistor

  // Set up the RS485 control pin if needed
  pinMode(RS485ControlPin, OUTPUT);
  digitalWrite(RS485ControlPin, LOW); // Start in receive mode

  //additional setup stuff required to read on boot up(what is the reason?)
   // Send the command to take a new reading
      sendCommand("000TR");
      //delay(100); // Short delay between commands

      // Send the command to request the values
      sendCommand("000T0");
      //delay(100); // Wait for the response


//****************************************************************************************************************
  // Setup BH1750 sensor
  //if(!BH1750.begin(BH1750_TO_GROUND)){
   // Serial.println("BH1750 not found!");
   // while (true) {} // halt program code
 // }
 // BH1750.start();

  // SD Card Setup
  pinMode(chipSelect, OUTPUT);

  if (!SD.begin(chipSelect)) {
    error("SD failed");//********
  }
  Serial.println("SD Initialized.");

  // create a new file
  for (uint8_t i = 0; i < 100; i++) {
    filename[1] = i/10 + '0';
    filename[2] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }

  if (! logfile) {
    Serial.println("Cannot write a new file. File capacity reached (100/100)");//**********
    while(1);//**********
  }

  Serial.print("Logging to: ");
  Serial.println(filename);

  // Add Data Headers to file
  logfile.println("Plot Number,Number of Readings,Above Canopy PAR,Ground PAR,Soil Moiture, BulkEC_TempCorr, Temp_C, Temp_F,BulkEC, DielecPerm_real, DielecPerm_imag, PoreWaterEC, DielecLossTan,month,day,year,Hours,Minutes");
  logfile.flush();


//***************************************************************************************
 read_photocells();
      read_top_light();
      read_moisture_probe();
      read_moisture_probe();
       //Serial.println(Serial2.availableForWrite());
//Serial2.print(String(SoilMoisture_wfv));
//Serial2.print("-");
//Serial2.print(String(number_of_readings));
//Serial2.print("-");
//Serial2.print(String(top_lightReading));
//Serial.println(Serial2.availableForWrite());
//Serial2.flush();
      read_plot_number();
      // print_readings(); // Print readings to serial monitor for debugging.
      //Serial2.write(SoilMoisture_wfv);
      // Write all readings to SD card
      print_readings();


//*****************************************************************************************
//**********keypad stuff

 tft.begin();

  if (!ts.begin(40)) {
    //    Serial.println("Unable to start touchscreen.");
  }
  else {
    //    Serial.println("Touchscreen started.");
  }

  // Set background color
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);

  // origin = left,top landscape (USB left upper)
  tft.setRotation(0);

  // Run once to set rect on screen initially
  button_1();


}

void loop() { 


//******************main loop stuff


 //get_date_time();
 //print_readings();

  int tempr = SoilMoisture_wfv*100;
  //Serial2.print(tempr);
  buttonFlag = 0;
//Serial.print("running");
  // Button loop. Writes data once per press.
  while (digitalRead(buttonPin) == LOW){
    if (buttonFlag == 0){
      // Read sensors
      if(Plot_number!=Prev_Plot_number)
      {number_of_readings=0;}
     get_date_time();
      read_photocells();
      read_top_light();
      read_moisture_probe();
       read_moisture_probe();
      number_of_readings ++;
      write_data_to_screen();
      Prev_Plot_number = Plot_number;
      read_plot_number();
      // print_readings(); // Print readings to serial monitor for debugging.
      //Serial2.write(SoilMoisture_wfv);
      // Write all readings to SD card
      print_readings();

      write_readings();
      logfile.flush();
      
       analogWrite(22, number_of_readings);
  uint8_t tempr = 100;



      
      // Only take one sample per button press - Could upgrade to ISR if needed
      buttonFlag = 1;
      Serial.print("number of readings takes : ");
      Serial.print(number_of_readings);
      Prev_Plot_number = Plot_number;
      digitalWrite(buzzer,HIGH);
      delay(100);
      digitalWrite(buzzer,LOW);
    }

  }

  // Delete Button loop. deletes data once per press.
  while (digitalRead(delete_buttonPin) == LOW){
    if (buttonFlag == 0){
         if(number_of_readings>0){
          number_of_readings --;
       logfile.seek(start_positions[number_of_readings]);
        
      write_data_to_screen();
      // Write all readings to SD card
      write_delete_readings();
      logfile.flush();
      
      // Only take one sample per button press - Could upgrade to ISR if needed
      buttonFlag = 1;
      Serial.print("number of readings takes : ");
      Serial.print(number_of_readings);
      digitalWrite(buzzer,HIGH);
      delay(100);
      digitalWrite(buzzer,LOW);
      delay(100);
      digitalWrite(buzzer,HIGH);
      delay(100);
      digitalWrite(buzzer,LOW);
      delay(100);
      digitalWrite(buzzer,HIGH);
      delay(100);
      digitalWrite(buzzer,LOW);
         }
         


    }
  }
//***********************keypad loop stuff****************

 button_1();
 
//num_of_readings=analogRead(A14);
val = analogRead(analogPin);  // read the input pin
int temp = val>>6;
if(temp>9){temp=9;}
plot_subsection=temp;
val = analogRead(analogPin2);  // read the input pin
temp = val>>5;
if(temp>20){temp=20;}

Plot_number=plot_subsection*100 + temp;
 // Serial.println(Serial2.available());
 
     
   
   
   


//****************************

}

//Function to write screen
void button_1()
{if(prev_val[0]!=Plot_number || prev_val[5]!=plot_subsection){
  tft.fillRect(button_1_X +4, button_1_Y +20, button_1_X +240 , button_1_Y +90, ILI9341_BLACK);}
prev_val[0]=Plot_number;
prev_val[5]=plot_subsection;
  tft.setCursor(button_1_X + 4 , button_1_Y + 4);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.println("Current Plot number: ");
   tft.setCursor(button_1_X + 4 , button_1_Y + 20);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(11);
  tft.println(Plot_number);
  tft.setCursor(button_1_X + 100 , button_1_Y + 20);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(7);
  //tft.println("");
  
  tft.setCursor(button_1_X + 130 , button_1_Y + 20);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(9);
  //tft.println(plot_subsection);
  tft.setCursor(button_1_X + 4  , button_2_Y + 110);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Last reading...");
 tft.setCursor(button_1_X +4  , button_2_Y + 140);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Plot:");
  if(prev_val[1]!=Prev_Plot_number){}
 // tft.fillRect(button_1_X +5, button_1_Y +140, button_1_X +240 , button_1_Y +260, ILI9341_BLACK);}
prev_val[1]=Prev_Plot_number;
  tft.setCursor(button_1_X + 65 , button_2_Y + 140);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println(Prev_Plot_number);
  tft.setCursor(button_1_X + 4 , button_2_Y + 170);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Soil moist:");
  if(temp_flo!=soil_moist){}
  //tft.fillRect(button_1_X +140, button_1_Y +170, button_1_X +240 , button_1_Y +190, ILI9341_BLACK);}
temp_flo=soil_moist;
  tft.setCursor(button_1_X + 140 , button_2_Y + 170);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print(soil_moist);
   tft.setCursor(button_1_X + 4 , button_2_Y + 200);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Upper PAR:");
   tft.setCursor(button_1_X + 135 , button_2_Y + 200);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print(top_light);
  if(prev_val[7]!=top_light){}
  //tft.fillRect(button_1_X +135, button_1_Y +140, button_1_X +220 , button_1_Y +160, ILI9341_BLACK);}
prev_val[7]=top_light;
    tft.setCursor(button_1_X + 4 , button_2_Y + 230);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("To Filename=>");
   tft.setCursor(button_1_X + 170 , button_2_Y + 230);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.print(filename);
    tft.setCursor(button_1_X + 110 , button_2_Y + 140);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Reads:");
 // if(prev_val[3]!=num_of_readings){
  //tft.fillRect(button_1_X +185, button_2_Y +230, button_1_X +240 , button_1_Y +245, ILI9341_BLACK);}
prev_val[3]=num_of_readings;
    tft.setCursor(button_1_X + 185 , button_2_Y + 140);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print(num_of_readings);
   tft.setCursor(button_1_X + 4 , button_2_Y + 260);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print(month);
  tft.setCursor(button_1_X + 29 , button_2_Y + 260);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("/");
    tft.setCursor(button_1_X + 45 , button_2_Y + 260);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print(day);
  tft.setCursor(button_1_X + 69 , button_2_Y + 260);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("/");
    tft.setCursor(button_1_X + 85 , button_2_Y + 260);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print(year);
  tft.setCursor(button_1_X + 4 , button_2_Y + 290);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Time:");
  tft.setCursor(button_1_X + 75 , button_2_Y + 290);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print(HH);
   tft.setCursor(button_1_X + 99 , button_2_Y + 290);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print(":");
   tft.setCursor(button_1_X + 112 , button_2_Y + 290);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print(MM);
}




// Function reads all photocells
void read_photocells(){
  // Read all photocell resistors
  photocellReading = (4.5)*analogRead(photocellPin); // Read photoresistor 1

}

void write_data_to_screen(){
   tft.fillRect(button_1_X, button_1_Y +140, button_1_X +240 , button_1_Y +260, ILI9341_BLACK);
soil_moist = SoilMoisture_wfv;
num_of_readings = number_of_readings;

top_light = top_lightReading;

}

void get_date_time () {
 DateTime now = rtc.now();
  year = now.year();
  
    month = now.month();
   
   day = now.day();
    
    HH = now.hour();
     MM = now.minute();
  
}

void read_plot_number(){
  

  if(Serial3.available()){
   
   
   char buffer[64];
   String tempr;
    tempr=Serial3.readString();
    for(int u=0;u<64;u++){
    buffer[u] = tempr[u];
    
    }
       //soil_moist=atof(buffer);
    //Serial.println(soil_moist);
    char* token = strtok(buffer, " - ");
 
    // Keep printing tokens while one of the
    // delimiters present in str[].
    int index =0;
    while(token != NULL ) {
       Serial.println(token);
       
        float temp;
        temp=atof(token);
       if(index==1){Plot_number=temp;}
      
        token = strtok(NULL, " - ");
       index++;
    }}
}
// Function that reads the BH1750 light sensor (ambient sensor) 
void read_top_light(){
  top_lightReading = (4.5)*analogRead(top_lightPin); // Read photoresistor 1
}

void write_readings() {
  // Write all readings to the SD card
       start_positions[number_of_readings] = logfile.position();
       logfile.print(String(Plot_number) + ",");
       logfile.print(String(number_of_readings) + ",");
  logfile.print(String(top_lightReading) + ",");
  // string data;
  
  logfile.print(String(photocellReading) + ",");
  
  logfile.print(String(SoilMoisture_wfv) + ",");
  logfile.print(String(BulkEC_TempCorr) + ",");
  logfile.print(String(Temp_C) + ",");
  logfile.print(String(Temp_F) + ",");
  logfile.print(String(BulkEC) + ",");
  logfile.print(String(DielecPerm_real) + ",");
  logfile.print(String(DielecPerm_imag) + ",");
  logfile.print(String(PoreWaterEC) + ",");
  logfile.print(String(DielecLossTan) + ",");
  logfile.print(String(month) + ",");
  logfile.print(String(day) + ",");
  logfile.print(String(year) + ",");
  logfile.print(String(HH) + ",");
  logfile.print(String(MM) + ",");
  logfile.println("");
  delay(10);
  // logfile.flush();
  delay(15);
}

void write_delete_readings() {
  // Write all readings to the SD card
       prev_position = logfile.position();
  logfile.print(String("Deleted") + ",");
  // string data;
  logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");
  
  logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");
   logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");
    logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");
  logfile.print(String("Deleted") + ",");

  logfile.println("");
  delay(10);
  // logfile.flush();
  delay(15);
}


void print_readings() {
  // Print all readings to the Serial Monitor
  Serial.print(String(top_lightReading) + ",");
  Serial.print(String(photocellReading) + ",");
  Serial.print(String(SoilMoisture_wfv) + ",");
  Serial.print(String(BulkEC_TempCorr) + ",");
  Serial.print(String(Temp_C) + ",");
  Serial.print(String(Temp_F) + ",");
  Serial.print(String(BulkEC) + ",");
  Serial.print(String(DielecPerm_real) + ",");
  Serial.print(String(DielecPerm_imag) + ",");
  Serial.print(String(PoreWaterEC) + ",");
  Serial.print(String(DielecLossTan) + ",");
  Serial.println(String(Plot_number));
    Serial.println(year);
    //Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(month);
    Serial.print('/');
    Serial.print(day);
   Serial.println();
    Serial.print(HH);
    Serial.print(':');
    Serial.print(MM);
    
    
  Serial.println("");
}


//************************************************************soil moisture sensor stuff**************************


void sendCommand(const char* command) {
  // Enable transmission mode if needed
  digitalWrite(RS485ControlPin, HIGH);
 // delay(1); // Ensure the pin has time to switch

  // Send command to the sensor
  Serial2.print(command);
  Serial2.print("\r\n"); // Append CR and LF to the command

  // Brief delay to ensure the command is sent completely
 // delay(10);

  // Switch back to receive mode if needed
  digitalWrite(RS485ControlPin, LOW);
  //delay(1); // Ensure the pin has time to switch
}

String readResponse() {
  String response = "";
  unsigned long startTime = millis();

  // Wait for a response with a timeout
  while (millis() - startTime < 500) { // 500 ms timeout
    if (Serial2.available()) {
      char receivedChar = Serial2.read();
      response += receivedChar;
    }
  }

  return response;
}

// Function that reads moisture sensor
void read_moisture_probe(){
   // Send the command to take a new reading
      sendCommand("000TR");
     // delay(100); // Short delay between commands

      // Send the command to request the values
      sendCommand("000T0");
      //delay(100); // Wait for the response

      // Read and process the response
      String response = readResponse();

      // Print the response to the Serial Monitor including the address
      Serial.println("Received: " + response);

      // Remove the address prefix ("000")
      if (response.startsWith("000")) {
        response = response.substring(3);
      }

      // Process the response (assuming it's a series of 9 comma-separated numbers)
      char responseArray[response.length() + 1];
      response.toCharArray(responseArray, response.length() + 1);

      // Variable names in order
      const char* variableNames[] = {
        "SoilMoisture_wfv", "BulkEC_TempCorr", "Temp_C", "Temp_F", 
        "BulkEC", "DielecPerm_real", "DielecPerm_imag", 
        "PoreWaterEC", "DielecLossTan"
      };
       float temp;
      char* token = strtok(responseArray, ",");
      int index = 0;
      while (token != NULL && index < 9) {
        Serial.print(variableNames[index]);
        Serial.print(": ");
        temp = atof(token);

        if(index==0){SoilMoisture_wfv = temp;}
        if(index==1){BulkEC_TempCorr = temp;}
        if(index==2){Temp_C = temp;}
        if(index==3){Temp_F = temp;}
        if(index==4){BulkEC = temp;}
        if(index==5){DielecPerm_real = temp;}
        if(index==6){DielecPerm_imag = temp;}
        if(index==7){PoreWaterEC = temp;}
        if(index==8){DielecLossTan = temp;}
        
        

        Serial.println(temp);
        token = strtok(NULL, ",");
        index++;
      }
}


//****************************************************************************************************************
