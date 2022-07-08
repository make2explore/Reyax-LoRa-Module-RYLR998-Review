/* ---------------------------------- make2explore.com----------------------------------------------------*/
// Project           - Reyax LoRa Module RYLR998 Testing - IoT mini Project - Remote GreenHouse Monitoring
// Created By        - blog@make2explore.com
// Version - 1.0     - Sender Code
// Last Modified     - 31/05/2022 20:36:00 @admin
// Software          - C/C++, Arduino IDE, ESP8266 SDK, Libraries
// Hardware          - NodeMCU ESP8266, Reyax LoRa RYLR998 Module, DHT22, 0.96 OLED Display, Relay Module              
// Sensors Used      - DHT22 Temperature and Humidity Sensor, Soil Moisture Sensor
// Source Repo       - github.com/make2explore
/* -------------------------------------------------------------------------------------------------------*/


// LoRa Module Configurations Transmitter Node
String lora_band = "865000000";     // Enter LoRa band as per your country
String lora_networkid = "7";        // Enter Lora Network ID - Must be same for Tx and Rx Nodes
String lora_TX_address = "126";     // Enter Lora RX address

#include <SoftwareSerial.h>         // include Software Serial library
#include <Wire.h>
#include <Adafruit_GFX.h>           // include Adafruit Graphics library
#include <Adafruit_SSD1306.h>       // include OLED Display library
#include "DHT.h"                    // include DHT Sensor library

const byte rxPin = D7;              // Software Serial Pins Rx and Tx
const byte txPin = D8;

const int sensor_pin = A0;          // Connect Soil moisture analog sensor pin to A0 of NodeMCU
const int Relay1_Pump = D6;         // Connect Relay to D6 of NodeMCU

// Set up a new SoftwareSerial object
SoftwareSerial sendLoRa (rxPin, txPin);   // Software Serial Pins Rx=D7 and Tx=D8

// Set up a new Display object
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// make2explore Logo Bitmap byte array
static const uint8_t image_data_096OLEDscreens[1024] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0xe0, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x3f, 0xfc, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xff, 0xfe, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0xff, 0xff, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc7, 0xff, 0x3f, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xff, 0x3f, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1f, 0xfe, 0x1f, 0xc0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x3f, 0xf8, 0x07, 0xc0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x7f, 0xf8, 0x07, 0xc0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x7f, 0xfe, 0x1f, 0xc0, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xff, 0xff, 0x3f, 0xc0, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xff, 0xff, 0x3f, 0xc0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xff, 0xff, 0xff, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xff, 0xff, 0xff, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0xff, 0xff, 0xfe, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0xff, 0xff, 0xf8, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0xff, 0xff, 0xe0, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0xff, 0xf8, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0xff, 0xe0, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0xff, 0xc0, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0xff, 0x80, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xff, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xfe, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xfe, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xfe, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x7e, 0x07, 0xf8, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x7e, 0x07, 0xf8, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x3e, 0x07, 0xf8, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1e, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8e, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc7, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x80, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define DHTPIN D5           // Digital pin connected to the DHT sensor

#define DHTTYPE DHT22       // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);   // DHT object

float humidity, temperature, moisture_percentage;

int data_length; 
String sensorsdata,Pump_Status;

const unsigned long sendInterval = 5000;
unsigned long previousTime = 0;


void setup() {
  Serial.begin(9600);
  
  // Insitialise Software serial and Set the baud rate for the SoftwareSerial object
  sendLoRa.begin(115200);
  delay(1000);

  // Configure LoRa module at Startup
  sendLoRa.println("AT+BAND=" + lora_band);
  delay(1000);
  sendLoRa.println("AT+ADDRESS=" + lora_TX_address);
  delay(1000);
  sendLoRa.println("AT+NETWORKID=" + lora_networkid);
  delay(1000);

  pinMode(Relay1_Pump, OUTPUT);
  digitalWrite(Relay1_Pump, HIGH);
  Pump_Status = "Pump_OFF";
 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000); // Pause for 2 seconds

  dht.begin();
 
  // Clear the buffer.
  display.clearDisplay();
  
  //Draw bitmap on the screen
  display.drawBitmap(0, 0, image_data_096OLEDscreens, 128, 64, 1);
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 56);
  // Display static text
  display.println("make2explore.com");
  display.display();
  delay(2000);
  display.clearDisplay();
}

//====================== Function to Read the DHT Data ======================================//
void readSensor(){
  delay(250);
  humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temperature = dht.readTemperature();

  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );
  
  /*Serial.println("GREENHOUSE Data");
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(F(" °C"));
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(F(" % Rh"));
  Serial.print("Soil Moisture = ");
  Serial.print(moisture_percentage);
  Serial.println(F(" %"));*/

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
}


//====================== Function to Display Sensor Readings ==============================//
void displayData(){
    display.clearDisplay();
    display.drawRect(0, 0, 126, 62, WHITE);
    display.setCursor(6,5);
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(5,5);
    display.print("Temp = ");
    
    display.setTextSize(1);
    display.setCursor(55,5);
    display.print(temperature);
    
    display.setCursor(104,5);
    display.setTextSize(1);
    display.cp437(true);
    display.write(167);
    display.setTextSize(1);
    display.setCursor(112,5);
    display.print("C");
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(5,21);
    display.print("Humi = ");
    
    display.setCursor(55,21);
    display.setTextSize(1);
    display.print(humidity);
    
    display.setCursor(112,21);
    display.print("%");

    display.setCursor(5,37);
    display.setTextSize(1);
    display.print("Mois = ");

    display.setCursor(55,37);
    display.setTextSize(1);
    display.print(moisture_percentage);

    display.setCursor(112,37);
    display.print("%");  

    display.setCursor(5,53);
    display.setTextSize(1);
    display.print("Pump = ");

    display.setCursor(55,53);
    display.setTextSize(1);
    display.print(Pump_Status);  
     
    display.display();
    delay(250);  
}

//====================== Function to Process Readings and create string ==============================//
void processData(){
  if(moisture_percentage <= 30){
    digitalWrite(Relay1_Pump, LOW);
    Pump_Status = "Pump_ON";
  }
  else if(moisture_percentage >=60){
    digitalWrite(Relay1_Pump, HIGH);
    Pump_Status = "Pump_OFF";
  }
 sensorsdata = sensorsdata + String(temperature) + "%" + String(humidity) + "%" + String(moisture_percentage) + "%" + Pump_Status; 
 Serial.print("sensor values: ");
 Serial.println(sensorsdata);
 
// find the length of data
 data_length = sensorsdata.length();
 Serial.print("data length: "); 
 Serial.println(data_length);
 delay(250);
}

//====================== Function to send data Command  ==============================//

void send_data(String sensorvalue, int valuelength)
{
 String mymessage; 
 mymessage = mymessage + "AT+SEND=621" + "," + valuelength + "," + sensorvalue + "\r\n";
 sendLoRa.print(mymessage);
 Serial.print("Data Sent : ");
 Serial.println(mymessage); 
 delay(250);
}

//======================== Main Loop Function  ======================================//
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= sendInterval) {
    /* Event code */
    //Serial.println("Send The Sensor Readings");

    processData();
    send_data(sensorsdata , data_length);
   /* Update the timing for the next time around */
    previousTime = currentTime;
  }
  readSensor();
  displayData();
  sensorsdata = "";
}

/*========================================= info@make2explore.com ===============================================================*/
