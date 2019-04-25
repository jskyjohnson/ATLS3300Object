#include <Servo.h>

Servo left;
Servo right;

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  left.attach(11);
  right.attach(10);
  ti = 0;
  leftVal = 70;
  rightVal = 40;
  right.write(rightVal);
}

void loop() {
  // put your main code here, to run repeatedly:
  ti += 0.00006;

  tx = 4*sin(11*PI*ti) + 6;
  ty = 4*cos(11*PI*ti) + 13.25;
  
  leftVal = LeftTurnTo(tx, ty);
  
  //Serial.print("\t Left Val : ");
  //Serial.print(leftVal);

  rightVal = RightTurnTo(tx, ty);
  //Serial.print("\t Right Val : ");
  //Serial.print(rightVal);
  //Serial.println();

  left.write(leftVal);
  right.write(rightVal);

  //Serial.println();
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
  float Servo_Angle = map(angle, 90.0, 180.0, 154.0, 50.0); // left vals 154.0, 50.0
  return Servo_Angle;
}

float RightTurnTo(float x, float y){
  float dy = y - 33.0;
  float dx = x - 8.0;

  float D = sqrt(dy*dy + dx*dx);
  float alpha = 0 ;
  alpha = (atan(dx / dy) * RAD_TO_DEG) + 90;
  
  float beta = acos(D/31.0)* RAD_TO_DEG;

  float angle = alpha - beta;
  //Serial.print("\t Right Angle: ");
  //Serial.println(angle);
  
  float Servo_Angle = map(angle, 0.0, 90.0, 124.0, 21.0); //right vals 127.0, 23.0
  return Servo_Angle;
}
