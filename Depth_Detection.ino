#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

// Pin definitions
int adc_id = 0;
int buzzerPin = 1;    // Change to your buzzer pin
int blueLED = 7;      // Change to your blue LED pin
int redLED = 8;      // Change to your red LED pin

// Water level thresholds (adjust these based on your sensor)
int NO_WATER = 10;       // Below this = no water
int SHALLOW = 40;       // Below this = shallow
int SUBMERGED = 300;     // Below this = good level
// Above SUBMERGED = too deep

int HistoryValue = 0;
unsigned long previousMillis = 0;
bool redLEDState = false;

void setup()
{
  lcd.init();
  lcd.init();
  lcd.backlight();
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  
  // Turn everything off initially
  digitalWrite(buzzerPin, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(redLED, LOW);
}

void loop()
{
  int value = analogRead(adc_id);
  unsigned long currentMillis = millis();
  
  // Update LCD and other actuators only when value changes significantly
  if(((HistoryValue>=value) && ((HistoryValue - value) > 10)) || 
     ((HistoryValue<value) && ((value - HistoryValue) > 10)))
  {
    HistoryValue = value;
    
    // Update LCD
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
  
  // Handle red LED flashing (asynchronous - every 200ms)
  if(HistoryValue >= SUBMERGED) {
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
  
  delay(100);
}