
#include <dht.h>
#define outPin 8
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "RTClib.h"

dht DHT;
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const int LightSensorPin = A0;
const int MoistSensorPin = A1;
const int pumpPin = 9;
const int redLED = 6;
const int yellowLED = 7;

int LightValue;
int MoistValue;

int LightPercent;
int MoistPercent;

int period = 5000;
unsigned long time_now = 0;

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

void setup() {
  Serial.begin(9600);
    delay(3000); // wait for console opening
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  u8g2.begin();

  int readData = DHT.read22(outPin);
  pinMode(redLED, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(yellowLED, OUTPUT);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
}

void loop() {

  DateTime now = rtc.now();

  float temp = DHT.temperature;  // Read temperature
  float humid = DHT.humidity;   // Read humidity

  LightValue = analogRead(LightSensorPin);
  MoistValue = analogRead(MoistSensorPin);

  LightPercent = map(LightValue, 0, 1023, 0, 100);
  MoistPercent = map(MoistValue, 230, 730, 100, 0);
    
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC); 
    Serial.print('/');  
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.println();  
    Serial.print("Light: ");
    Serial.print(LightValue);
    Serial.print("; Light Percent: ");
    Serial.print(LightPercent);
    Serial.print("%; Moist: ");
    Serial.print(MoistValue);
    Serial.print("; Moisture Percent: ");
    Serial.print(MoistPercent);
    Serial.print("%; Temperature: ");
    Serial.print(temp);
    Serial.print(" C; Humidty: ");
    Serial.print(humid);
    Serial.println(" %");

  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font

    u8g2.setCursor(0,10);
    u8g2.print(now.day(), DEC);
    u8g2.print('/');
    u8g2.print(now.month(), DEC); 
    u8g2.print('/');  
    u8g2.print(now.year(), DEC);
    u8g2.print(' ');
    u8g2.print(now.hour(), DEC);
    u8g2.print(':');
    u8g2.print(now.minute(), DEC);
  
  u8g2.setCursor(0,22);
  u8g2.print("Temperature: ");
  u8g2.print(temp);
  u8g2.print(" C");

  u8g2.setCursor(0,34);
  u8g2.print("Humidity: ");
  u8g2.print(humid);
  u8g2.print(" %");

  u8g2.setCursor(0,46);
  u8g2.print("Light: ");
  u8g2.print(LightPercent);
  u8g2.print(" %");

  u8g2.setCursor(0,58);
  u8g2.print("Soil Moisture: ");
  u8g2.print(MoistPercent);
  u8g2.print(" %");

  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);  

if (MoistValue > 500) {
  digitalWrite(redLED, HIGH);
  time_now = millis();
  digitalWrite(pumpPin, HIGH);
  while(millis() < time_now + period){
  }
  time_now = millis();
   digitalWrite(pumpPin, LOW);
 while(millis() < time_now + period){
  }
 

}
  else {
    digitalWrite(pumpPin, LOW);
    digitalWrite(redLED, LOW);
}

if (LightValue < 500) {
  digitalWrite(yellowLED, HIGH);
}
  else {
    digitalWrite(yellowLED, LOW);

    delay(2000);
  }
}
