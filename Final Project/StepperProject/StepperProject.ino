// defines pins numbers

const int step1Pin = 5; 
const int dir1Pin = 4; 

const int stepPin = 7; 
const int dirPin = 6; 
 
void setup() {
  // Sets the two pins as Outputs

  pinMode(step1Pin,OUTPUT); 
  pinMode(dir1Pin,OUTPUT);
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
}
void loop() {
  digitalWrite(dirPin,LOW); // Enables the motor to move in a particular direction
  digitalWrite(dir1Pin,HIGH);
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 800; x++) {
    digitalWrite(step1Pin,HIGH); 
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(5000); 
    digitalWrite(step1Pin,LOW); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(5000); 
    
  }
  delay(1000); // One second delay
  
  digitalWrite(dirPin,HIGH); //Changes the rotations direction
  digitalWrite(dir1Pin,LOW);
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 800; x++) {
    digitalWrite(step1Pin,HIGH);
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(10000);
    digitalWrite(step1Pin,LOW);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(10000);
  }
  delay(1000);
}
