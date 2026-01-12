/************************
Motor Speed Control with LED Display
- Potentiometer controls DC motor speed
- 74HC595 displays speed level on LEDs
************************/

// Motor control pins (L293D)
#define ENABLE 5
#define DIRA 3
#define DIRB 4

// 74HC595 shift register pins
#define LATCH_PIN 11  // ST_CP [RCK]
#define CLOCK_PIN 9   // SH_CP [SCK]
#define DATA_PIN 12   // DS [S1]

// Potentiometer pin
#define POT_PIN A0

// Speed thresholds (in RPM)
const int BASE_SPEED = 10;
const int SPEED_INCREMENT = 10;
const int MAX_SPEED = 100;  // 30 + (7 * 10) = 100 RPM for all 8 LEDs

byte leds = 0;
int currentSpeed = BASE_SPEED;

void setup() {
  // Motor pins
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  
  // Shift register pins
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  
  // Set motor direction (one way)
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, LOW);

  Serial.begin(9600);
  Serial.println("Motor Speed Control Started");
}

void updateShiftRegister() {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, leds);
  digitalWrite(LATCH_PIN, HIGH);
}

void updateLEDs(int speed) {
  // Calculate how many LEDs should be on based on speed
  // Base speed (30 RPM) = 0 LEDs
  // 40 RPM = 1 LED (Q7)
  // 50 RPM = 2 LEDs (Q7, Q6)
  // etc.
  
  int numLEDs = 0;
  if (speed >= 20) {
    numLEDs = (speed - 10) / SPEED_INCREMENT;
    if (numLEDs > 8) numLEDs = 8;  // Cap at 8 LEDs
  }
  
  // Build LED pattern starting from Q7 (MSB)
  leds = 0;
  for (int i = 0; i < numLEDs; i++) {
    bitSet(leds, i);  // Start from Q7 and work down
  }
  
  updateShiftRegister();
}

int calculateMotorPWM(int speed) {
  // Map speed (30-100 RPM) to PWM value (0-255)
  // At 30 RPM (base): minimal PWM
  // At 100 RPM (max): full PWM
  
  if (speed <= BASE_SPEED) {
    return 50;  // Minimum PWM to get motor moving
  } else if (speed >= MAX_SPEED) {
    return 255;  // Maximum PWM
  } else {
    // Linear mapping: 30 RPM = 50 PWM, 100 RPM = 255 PWM
    return map(speed, BASE_SPEED, MAX_SPEED, 50, 255);
  }
}

void loop() {
  // Read potentiometer value (0-1023)
  int potValue = analogRead(POT_PIN);
  
  // Map potentiometer to speed range (30-100 RPM)
  currentSpeed = map(potValue, 0, 1023, BASE_SPEED, MAX_SPEED);
  
  // Update LED display
  updateLEDs(currentSpeed);
  
  // Calculate and set motor speed
  int pwmValue = calculateMotorPWM(currentSpeed);
  analogWrite(ENABLE, pwmValue);
  
  // Debug output
  Serial.print("Pot: ");
  Serial.print(potValue);
  Serial.print(" | Speed: ");
  Serial.print(currentSpeed);
  Serial.print(" RPM | PWM: ");
  Serial.print(pwmValue);
  Serial.print(" | LEDs: 0b");
  Serial.println(leds, BIN);
  
  delay(50);  // Small delay for stability
}