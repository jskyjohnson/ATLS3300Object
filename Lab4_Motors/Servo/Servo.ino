//Sky Johnson
//Lab 4, Servo thing. Literally copied almost entirely from the examplew
#include <Servo.h> 
 
Servo servo;
 
int potpin = 0; 
int val;     
 
void setup() 
{ 
  servo.attach(9); 
} 
 
void loop() 
{ 
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  servo.write(val);                  // sets the servo position according to the scaled value 
  delay(15);                           // waits for the servo to get there 
} 
