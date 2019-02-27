#include <Stepper.h>

const int stepsPerRevolution = 513;

Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  pinMode(13, OUTPUT);
  myStepper.setSpeed(25);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(13, HIGH);
  myStepper.step(stepsPerRevolution);
  delay(500);

  digitalWrite(13, LOW);
  myStepper.step(-stepsPerRevolution);
  delay(500);
}
