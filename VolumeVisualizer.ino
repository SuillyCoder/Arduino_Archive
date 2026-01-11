int  sensorPin  =  A0;     // select the input  pin for  the potentiometer 
int  ledBlue  =  2;   // select the pin for  the LED
int  ledRed  =  3;   // select the pin for  the LED
int  ledGreen  =  4;   // select the pin for  the LED
int  ledYellow  =  5;   // select the pin for  the LED
int  ledWhite  =  6;   // select the pin for  the LED
int  sensorValue =  0;  // variable to  store  the value  coming  from  the sensor

void setup()
{
pinMode(ledBlue, OUTPUT);
pinMode(ledRed, OUTPUT);
pinMode(ledYellow, OUTPUT);
pinMode(ledGreen, OUTPUT);
pinMode(ledWhite, OUTPUT);
Serial.begin(9600);
}
void loop(){
sensorValue =  analogRead(sensorPin);
Serial.println(sensorValue,  DEC);
  if (sensorValue <= 84){
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledWhite, LOW);
  }
  else if (sensorValue >= 85 && sensorValue <= 90){
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledWhite, LOW);
  }
  else if (sensorValue >= 91 && sensorValue <= 95){
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledWhite, LOW);
  }
  else if (sensorValue >= 96 && sensorValue <= 100){
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledWhite, LOW);
  }
  else if (sensorValue >= 100 && sensorValue <= 105){
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledWhite, LOW);
  }
  else{
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledWhite, HIGH);
  }
delay(100);
}

