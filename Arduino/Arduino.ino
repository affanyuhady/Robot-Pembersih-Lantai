#include <LCD_I2C.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
LCD_I2C lcd(0x27);

char namaHari[7][12] = {"Minggu", "Senin", "Selasa", " Rabu ", "Kamis", "Jum'at", "Sabtu"};

int relay = 12;
int sound = 11;
int buzzer = 10;
int batt = A0; 
int nilai;
int volt;

boolean suaro = true;
boolean bunyi = true;

unsigned long prevMillis = 0;
const long interval = 5000;

void setup () 
{
  Serial.begin(9600);
  pinMode(sound, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(batt, INPUT);

  digitalWrite(relay, HIGH);
  digitalWrite(buzzer, HIGH);

  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
  }

  lcd.begin(); 
  lcd.backlight();
  lcd.setCursor(0, 0);
           //0123456789012345 
  lcd.print("CELSI PUTRI  W S");
  lcd.setCursor(0, 1);
           //0123456789012345      
  lcd.print(" 17101152620098 ");
  delay(2000);
  lcd.setCursor(0, 0);
           //0123456789012345 
  lcd.print(" ALAT PEMBERSIH ");
  lcd.setCursor(0, 1);
           //0123456789012345      
  lcd.print("LANTAI  OTOMATIS");   
  delay(2000);
  lcd.clear();
}

void loop () {
  if (bunyi == true) {
    nilai = analogRead(batt);
    float volt = nilai / 145.0 ;
  
    Serial.print("VOLT BATTERAI : ");
    Serial.print(volt);
    Serial.println(" V");
    delay(500);

    if ((volt > 0.9) && (volt <=1.85)) {
      digitalWrite(buzzer,HIGH);
      delay(1500);
      digitalWrite(buzzer,LOW);
      delay(250);
      suaro = false;
    }
    else {
      digitalWrite(buzzer,HIGH);
      suaro == true;
    }
  }

  if (suaro = true) {
    int status_sensor = digitalRead(sound);
    Serial.println(status_sensor);
    delay(10);
  
    if (status_sensor == 0) {
      digitalWrite(relay, LOW);
      delay(30000);
      digitalWrite(relay, HIGH);
      delay(500);
      digitalWrite(relay, LOW);
      delay(500);
      digitalWrite(relay, HIGH);
    } 
  }
  
  unsigned long currentMillis = millis();
  if (currentMillis - prevMillis >= interval)
  {
    prevMillis = currentMillis;
 
    waktu();
  }
  
}

void print2digits(int number) {         
  if (number >= 0 && number < 10) {     
    lcd.print('0');
  }
  lcd.print(number);
}

void waktu () {
  DateTime now = rtc.now();

  lcd.setCursor(0, 0);
           //0123456789012345 
  print2digits(now.day());
  lcd.print("/");

  print2digits(now.month());
  
  lcd.print("/");
  lcd.print(now.year());
  lcd.setCursor(1, 1);
           //0123456789012345      
  print2digits(now.hour());
  lcd.print(':');

  print2digits(now.minute());
  
  lcd.print(':');
  print2digits(now.second());
  lcd.print(" ");
  lcd.setCursor(10, 0);
           //0123456789012345      
  lcd.print(" HARI ");
  lcd.setCursor(10, 1);  
  lcd.print(namaHari[now.dayOfTheWeek()]);
  delay(1000);
  
  if ((now.hour() == 8) && (now.minute() == 0)) {
    digitalWrite(relay, LOW);
    delay(30000);
    digitalWrite(relay, HIGH);
    delay(500);
    digitalWrite(relay, LOW);
    delay(500);
    digitalWrite(relay, HIGH);
  }
  else if ((now.hour() == 13) && (now.minute() == 0)) {
    digitalWrite(relay, LOW);
    delay(30000);
    digitalWrite(relay, HIGH);
    delay(500);
    digitalWrite(relay, LOW);
    delay(500);
    digitalWrite(relay, HIGH);
  }
  else if ((now.hour() == 18) && (now.minute() == 0)) {
    digitalWrite(relay, LOW);
    delay(30000);
    digitalWrite(relay, HIGH);
    delay(500);
    digitalWrite(relay, LOW);
    delay(500);
    digitalWrite(relay, HIGH);
  }
  /*else if ((now.hour() == 1) && (now.minute() == 32)) {
    digitalWrite(relay, LOW);
    delay(30000);
    digitalWrite(relay, HIGH);
    delay(500);
    digitalWrite(relay, LOW);
    delay(500);
    digitalWrite(relay, HIGH);
  }*/
}
