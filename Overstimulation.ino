#include <Servo.h>

//Pin declarations for roundabout LEDs
const int red1 = 9;
const int yellow1 = 10;
const int green1 = 3;
const int blue1 = 4;
const int red2 = 6;
const int yellow2 = 7;
const int green2 = 8;
const int blue2 = 12;

// Arduino pin numbers
const int SW_pin = 13; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output
const int SERVO_PIN = 2; // servo motor pin
const int BUZZER_PIN = 5; // passive buzzer pin (changed from 0 to avoid conflict)

// Servo variables
Servo myServo;
bool servoRunning = false;
int servoPos = 0;
bool lastButtonState = HIGH;
bool buttonPressed = false;

// Buzzer variables
bool buzzerRunning = false;
unsigned long previousBuzzerMillis = 0;
const long buzzerInterval = 100; // Oscillate every 100ms
int currentFreq = 800; // Starting frequency
bool freqToggle = false;

// Timing variables for asynchronous operation
unsigned long previousSerialMillis = 0;
unsigned long previousServoMillis = 0;
const long serialInterval = 500; // Print serial info every 500ms
const long servoInterval = 300; // Sharp swerve every 300ms

// Joystick threshold values
const int CENTER = 512;
const int THRESHOLD = 200; // Range around center/edges for detection

void ledActivate(int x, int y){
  // Turn off all LEDs first
  digitalWrite(red1, LOW);
  digitalWrite(red2, LOW);
  digitalWrite(green1, LOW);
  digitalWrite(green2, LOW);
  digitalWrite(yellow1, LOW);
  digitalWrite(yellow2, LOW);
  digitalWrite(blue1, LOW);
  digitalWrite(blue2, LOW);
  
  // Define ranges for detection
  bool isLeft = (x < THRESHOLD);
  bool isRight = (x > (1023 - THRESHOLD));
  bool isCenterX = (x >= (CENTER - THRESHOLD) && x <= (CENTER + THRESHOLD));
  
  bool isDown = (y < THRESHOLD);
  bool isUp = (y > (1023 - THRESHOLD));
  bool isCenterY = (y >= (CENTER - THRESHOLD) && y <= (CENTER + THRESHOLD));
  
  // Cardinal directions (center X or Y)
  if (isLeft && isCenterY){
    digitalWrite(green1, HIGH);
  }
  else if (isRight && isCenterY){
    digitalWrite(green2, HIGH);
  }
  else if (isCenterX && isDown){
    digitalWrite(red2, HIGH);
  }
  else if (isCenterX && isUp){
    digitalWrite(red1, HIGH);
  }
  // Diagonal directions
  else if (isLeft && isDown){
    digitalWrite(blue1, HIGH);
  }
  else if (isRight && isUp){
    digitalWrite(blue2, HIGH);
  }
  else if (isLeft && isUp){
    digitalWrite(yellow1, HIGH);
  }
  else if (isRight && isDown){
    digitalWrite(yellow2, HIGH);
  }
}

void updateServo(){
  if (servoRunning){
    unsigned long currentMillis = millis();
    if (currentMillis - previousServoMillis >= servoInterval){
      previousServoMillis = currentMillis;
      
      // Sharp swerve left to right
      if (servoPos == 0){
        servoPos = 180; // Jump to right
        myServo.write(servoPos);
      } else {
        servoPos = 0; // Jump to left
        myServo.write(servoPos);
      }
    }
  }
}

void updateBuzzer(){
  if (buzzerRunning){
    unsigned long currentMillis = millis();
    if (currentMillis - previousBuzzerMillis >= buzzerInterval){
      previousBuzzerMillis = currentMillis;
      
      // Sharp oscillating tone - alternate between two frequencies
      if (freqToggle){
        currentFreq = 1200; // High pitch
      } else {
        currentFreq = 600; // Low pitch
      }
      freqToggle = !freqToggle;
      
      tone(BUZZER_PIN, currentFreq);
    }
  } else {
    noTone(BUZZER_PIN); // Stop the buzzer
  }
}

void handleButtonPress(int swState){
  // Detect button press (active LOW with pullup)
  if (swState == LOW && lastButtonState == HIGH){
    // Button just pressed
    buttonPressed = true;
  }
  
  if (swState == HIGH && lastButtonState == LOW && buttonPressed){
    // Button just released - toggle servo and buzzer
    servoRunning = !servoRunning;
    buzzerRunning = !buzzerRunning;
    buttonPressed = false;
    
    Serial.print("Servo & Buzzer ");
    Serial.println(servoRunning ? "STARTED" : "STOPPED");
  }
  
  lastButtonState = swState;
}

void readInfo(int swState, int xVal, int yVal){
  Serial.print("Switch:  ");
  Serial.print(swState);
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(xVal);
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(yVal);
  Serial.print("Servo: ");
  Serial.println(servoRunning ? "Running" : "Stopped");
  Serial.print("Buzzer: ");
  Serial.println(buzzerRunning ? "Running" : "Stopped");
  Serial.print("\n\n");
}

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);

  //Output declaration of the LEDs
  pinMode(red1, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(green1, OUTPUT);
  pinMode(green2, OUTPUT);
  pinMode(yellow1, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(blue1, OUTPUT);
  pinMode(blue2, OUTPUT);
  
  // Initialize buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialize servo
  myServo.attach(SERVO_PIN);
  myServo.write(0); // Start at 0 degrees
}

void loop() {
  // Read joystick values
  int xVal = analogRead(X_pin);
  int yVal = analogRead(Y_pin);
  int swState = digitalRead(SW_pin);
  
  // Update LEDs immediately (no delay)
  ledActivate(xVal, yVal);
  
  // Handle button press for servo and buzzer toggle
  handleButtonPress(swState);
  
  // Update servo position if running
  updateServo();
  
  // Update buzzer tone if running
  updateBuzzer();
  
  // Print serial info asynchronously using millis()
  unsigned long currentMillis = millis();
  if (currentMillis - previousSerialMillis >= serialInterval) {
    previousSerialMillis = currentMillis;
    readInfo(swState, xVal, yVal);
  }
}