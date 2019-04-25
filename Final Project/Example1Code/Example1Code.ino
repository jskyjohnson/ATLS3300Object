#include <Servo.h>

Servo Left;  // create servo object to control a servo
Servo Right;

const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int inputPin = A0;
int Val;

int Left_MIN_Val = 60;
int Left_MAX_Val = 160;

int Right_MIN_Val = 30;
int Right_MAX_Val = 90;

//related to sweeping but without delay/for loops

boolean sweep;
int leftVal;
int rightVal;

unsigned long SweepDuration = 10000;
unsigned long currentTime;
unsigned long lastEpoch;

void setup() {
  Serial.begin(9600);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  Left.attach(11);
  Right.attach(10);

  leftVal = Left_MIN_Val;
  rightVal = Right_MAX_Val;
  unsigned long currentTime = 0;
  unsigned long lastEpoch = 0;
}

void loop() {

  //Left.write(Left_MAX_Val);
 // Right.write(Right_MIN_Val);
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(inputPin);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings;
  
  Val = map(average, 0, 1023, 0, 180);

  /*
  if(sweep){
    currentTime = millis() - lastEpoch;
    leftVal = (int) (Left_MIN_Val + ((float) (Left_MAX_Val - Left_MIN_Val)/SweepDuration) * currentTime);
    rightVal = (int) (Right_MIN_Val + ((float) (Right_MAX_Val - Right_MIN_Val)/SweepDuration) * currentTime);
    if(currentTime > SweepDuration){
      lastEpoch = millis();
      sweep = false;
    }
  }else{
    currentTime = millis() - lastEpoch;
    leftVal = (int) (Left_MAX_Val - ((float) (Left_MAX_Val - Left_MIN_Val)/SweepDuration) * currentTime);
    rightVal = (int) (Right_MAX_Val - ((float) (Right_MAX_Val - Right_MIN_Val)/SweepDuration) * currentTime);

    if(currentTime > SweepDuration){
      lastEpoch = millis();
      sweep = true;
    }
  }
  */
  leftVal = Val;
  Left.write(leftVal);
  Right.write(rightVal);
  
  Serial.print("Mils ");
  Serial.print(millis());
  Serial.print(" \t Current Time : ");
  Serial.print(currentTime);
  Serial.print(" \t lastEpoch: ");
  Serial.print(lastEpoch);
  Serial.print("\t Sweep ");
  Serial.print(sweep);
  Serial.print(" \t");
  Serial.print(leftVal);
  Serial.print(" ");
  Serial.print(rightVal);
  Serial.print(" ");
  Serial.println(Val);
  delay(10);
}
