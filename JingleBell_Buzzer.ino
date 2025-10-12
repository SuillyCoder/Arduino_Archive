/*****************************************/
// Constants for pin numbers
const int buttonSoundOn = 3;
const int buttonSoundOff = 5;
const int buzzer = 8;
const int led = 10;

// Variables to manage the LED blinking state
unsigned long previousMillis = 0;
const long blinkInterval = 500; // Blink every 500 milliseconds

// Variable to track the buzzer state
bool isBuzzerOn = false;

// Define a note for rest
#define REST 0

// Define the notes and their durations for "Jingle Bells"
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880

int melody[] = {
  // Jingle Bells, jingle bells, jingle all the way
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  
  // Oh, what fun it is to ride
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5,

  // in a one-horse open sleigh, hey!
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5,
  NOTE_G5, NOTE_G5, NOTE_F5, NOTE_D5, NOTE_C5
};

// Note durations (in milliseconds)
// Quarter note = 400ms
int durations[] = {
  // Jingle bells, jingle bells, jingle all the way
  400, 400, 800,
  400, 400, 800,
  400, 400, 400, 400,
  1600,
  
  // Oh, what fun it is to ride in a one-horse open sleigh, hey!
  400, 400, 400, 200,
  400, 400, 400, 200,
  200, 400, 400, 400,
  400, 1600,

  // Jingle bells, jingle bells, jingle all the way
  400, 400, 800,
  400, 400, 800,
  400, 400, 400, 400,
  1600,

  // Oh, what fun it is to ride in a one-horse open sleigh, hey!
  800,
  400, 400, 400, 200,
  400, 400, 400,
  400, 400, 400, 400, 1600
};

// Variables to track song progress
int songIndex = 0;
unsigned long noteStartTime = 0;

void setup() {
  // Set up pin modes
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(buttonSoundOn, INPUT_PULLUP);
  pinMode(buttonSoundOff, INPUT_PULLUP);
}

void loop() {
  // Read the current state of the buttons
  int buttonOnState = digitalRead(buttonSoundOn);
  int buttonOffState = digitalRead(buttonSoundOff);
  unsigned long currentMillis = millis();

  // Check if the "Sound On" button is pressed
  if (buttonOnState == LOW) {
    isBuzzerOn = true;
  }

  // Check if the "Sound Off" button is pressed
  if (buttonOffState == LOW) {
    if (isBuzzerOn) {
      noTone(buzzer);
      isBuzzerOn = false;
      songIndex = 0; // Reset the song
    }
  }

  // Play the song and blink the LED if the buzzer is on
  if (isBuzzerOn) {
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      digitalWrite(led, !digitalRead(led));
    }

    // Song playing logic
    if (currentMillis - noteStartTime >= durations[songIndex]) {
      // Stop the previous tone and move to the next note
      noTone(buzzer);
      noteStartTime = currentMillis;
      songIndex++;

      // Reset the song to loop
      if (songIndex >= sizeof(melody) / sizeof(melody[0])) {
        songIndex = 0;
      }

      // Play the next note (or a rest)
      if (melody[songIndex] != REST) {
        tone(buzzer, melody[songIndex]);
      }
    }

  } else {
    // If the buzzer is off, make sure the LED is also off
    digitalWrite(led, LOW);
  }
}
