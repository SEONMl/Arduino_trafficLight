#include <Wire.h>
#include "pitches.h"
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

const byte trafficLight[3] = {3, 4, 5};
int lightTime[3] = {5, 2, 5};
unsigned long timeVal = 0;
int indexVal = 0;
  

int piezo = 12;             // 12번에 피에조 스피커 연결
int shock_sensor = A0;       // 2번에 충격 센서 연결

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);   // I2C LCD 객체 선언

DHT dht(8, DHT11);          // 8번에 온습도 센서 연결



void setup()
{
  Serial.begin(9600);
  lcd.init();
  dht.begin();
  
  for (int i=0; i<3; i++){
    pinMode(trafficLight[i], OUTPUT);
  }

  pinMode(piezo, OUTPUT);   // 스피커
  pinMode(shock_sensor, INPUT);   // 충격 센서
  
  lcd.backlight();                  // backlight를 On 시킵니다
  lcd.display();


}



void loop()
{
   traffic_light_func();
   analog_shock_func();
}


void traffic_light_func(){

  if (millis()-timeVal >= lightTime[indexVal]*1000){
    digitalWrite(trafficLight[indexVal], LOW);
    indexVal++;
    
    if (indexVal==3) indexVal=0;
    digitalWrite(trafficLight[indexVal], HIGH);
    timeVal = millis();
  }
}



void dht_print_func()
{ 
  float h = dht.readHumidity();     // 습도 정보
  float t = dht.readTemperature();  // 온도 정보

  lcd.clear();
  
  lcd.print("TEMP:     ");          // LCD에 온도 정보 출력
  lcd.print(t);
  Serial.print("온도: ");
  Serial.println(t); 
  
  lcd.setCursor(0, 1);
  lcd.print("HUMIDITY: ");          // LCD에 습도 정보 출력
  lcd.print(h);
  Serial.print("습도: ");
  Serial.println(h);
  
  
}


void analog_shock_func()
{ //충격센서 아날로그 값으로 읽음
  int val = analogRead(shock_sensor);
  
  Serial.print("충격 감지 정도 : ");
  Serial.println(val);
  
  if (val > 600){
    tone(piezo, 262, 500);
    lcd.clear();
    
    lcd.print("Shock was detected");
    Serial.println("충격 감지됨");
    delay(500);
   
    dht_print_func();
  }
  else{
    noTone(piezo);
    dht_print_func();
    delay(500);
  }
 
}
