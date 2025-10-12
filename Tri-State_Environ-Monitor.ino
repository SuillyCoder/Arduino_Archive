#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleDHT.h>

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT Sensor Setup
#define DHTPIN 5
SimpleDHT11 dht11(DHTPIN);  // Use SimpleDHT22 if you have DHT22

// Pin definitions
int waterSensorPin = A0;
int lightSensorPin = A3;
int buzzerPin = 1;
int blueLED = 7;
int redLED = 8;
int sw1Pin = 11;  // Water Level Mode
int sw2Pin = 12;  // Light Sensitivity Mode
int sw3Pin = 13;  // Temperature Mode

// Water level thresholds
int NO_WATER = 10;
int SHALLOW = 40;
int SUBMERGED = 300;

// Light level thresholds (adjust based on your photoresistor)
int NO_LIGHT = 300;      // Below this = no light
int GOOD_LIGHT = 850;    // Below this = good light
// Above GOOD_LIGHT = too bright

// Temperature threshold
float HOT_TEMP = 30.0;   // Temperature threshold in Celsius

// State variables
int currentMode = 1;     // 1 = Water, 2 = Light, 3 = Temperature
int waterHistoryValue = 0;
int lightHistoryValue = 0;
float tempHistoryValue = 0;

unsigned long previousMillis = 0;
unsigned long previousTempReadMillis = 0;
bool redLEDState = false;

// Button debouncing
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
unsigned long debounceDelay = 200;
int lastButtonState1 = HIGH;
int lastButtonState2 = HIGH;
int lastButtonState3 = HIGH;

