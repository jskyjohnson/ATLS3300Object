/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led1 = 7;
int led2 = 4;
int led3 = 2;

int button1 = 10;
int button2 = 11;
int button3 = 12;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led1, OUTPUT);  
  pinMode(led2, OUTPUT); 
  pinMode(led3, OUTPUT);  

  pinMode(button1, INPUT);  
  pinMode(button2, INPUT); 
  pinMode(button3, INPUT);  
}

// the loop routine runs over and over again forever:
void loop() {

  int button1State = digitalRead(button1);
  int button2State = digitalRead(button2);
  int button3State = digitalRead(button3);

  if(button1State){
    digitalWrite(led1, HIGH);
    delay(1);  
  }else if(!button1State){
    digitalWrite(led1, LOW);
    delay(1);  
  }
  if(button2State){
    digitalWrite(led2, HIGH);
    delay(1);  
  }else if(!button2State){
    digitalWrite(led2, LOW);
    delay(1);  
  }
  if(button3State){
    digitalWrite(led3, HIGH);
    delay(1);  
  }else if(!button1State){
    digitalWrite(led3, LOW);
    delay(1);  
  }
  delay(1);  
  
}
