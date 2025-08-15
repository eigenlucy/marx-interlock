#include "Arduino.h"
#include <Servo.h>

void deenergize();
bool modeDeenergize = false;
bool isDeenergized = false;
unsigned long deenergize_duration_ms = 2000;

void charge();
bool modeCharge = false;
bool isCharging = false;
unsigned long charge_duration_ms = 1000;
unsigned long charge_start_time = 0;

// Define Digital Pins
#define CHARGE_PIN  2
#define BLEED_PIN   4
#define COMMHALL_PIN 7
#define FIRE_PIN    8
#define SAFETY_PIN  11
#define MODE_PIN    12
#define RUN_PIN     13
#define DRAIN_PIN   3

void setup()  {
  pinMode(CHARGE_PIN, OUTPUT);
  pinMode(DRAIN_PIN, OUTPUT);
  digitalWrite(CHARGE_PIN, LOW);
  digitalWrite(DRAIN_PIN, LOW);
  delay(1000);
  digitalWrite(DRAIN_PIN, HIGH);
  isDeenergized = true;
  Serial.begin(9600);
  Serial.println("Thunderstone Test");
}

void loop() {
  Serial.println("Press r to run, and q to quit");
   if (Serial.available() > 0)  {
     char command = Serial.read();

     switch (command) {
       case 'r':
         modeDeenergize = false;
         modeCharge = true;
         break;
      case 'q':
         modeCharge = false;
         modeDeenergize = true;
         break;
      default:
         modeDeenergize = true;
     }
   }
   if (modeDeenergize == true)  {
     deenergize();
   }
   else if (modeCharge == true)  {
     charge();
   }
   else {
     deenergize();
     Serial.println("error");
   }
  delay(100);
}

void deenergize() {
  digitalWrite(DRAIN_PIN, LOW);
  delay(deenergize_duration_ms);
  digitalWrite(DRAIN_PIN, HIGH);
  isDeenergized = true;
  Serial.println("De-energized");
}

void charge() {
  digitalWrite(DRAIN_PIN, HIGH);
  digitalWrite(CHARGE_PIN, HIGH);
  delay(charge_duration_ms);
  digitalWrite(CHARGE_PIN, LOW);
  Serial.println("Charged");
  isCharging = false;
}
