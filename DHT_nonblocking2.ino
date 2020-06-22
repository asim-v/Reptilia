//////////////////////////
#include "RTClib.h"
#define moisturePIN A3
#define mosPIN 5
#define greenLED 6
#define blueLED 7
#define redLED 8
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
int sensorValue;
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0xA7, 16, 2);
#include  <TimerOne.h>
volatile int i = 0;
volatile boolean zero_cross = 0;
int AC_pin = 3;
int dim = 0;
int inc = 1;
int freqStep = 65;

///////////////////////


#include <dht_nonblocking.h>
//#define DHT_SENSOR_TYPE DHT_TYPE_21
#define DHT_SENSOR_TYPE DHT_TYPE_22

static const int DHT_SENSOR_PIN = 9;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

void setup( )
{
  lcd.begin();
  lcd.backlight();
  lcd.print("Iniciando...");
  Serial.begin( 115200 );
  pinMode(AC_pin, OUTPUT);                          // Set the Triac pin as output
  attachInterrupt(0, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);
  Wire.begin();
  rtc.begin();

  pinMode(mosPIN, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  digitalWrite(greenLED, HIGH);
  digitalWrite(blueLED, HIGH);
  digitalWrite(redLED, HIGH);
  delay(1000);
  digitalWrite(greenLED, HIGH);
  digitalWrite(blueLED, LOW);
  digitalWrite(redLED, LOW);
  delay(500);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, HIGH);
  digitalWrite(redLED, LOW);
  delay(500);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(redLED, HIGH);
  delay(500);
  digitalWrite(greenLED, HIGH);
  digitalWrite(blueLED, HIGH);
  digitalWrite(redLED, HIGH);
  delay(300);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(redLED, LOW);
  delay(100);
  digitalWrite(greenLED, HIGH);
  digitalWrite(blueLED, HIGH);
  digitalWrite(redLED, HIGH);
  delay(1000);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(redLED, LOW);
  delay(2000);

  digitalWrite(greenLED, 1);
  digitalWrite(blueLED, 0);
  digitalWrite(redLED, 0);

}
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if ( millis( ) - measurement_timestamp > 4000ul )
  {
    if ( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return ( true );
    }
  }

  return ( false );
}

void zero_cross_detect() {
  zero_cross = true;               // set the boolean to true to tell our dimming function that a zero cross has occured
  i = 0;
  digitalWrite(AC_pin, LOW);       // turn off TRIAC (and AC)
}

// Turn on the TRIAC at the appropriate time
void dim_check() {
  if (zero_cross == true) {
    if (i >= dim) {
      digitalWrite(AC_pin, HIGH); // turn on light
      i = 0; // reset time step counter
      zero_cross = false; //reset zero cross detection
    }
    else {
      i++; // increment time step counter
    }
  }
}

float f(int x) {
  int result;
  result = 123-(((x * 122) / 100) + 1);
  return result;
}
int f2(int x) {
  int result;
  result = 100-((x * 100) / 122);
  return result;
}
/*
   Main program loop.
*/
void loop( )
{
  if (Serial.available()) {
    int val = Serial.parseInt();
    if (val > 0) {
      dim = val;
      Serial.println(dim);
    }
  }
  DateTime now = rtc.now();
  float temperature;
  float humidity;
  sensorValue = analogRead(moisturePIN);

  if ( measure_environment( &temperature, &humidity ) == true )
  {
    Serial.print(now.day());
    Serial.print('/');
    Serial.print(now.month());
    Serial.print('/');
    Serial.print(now.year());
    Serial.print(" ");
    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.print(now.second());
    Serial.print(" -- ");
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" );
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temperature, 1);
    lcd.print((char)223);
    lcd.print("");
    lcd.print("");
    lcd.print("S");
    lcd.print(moisturePIN);
    lcd.print("L:");
    lcd.print(f2(dim));
    lcd.print("%");


    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print(humidity, 1);
    lcd.print("% Hr:");
    lcd.print(now.hour());
    lcd.print(":");
    lcd.print(now.minute());

    if (humidity < 75) {
      digitalWrite(blueLED, 1);
      digitalWrite(mosPIN, 1);
    }
    else {
      digitalWrite(blueLED, 0);
      digitalWrite(mosPIN, 0);
    }

  }
  if (now.hour() == 0) {
    dim = f(0);
  } else if (now.hour() == 1) {
    dim = f(0);
  } else if (now.hour() == 2) {
    dim = f(0);
  } else if (now.hour() == 3) {
    dim = f(0);
  } else if (now.hour() == 4) {
    dim = f(5);
  } else if (now.hour() == 5) {
    dim = f(10);
  } else if (now.hour() == 6) {
    dim = f(15);
  } else if (now.hour() == 7) {
    dim = f(35);
  } else if (now.hour() == 8) {
    dim = f(55);
  } else if (now.hour() == 9) {
    dim = f(80);
  } else if (now.hour() == 10) {
    dim = f(85);
  } else if (now.hour() == 11) {
    dim = f(100);
  } else if (now.hour() == 12) {
    dim = f(100);
  } else if (now.hour() == 13) {
    dim = f(100);
  } else if (now.hour() == 14) {
    dim = f(100);
  } else if (now.hour() == 15) {
    dim = f(100);
  } else if (now.hour() == 16) {
    dim = f(90);
  } else if (now.hour() == 17) {
    dim = f(90);
  } else if (now.hour() == 18) {
    dim = f(75);
  } else if (now.hour() == 19) {
    dim = f(65);
  } else if (now.hour() == 20) {
    dim = f(40);
  } else if (now.hour() == 21) {
    dim = f(20);
  } else if (now.hour() == 22) {
    dim = f(10);
  } else if (now.hour() == 23) {
    dim = f(0);
  } else if (now.hour() == 24) {
    dim = f(0);
  }
  Serial.println(dim);
}
