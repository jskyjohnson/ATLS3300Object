#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-2500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

unsigned int pingSpeed = 50;
unsigned long pingTimer;



bool rightStepping;
bool rightDirection;
int rightCurrent;
const int RightstepPin = 5; 
const int RightdirPin = 4; 

int leftCurrent;
bool leftDirection;
bool leftStepping;
const int LeftstepPin = 7; 
const int LeftdirPin = 6; 


#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

float ti;

float tx;
float ty;

int leftVal;
int rightVal;

float a = 1;
float b = 0.5;

float c = 1.3;
float d = 0.5;

unsigned int changeSpeed = 30000;
unsigned long changeTimer;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  ti = 0;

  rightStepping = true;
  leftStepping = true;
  leftVal = 70;
  rightVal = 40;

  rightCurrent = 0;
  leftCurrent = 0;

  //pingTimer = millis();

  a = (float) random(3000, 240000) / 100000.0;
  b = (float) random(3000, 240000) / 100000.0;
  c = (float) random(25000, 10000) / 100000.0;
  d = (float)random(25000, 10000) / 100000.0;
  Serial.print(a);
  Serial.print("\t");
  Serial.print(b);
  Serial.print("\t");
  Serial.print(c);
  Serial.print("\t");
  Serial.print(d);
  Serial.println();

  pinMode(RightstepPin,OUTPUT); 
  pinMode(RightdirPin,OUTPUT);

  pinMode(LeftstepPin,OUTPUT); 
  pinMode(LeftdirPin,OUTPUT);

  pinMode(13, OUTPUT);
}

void loop() {
  
  ti += 2*PI / 1000;

  tx = 3.5*sin(a*PI*ti) +4.5; //+ sin(c*PI*ti) + 5;
  ty = 8*cos(b*PI*ti) +12; //+ cos(d*PI*ti) + 15;

  if(rightStepping){
    if(rightVal > rightCurrent){
      digitalWrite(RightdirPin,HIGH);
      
      digitalWrite(RightstepPin,HIGH); 
      delayMicroseconds(2500); 
      digitalWrite(RightstepPin,LOW); 
      delayMicroseconds(2500);

      rightCurrent++;
    }else if(rightVal < rightCurrent){
      digitalWrite(RightdirPin,LOW);
      
      digitalWrite(RightstepPin,HIGH); 
      delayMicroseconds(2500); 
      digitalWrite(RightstepPin,LOW); 
      delayMicroseconds(2500);

      rightCurrent--;
    }else{
      rightStepping = false;
    }
  }else{
    rightVal = RightTurnTo(tx, ty);
    rightStepping = true;
  }

  if(leftStepping){
    if(leftVal > leftCurrent){
      digitalWrite(LeftdirPin,LOW);
      
      digitalWrite(LeftstepPin,HIGH); 
      delayMicroseconds(2500); 
      digitalWrite(LeftstepPin,LOW); 
      delayMicroseconds(2500);

      leftCurrent++;
    }else if(leftVal < leftCurrent){
      digitalWrite(LeftdirPin,HIGH);
      
      digitalWrite(LeftstepPin,HIGH); 
      delayMicroseconds(2500); 
      digitalWrite(LeftstepPin,LOW); 
      delayMicroseconds(2500);

      leftCurrent--;
    }else{
      leftStepping = false;
    }
  }else{
    leftVal = LeftTurnTo(tx, ty);
    leftStepping = true;
  }

  if(millis() > changeTimer){
    changeTimer += changeSpeed;
    a = (float) random(25000, 240000) / 100000.0;
    b = (float) random(25000, 240000) / 100000.0;
    c = (float) random(25000, 240000) / 100000.0;
    d = (float)random(25000, 240000) / 100000.0;
    Serial.print(a);
    Serial.print("\t");
    Serial.print(b);
    Serial.print("\t");
    Serial.print(c);
    Serial.print("\t");
    Serial.print(d);
    Serial.println();
    ti = 0;
  }

  if (millis() >= pingTimer) {   
    
    pingTimer += pingSpeed;      
    sonar.ping_timer(echoCheck);
    //Serial.println();
  }

  if(leftStepping){
    
  }
}

float LeftTurnTo(float x, float y){
  float dy = y - 33.0;
  float dx = x - 4.0;
  
  float D = sqrt(dy*dy + dx*dx);
  float alpha = 0 ;
  alpha = (atan(dx / dy) * RAD_TO_DEG)+90;
  
  float beta = acos(D/31.0)* RAD_TO_DEG;

  float angle = alpha + beta;
  //Serial.println(alpha);
  //Serial.print("\t Left Angle: ");
  //Serial.print(alpha);
  //float Servo_Angle = map(angle, 90.0, 180.0, 154.0, 50.0); // left vals 154.0, 50.0
  float stepAmount = map(angle, 90.0, 180.0, 0, 600);
  return angle;
}

float RightTurnTo(float x, float y){
  float dy = y - 33.0;
  float dx = x - 8.0;

  float D = sqrt(dy*dy + dx*dx);
  float alpha = 0 ;
  alpha = (atan(dx / dy) * RAD_TO_DEG) + 90;
  
  float beta = acos(D/31.0)* RAD_TO_DEG;

  float angle = alpha - beta;

  float stepAmount = map(angle, 0.0, 90.0, 0, 600);
  return stepAmount;
}

void echoCheck() {
  if (sonar.check_timer()) {
    Serial.print("Ping: ");
    Serial.print(sonar.ping_result / US_ROUNDTRIP_CM);
    Serial.println("cm");
  }
}
