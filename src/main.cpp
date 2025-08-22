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

void buzz();

// Define Digital Pins
#define CHARGE_PIN  2
#define RELAY_LED   7
#define BUZZER_PIN  5
#define CHARGE_BTN  9
#define SAFETY_PIN  11
#define MODE_PIN    12
#define RUN_PIN     13
#define DRAIN_PIN   3

void setup()  {
  pinMode(CHARGE_PIN, OUTPUT);
  pinMode(RELAY_LED, OUTPUT);
  pinMode(DRAIN_PIN, OUTPUT);
  digitalWrite(CHARGE_PIN, LOW);
  digitalWrite(DRAIN_PIN, LOW);
  digitalWrite(DRAIN_PIN, HIGH);
  isDeenergized = true;
  Serial.begin(9600); 
  delay(1000);
   static unsigned long buttonPressStartTime = 0; // Static to retain value between loop() calls
   static bool chargeModeEnabled = false; // Track if charge mode has been enabled
   static unsigned long lastButtonPressTime = 0;

   if (digitalRead(CHARGE_BTN) == HIGH) { // Button is pressed
     unsigned long currentTime = millis();
     if (!chargeModeEnabled) { // Charge mode not yet enabled, check for 5-second hold
       if (buttonPressStartTime == 0) { // First press detected
         buttonPressStartTime = currentTime; // Record start time
       }
       if (currentTime - buttonPressStartTime >= 3000) { // 5000ms = 5 seconds
         // Button has been held for 5 seconds!
         modeCharge = true;  // Enable charging
         modeDeenergize = false; // Disable deenergizing
         chargeModeEnabled = true; // Remember that charge mode has been enabled
         buttonPressStartTime = 0; // Reset timer
         Serial.println("Charge button held for 3 seconds.  Enabling charging.");
       }
     } else { // Charge mode is already enabled, single press to disable and de-energize
       if (currentTime - lastButtonPressTime > 500) { // Debounce
         modeCharge = false;       // Disable charging
         modeDeenergize = true;      // Enable de-energizing
         chargeModeEnabled = false;   // Disable charging mode
         buttonPressStartTime = 0;    // Reset timer for next 5-second hold
         lastButtonPressTime = currentTime;
         Serial.println("Charge button pressed. Disabling charging and enabling de-energizing.");
       }
     }
   } else {
     buttonPressStartTime = 0; // Reset timer if button is released
   }

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
         delay(2000); //Give user time to release button
      case 'q':
         modeCharge = false;
         modeDeenergize = true;
      default:
         delay(50);
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

  static unsigned long buttonPressStartTime = 0;
  static bool chargeModeEnabled = false;
  static unsigned long lastDebounceTime = 0;
  const long debounceDelay = 100;
  const long holdTime = 5000;

  int buttonState = digitalRead(CHARGE_BTN);

  Serial.print("buttonState: ");
  Serial.println(buttonState);
  Serial.print("chargeModeEnabled: ");
  Serial.println(chargeModeEnabled);
  Serial.print("millis() - lastDebounceTime: ");
  Serial.println(millis() - lastDebounceTime);
  Serial.print("buttonPressStartTime: ");
  Serial.println(buttonPressStartTime);

  if (buttonState == HIGH) {
    if (millis() - lastDebounceTime > debounceDelay) {
      Serial.println("Debounced button press");
      if (!chargeModeEnabled) { // Not in charge mode - check for long press
        Serial.println("Not in charge mode");
        if (buttonPressStartTime == 0) {
          Serial.println("buttonPressStartTime == 0, setting to millis()");
          buttonPressStartTime = millis();
        }
        if (millis() - buttonPressStartTime >= holdTime) {
          Serial.println("Long press detected, enabling charge mode");
          buzz();
          delay(2500);
          modeCharge = true;
          modeDeenergize = false;
          chargeModeEnabled = true;
          Serial.println("Charge mode enabled (3s hold)");
          buttonPressStartTime = 0; // Reset
        }
      } else { // Already in charge mode - single press to disable
        Serial.println("Already in charge mode, disabling");
        tone(BUZZER_PIN, 2000);
        delay(50);
        noTone(BUZZER_PIN);
        tone(BUZZER_PIN, 2000);
        delay(50);
        noTone(BUZZER_PIN);

        modeCharge = false;
        modeDeenergize = true;
        chargeModeEnabled = false;
        Serial.println("Charge mode disabled (single press)");
        buttonPressStartTime = 0; // Reset
      }
      lastDebounceTime = millis();
    }
  } else {
    Serial.println("Button released, resetting buttonPressStartTime");
    buttonPressStartTime = 0; // Reset if button is released
  }
}

void deenergize() {
  digitalWrite(DRAIN_PIN, LOW);
  digitalWrite(RELAY_LED, HIGH);
  delay(deenergize_duration_ms);
  digitalWrite(DRAIN_PIN, HIGH);
  digitalWrite(RELAY_LED, LOW);
  isDeenergized = true;
  delay(500); //Cool down relay
  Serial.println("De-energized");
}

void charge() {
  digitalWrite(DRAIN_PIN, HIGH);
  delay(50);
  digitalWrite(CHARGE_PIN, HIGH);
  delay(charge_duration_ms);
  digitalWrite(CHARGE_PIN, LOW);
  Serial.println("Charged");
  isCharging = false;
}

void buzz() {
  tone(BUZZER_PIN, 2000);
  delay(300);
  noTone(BUZZER_PIN);
}
