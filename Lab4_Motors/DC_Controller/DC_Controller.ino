// Sky Johnson
// Lab 4. Part 2, DC Controller

bool direction; //false is spinning one way, true is spinning the other way.

const int ledpin1 = 11;
const int ledpin2 = 10;

const int buttonpin = A0;

const int ENpin = 3;
const int logic1 = 4;
const int logic2 = 5;

bool change = 0;

void setup(){
  Serial.begin(9600);
  pinMode(ledpin1, OUTPUT);
  pinMode(ledpin2, OUTPUT);

  pinMode(buttonpin, INPUT);

  pinMode(ENpin, OUTPUT);
  pinMode(logic1, OUTPUT);
  pinMode(logic2, OUTPUT);

  digitalWrite(ENpin, HIGH);
  direction = false;
}

void loop(){

  if(change && digitalRead(buttonpin)){
    
  }else if(change == false && digitalRead(buttonpin) == true){
    change = true;
  }else if(change == true && digitalRead(buttonpin) == false){
    change = false;
  }
  
  if(change){
    if(direction){
      direction = false;
      digitalWrite(ledpin1, HIGH);
      digitalWrite(ledpin2, LOW);

      digitalWrite(logic1, HIGH);
      digitalWrite(logic2, LOW);
      change = false;
    }else{
      direction = true;
      digitalWrite(ledpin1, LOW);
      digitalWrite(ledpin2, HIGH);

      digitalWrite(logic1, LOW);
      digitalWrite(logic2, HIGH);
      change = false;
    }
  }
  
}
