// HC-SR04 Ultrasonic Sensor - Asynchronous Version
// TRIG -> Digital Pin 12
// ECHO -> Digital Pin 11
// Buzzer -> Digital Pin 2

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 12;
const int echoPin = 11;
const int buzzer = 2;

// Timing variables for non-blocking operations
unsigned long previousSensorMillis = 0;
unsigned long previousDisplayMillis = 0;

const long sensorInterval = 100;    // Read sensor every 100ms
const long displayInterval = 500;   // Update display every 500ms

int currentDistance = 0;
String currentDisplayState = "";

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PROXIMITY");
  lcd.setCursor(0, 1);
  lcd.print("DETECTOR");
  
  delay(2000); // Show splash screen for 2 seconds
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Read sensor asynchronously
  if (currentMillis - previousSensorMillis >= sensorInterval) {
    previousSensorMillis = currentMillis;
    currentDistance = readDistance();
    
    Serial.print("Distance: ");
    Serial.print(currentDistance);
    Serial.println(" cm");
  }
  
  // Update display asynchronously
  if (currentMillis - previousDisplayMillis >= displayInterval) {
    previousDisplayMillis = currentMillis;
    updateDisplay(currentDistance);
  }
  
  // Handle buzzer asynchronously with beeping pattern
  handleBuzzer(currentMillis, currentDistance);
}

int readDistance() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  
  if (duration == 0) {
    return 9999; // Return large value if no echo received
  }
  
  return duration * 0.034 / 2;
}

void handleBuzzer(unsigned long currentMillis, int distance) {
  if (distance <= 10) {
    // Continuous sound for active buzzer
    digitalWrite(buzzer, HIGH);
  } else {
    // Turn off buzzer when not too close
    digitalWrite(buzzer, LOW);
  }
}

void updateDisplay(int distance) {
  String newState = "";
  String line1 = "Distance Reading";
  String line2 = "";
  
  if (distance <= 15) {
    newState = "CLOSE";
    line2 = "TOO CLOSE!";
  } else if (distance <= 50) {
    newState = "MEDIUM";
    line2 = "That's alright";
  } else if (distance < 200) {
    newState = "FAR";
    line2 = "Kinda Far";
  } else {
    newState = "NOOBJ";
    line2 = "No object";
  }
  
  // Only clear and update if state changed (reduces flicker)
  if (newState != currentDisplayState) {
    currentDisplayState = newState;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
  }
}
