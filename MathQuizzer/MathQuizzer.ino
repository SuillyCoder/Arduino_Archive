/* @file MathQuizSystem.ino
|| @version 1.0
|| @description
|| | Math Quiz System with LCD display and keypad input
|| #
*/

//LIBRARY DECLARATIONS
#include "Keypad.h"
#include <LiquidCrystal_I2C.h>

//LCD SIZE INITIALIZATION
LiquidCrystal_I2C lcd(0x27, 16, 2);

//PIN VARIABLE DECLARATIONS
const int buzzerLed = 11;
const int ledRight = 12;
const int ledWrong = 13;

//ARITHMETIC VARIABLE DECLARATIONS
int numberList[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int result = 0;
int num1 = 0;
int num2 = 0;
char operation = '+';
String userAnswer = "";
bool quizActive = false;

//NUMERIC KEYPAD VARIABLE DECLARATIONS
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Initialize pins
  pinMode(ledRight, OUTPUT);
  pinMode(ledWrong, OUTPUT);
  pinMode(buzzerLed, OUTPUT);
  
  // Seed random number generator
  randomSeed(analogRead(0));
  
  // Display welcome screen
  displayWelcome();
}

void displayWelcome() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MATH QUIZ TIME!");
  lcd.setCursor(0, 1);
  lcd.print("Press # to Start");
  quizActive = false;
  userAnswer = "";
}

void startNewQuestion() {
  // Randomly select two numbers from the array
  num1 = numberList[random(0, 9)];
  num2 = numberList[random(0, 9)];
  
  // Randomly select operation (+ or *)
  if (random(0, 2) == 0) {
    operation = '+';
    result = num1 + num2;
  } else {
    operation = '*';
    result = num1 * num2;
  }
  
  // Display the question
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("What is ");
  lcd.print(num1);
  lcd.print(operation);
  lcd.print(num2);
  lcd.print("?");
  lcd.setCursor(0, 1);
  lcd.print("Ans: ");
  
  userAnswer = "";
  quizActive = true;
  
  Serial.print("Question: ");
  Serial.print(num1);
  Serial.print(operation);
  Serial.print(num2);
  Serial.print(" = ");
  Serial.println(result);
}

void flashLED(int ledPin, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
}

void checkAnswer() {
  int userAnswerInt = userAnswer.toInt();
  
  lcd.clear();
  
  if (userAnswerInt == result) {
    // Correct answer
    lcd.setCursor(0, 0);
    lcd.print("CORRECT!");
    lcd.setCursor(0, 1);
    lcd.print("Well Done!");
    
    flashLED(ledRight, 3);
    
    delay(2000);
    
    // Start a new question
    startNewQuestion();
  } else {
    // Wrong answer
    lcd.setCursor(0, 0);
    lcd.print("YOU FAILED!");
    lcd.setCursor(0, 1);
    lcd.print("Ans was: ");
    lcd.print(result);
    
    // Flash wrong LED and sound buzzer
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledWrong, HIGH);
      digitalWrite(buzzerLed, HIGH);
      delay(200);
      digitalWrite(ledWrong, LOW);
      digitalWrite(buzzerLed, LOW);
      delay(200);
    }
    
    delay(2000);
    
    // Return to welcome screen
    displayWelcome();
  }
}

void loop() {
  char customKey = customKeypad.getKey();
  
  if (customKey) {
    Serial.println(customKey);
    
    // If not in quiz, wait for # to start
    if (!quizActive && customKey == '#') {
      startNewQuestion();
    }
    // If in quiz
    else if (quizActive) {
      // If * is pressed, check the answer
      if (customKey == '*') {
        if (userAnswer.length() > 0) {
          checkAnswer();
        }
      }
      // If # is pressed, clear the answer
      else if (customKey == '#') {
        userAnswer = "";
        lcd.setCursor(5, 1);
        lcd.print("          "); // Clear answer area
        lcd.setCursor(5, 1);
      }
      // If a number is pressed, add to answer
      else if (customKey >= '0' && customKey <= '9') {
        if (userAnswer.length() < 5) { // Limit answer length
          userAnswer += customKey;
          lcd.setCursor(5, 1);
          lcd.print(userAnswer);
        }
      }
    }
  }
}