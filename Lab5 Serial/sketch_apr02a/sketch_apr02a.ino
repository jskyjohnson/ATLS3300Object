
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(5, 6, NEO_GRB + NEO_KHZ800);
int Pot = A0;
int Mic = A1;

int sampleWindow = 200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pixels.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  int potVal = analogRead(Pot);
  double fpotVal = potVal / 204.6;

  int signalMax = 0;
  int signalMin = 1024;
  unsigned long start= millis(); 
 
  while (millis() - start < sampleWindow){
    int micVal = analogRead(Mic);
    if (micVal < 1024){
      if (micVal > signalMax){
         signalMax = micVal;
      } else if (micVal < signalMin){
       signalMin = micVal; 
      }
    }
  }
  double fmicVal = ((signalMax + signalMin) / 2 );

  String outValue = String(fmicVal) + "," + String(fpotVal);

  Serial.println(outValue);

  if(fmicVal < 400){
    pixels.setPixelColor(0, pixels.Color(0,0,50));
    pixels.setPixelColor(1, pixels.Color(0,0,0));
    pixels.setPixelColor(2, pixels.Color(0,0,0));
    pixels.setPixelColor(3, pixels.Color(0,0,0));
    pixels.setPixelColor(4, pixels.Color(0,0,0));
    pixels.show();
  }else if(fmicVal > 400 && fmicVal <= 550){
    pixels.setPixelColor(0, pixels.Color(0,0,50));
    pixels.setPixelColor(1, pixels.Color(0,50,0));
    pixels.setPixelColor(2, pixels.Color(0,0,0));
    pixels.setPixelColor(3, pixels.Color(0,0,0));
    pixels.setPixelColor(4, pixels.Color(0,0,0));
    pixels.show();
  }else if(fmicVal > 550 && fmicVal <= 650){
    pixels.setPixelColor(0, pixels.Color(0,0,50));
    pixels.setPixelColor(1, pixels.Color(0,50,0));
    pixels.setPixelColor(2, pixels.Color(0,50,0));
    pixels.setPixelColor(3, pixels.Color(0,0,0));
    pixels.setPixelColor(4, pixels.Color(0,0,0));
    pixels.show();
  }else if(fmicVal > 650 && fmicVal <= 800){
    pixels.setPixelColor(0, pixels.Color(0,0,50));
    pixels.setPixelColor(1, pixels.Color(0,50,0));
    pixels.setPixelColor(2, pixels.Color(0,50,0));
    pixels.setPixelColor(3, pixels.Color(0,50,0));
    pixels.setPixelColor(4, pixels.Color(0,0,0));
    pixels.show();
  }else if(fmicVal > 800){
    pixels.setPixelColor(0, pixels.Color(0,0,50));
    pixels.setPixelColor(1, pixels.Color(0,50,0));
    pixels.setPixelColor(2, pixels.Color(0,50,0));
    pixels.setPixelColor(3, pixels.Color(0,50,0));
    pixels.setPixelColor(4, pixels.Color(50,0,0));
    pixels.show();
  }
  delay(500);
}
