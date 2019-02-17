/*********************************************************************
Original sourse: https://github.com/adafruit/Adafruit_SSD1306
This is an example for our Monochrome OLEDs based on SSD1306 drivers
This code and video explains and shows you how to use SSD1306 OLED with 128x32 pixel Display.
You will see custom code for the display to display text and live values such as voltage, temperature, pressure etc.

This example is for a 128x32 size display using I2C 

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution

 * Watch the video for this code to learn it fully.
  * Watch the video here: https://youtu.be/RjyulqVsz2o
 * this code is offered "as is" without any warranty.
 *  Updated by Ahmad S. For Robojax.com video tutorial
 * on March 18, 2018 at 10:21 at Ajax, Ontario, Canada
 * Please view other Robojax codes and videos at http://robojax.com/learn/arduino
 * if you are sharing this code, you must keep this copyright note.
 * 
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
double count=0;

/*
 * PIN connection:
 * pin connection see: https://www.arduino.cc/en/Reference/Wire
 * for UNO: SDA to A4, SCL to A5
 * for Mega2560: SDA to 20, SCL to 21
 * for Leonardo: SDA to 2, SCL to 3
 * for Due: SDA to 20, SCL to 21
 * VCC to 5V
 * GND to GND :-)
 */


// this is the Width and Height of Display which is 128 xy 32 
#define LOGO16_GLCD_HEIGHT 32
#define LOGO16_GLCD_WIDTH  128 


#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


const int pot1pin = A1; 
const int pot2pin = A2; 
const int pot3pin = A3; 

const int speakerPin = 10;

int pot1val = 0;
int pot2val = 0;
int pot3val = 0;


void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();
  delay(2000);
   // Clear the buffer.
  display.clearDisplay(); 
}


void loop() {
  pot1val = analogRead(pot1pin);
  pot2val = analogRead(pot2pin);
  pot3val = analogRead(pot3pin);

  
  String vString =  String(count, 3);// using a float and the 
  display.clearDisplay();
  
  robojaxText("Pot 1:          ", 4, 3, 1, false); //pot1
  robojaxText(String(pot1val), 72, 3, 1, false);
  
  robojaxText("Pot 2:          ", 4, 11, 1, false); //pot2
  robojaxText(String(pot2val), 72, 11, 1, false);
  
  robojaxText("Pot 3:          ", 4, 21, 1, false); //pot3
  robojaxText(String(pot3val), 72, 21, 1, false);
  
  //display.drawLine(1, 37, 100, 37, WHITE);
 display.drawRect(1, 1, 126,31, WHITE);
 display.display();
 tone(10, pot1val, 500);
 delay(500); 
}


/*
 * robojaxText(String text, int x, int y,int size, boolean d)
 * text is the text string to be printed
 * x is the integer x position of text
 * y is the integer y position of text
 * z is the text size, 1, 2, 3 etc
 * d is either "true" or "false". Not sure, use true
 */
void robojaxText(String text, int x, int y,int size, boolean d) {

  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(text);
  if(d){
    display.display();
  }
  
  //delay(100);
}