void setup()
{
  lcd.init();
  lcd.init();
  lcd.backlight();
  
  // Setup pins
  pinMode(buzzerPin, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(sw1Pin, INPUT_PULLUP);
  pinMode(sw2Pin, INPUT_PULLUP);
  pinMode(sw3Pin, INPUT_PULLUP);
  
  // Turn everything off initially
  digitalWrite(buzzerPin, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(redLED, LOW);
  
  // Display initial mode
  displayModeChange(1);
}

void loop()
{
  unsigned long currentMillis = millis();
  
  // Check button presses
  checkButtons(currentMillis);
  
  // Execute current mode
  switch(currentMode) {
    case 1:
      waterLevelMode(currentMillis);
      break;
    case 2:
      lightSensitivityMode(currentMillis);
      break;
    case 3:
      temperatureMode(currentMillis);
      break;
  }
  
  delay(100);
}

void checkButtons(unsigned long currentMillis) {
  int reading1 = digitalRead(sw1Pin);
  int reading2 = digitalRead(sw2Pin);
  int reading3 = digitalRead(sw3Pin);
  
  // Check Sw1 (Water Level Mode)
  if(reading1 == LOW && lastButtonState1 == HIGH && 
     (currentMillis - lastDebounceTime1) > debounceDelay) {
    lastDebounceTime1 = currentMillis;
    if(currentMode != 1) {
      currentMode = 1;
      resetOutputs();
      displayModeChange(1);
    }
  }
  lastButtonState1 = reading1;
  
  // Check Sw2 (Light Sensitivity Mode)
  if(reading2 == LOW && lastButtonState2 == HIGH && 
     (currentMillis - lastDebounceTime2) > debounceDelay) {
    lastDebounceTime2 = currentMillis;
    if(currentMode != 2) {
      currentMode = 2;
      resetOutputs();
      displayModeChange(2);
    }
  }
  lastButtonState2 = reading2;
  
  // Check Sw3 (Temperature Mode)
  if(reading3 == LOW && lastButtonState3 == HIGH && 
     (currentMillis - lastDebounceTime3) > debounceDelay) {
    lastDebounceTime3 = currentMillis;
    if(currentMode != 3) {
      currentMode = 3;
      resetOutputs();
      displayModeChange(3);
    }
  }
  lastButtonState3 = reading3;
}

void displayModeChange(int mode) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode Changed:");
  lcd.setCursor(0, 1);
  
  switch(mode) {
    case 1:
      lcd.print("Water Level");
      break;
    case 2:
      lcd.print("Light Sensor");
      break;
    case 3:
      lcd.print("Temperature");
      break;
  }
  delay(1000);
}

void resetOutputs() {
  digitalWrite(buzzerPin, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(redLED, LOW);
  redLEDState = false;
  
  // Reset history values to force LCD update on mode entry
  waterHistoryValue = -999;
  lightHistoryValue = -999;
  tempHistoryValue = -999;
  
  // Reset timing
  previousMillis = 0;
  previousTempReadMillis = 0;
}

void waterLevelMode(unsigned long currentMillis) {
  int value = analogRead(waterSensorPin);
  
  // Update LCD and actuators when value changes significantly
  if(((waterHistoryValue >= value) && ((waterHistoryValue - value) > 10)) || 
     ((waterHistoryValue < value) && ((value - waterHistoryValue) > 10)))
  {
    waterHistoryValue = value;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Water Level:");
    lcd.setCursor(0, 1);
    
    if(value < NO_WATER) {
      lcd.print("No Water");
      digitalWrite(blueLED, LOW);
      digitalWrite(buzzerPin, LOW);
    }
    else if(value < SHALLOW) {
      lcd.print("Shallow Water");
      digitalWrite(blueLED, HIGH);
      digitalWrite(buzzerPin, LOW);
    }
    else if(value < SUBMERGED) {
      lcd.print("Submerged");
      digitalWrite(blueLED, HIGH);
      digitalWrite(buzzerPin, LOW);
    }
    else {
      lcd.print("Too Deep!");
      digitalWrite(blueLED, LOW);
      digitalWrite(buzzerPin, HIGH);
    }
  }
  
  // Handle red LED flashing for "Too Deep"
  if(waterHistoryValue >= SUBMERGED) {
    if(currentMillis - previousMillis >= 200) {
      previousMillis = currentMillis;
      redLEDState = !redLEDState;
      digitalWrite(redLED, redLEDState);
    }
  }
  else {
    digitalWrite(redLED, LOW);
    redLEDState = false;
  }
}

void lightSensitivityMode(unsigned long currentMillis) {
  int value = analogRead(lightSensorPin);
  
  // Update when value changes significantly
  if(((lightHistoryValue >= value) && ((lightHistoryValue - value) > 20)) || 
     ((lightHistoryValue < value) && ((value - lightHistoryValue) > 20)))
  {
    lightHistoryValue = value;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Light Level:");
    lcd.setCursor(0, 1);
    
    if(value < NO_LIGHT) {
      lcd.print("No Light");
      digitalWrite(blueLED, LOW);
      digitalWrite(redLED, LOW);
      digitalWrite(buzzerPin, LOW);
    }
    else if(value < GOOD_LIGHT) {
      lcd.print("Light Detected");
      digitalWrite(blueLED, HIGH);
      digitalWrite(redLED, LOW);
      digitalWrite(buzzerPin, LOW);
    }
    else {
      lcd.print("Too Bright!");
      digitalWrite(blueLED, LOW);
      digitalWrite(buzzerPin, HIGH);
    }
  }
  
  // Handle red LED rapid flashing for "Too Bright"
  if(lightHistoryValue >= GOOD_LIGHT) {
    if(currentMillis - previousMillis >= 100) {  // Rapid flashing (100ms)
      previousMillis = currentMillis;
      redLEDState = !redLEDState;
      digitalWrite(redLED, redLEDState);
    }
  }
  else {
    digitalWrite(redLED, LOW);
    redLEDState = false;
  }
}

void temperatureMode(unsigned long currentMillis) {
  // Read temperature every 2 seconds (DHT sensors are slow)
  if(currentMillis - previousTempReadMillis >= 2000) {
    previousTempReadMillis = currentMillis;
    
    byte temperature = 0;
    byte humidity = 0;
    
    // Read from DHT11 sensor
    int err = dht11.read(&temperature, &humidity, NULL);
    
    if(err == SimpleDHTErrSuccess) {
      float temp = (float)temperature;
      
      // Always update LCD and LEDs in temperature mode
      tempHistoryValue = temp;
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temperature:");
      lcd.setCursor(0, 1);
      lcd.print(temp, 1);
      lcd.print(" C");
      
      if(temp >= HOT_TEMP) {
        // Too hot - red LED and buzzer
        digitalWrite(blueLED, LOW);
        digitalWrite(redLED, HIGH);
        digitalWrite(buzzerPin, HIGH);
      }
      else {
        // Normal temperature - blue LED
        digitalWrite(blueLED, HIGH);
        digitalWrite(redLED, LOW);
        digitalWrite(buzzerPin, LOW);
      }
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp Sensor");
      lcd.setCursor(0, 1);
      lcd.print("Error!");
    }
  }
}