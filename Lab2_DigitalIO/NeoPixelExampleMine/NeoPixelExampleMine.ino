
#include <Adafruit_NeoPixel.h>

#define BUTTON1     10    
#define BUTTON2     11
#define BUTTON3     12

#define NEOPIXELPIN    9 

#define PIXEL_COUNT 5

//Strip Initialization retrieved from Example NEOPIXEL code library
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, NEOPIXELPIN, NEO_GRB + NEO_KHZ800);

bool button1State = HIGH; //Red
bool button2State = HIGH; //Green
bool button3State = HIGH; //Blue

int red = 0;
int green = 0;
int blue = 0;

void setup() {
  
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

void loop() {
  // Get current button state.
  bool newbutton1State = digitalRead(BUTTON1);
  bool newbutton2State = digitalRead(BUTTON2);
  bool newbutton3State = digitalRead(BUTTON3);

  //Detects if a button has been pressed.
  if (newbutton1State == LOW && button1State == HIGH) {
    //Increase value of associated color by set amount, or overflow
    if(red < 255){
      red += 50; 
    }else{
      red = 0;
    }
  }

  if (newbutton2State == LOW && button2State == HIGH) {
    //Increase value of associated color by set amount, or overflow
    if(green < 255){
      green += 50; 
    }else{
      green = 0;
    }
  }

  if (newbutton3State == LOW && button3State == HIGH) {
    //Increase value of associated color by set amount, or overflow
    if(blue < 255){
      blue += 50; 
    }else{
      blue = 0;
    }
  }


  Serial.print("RGB Values: ");
  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.print(blue);
  Serial.println();

  //Set the colors of the strip.
  SetColor(strip.Color(red, green, blue));
  delay(20); //short delay in the loop

  // Set the last button state to the old state.
  button1State = newbutton1State;
  button2State = newbutton2State;
  button3State = newbutton3State;

  
}

void SetColor(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}
