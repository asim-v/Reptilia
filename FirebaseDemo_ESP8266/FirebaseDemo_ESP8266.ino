//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.
#include "DHT.h"
#define DHTPIN 14 // Analog Pin sensor is connected to
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);


#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0xA7, 16, 2);

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "reptilllia.firebaseio.com"
#define FIREBASE_AUTH "9mCnrHQgGV5Ci4mKrjzdDykybi3sdt4e39am3XB8"
#define WIFI_SSID "TP-Link_0E69"
#define WIFI_PASSWORD "95242552"

void setup() {
  pinMode(15,OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.print("Iniciando...");  
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;

void loop() {

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print((char)223);

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity, 1);
  lcd.print("%");
  
  Firebase.setFloat("humidity", humidity);
  Firebase.setFloat("temperature", temperature);
  // set value
  Firebase.setFloat("number", 42.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100);
  
  // update value
  Firebase.setFloat("number", 43.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100);

  // get value 
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  if (Firebase.getBool("Light")){
    digitalWrite(15,HIGH);
  }else{
    digitalWrite(15,LOW);
  }
  delay(100);

  // remove value
  Firebase.remove("number");
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

//  // append a new value to /logs
//  String name = Firebase.pushInt("logs", n++);
//  // handle error
//  if (Firebase.failed()) {
//      Serial.print("pushing /logs failed:");
//      Serial.println(Firebase.error());  
//      return;
//  }
//  Serial.print("pushed: /logs/");

//  Serial.println(name);
  delay(1000);
}
