/* Interesting Music box
 * BY: Sky Johnson
 * FOR: ATLS3300 Object
 * Lab 3, Analog IO
 * 
 * This entire project was inspired by this BEAUTIFUL piece of bitshift abuse
 * Called Bitshift Variations in C Minor
 * https://www.youtube.com/watch?v=cRFMip5NlgY
 * However, I wasn't able to get this exact song to work (I'm very sad still, maybe someday but I need to abuse the timers more)
 * 
 * I abused a few libraries here. Check out the SSD1306 Graphics library from Adafruit, and the Adafruit GFX library. These are for the LCD screen
 * 
 * I have an arduino Nano knock off purchased from Emakefun, for ATmega328P, note these had the CH340 interface...
 * https://www.amazon.com/gp/product/B07GPPK4DK/ref=ppx_yo_dt_b_asin_title_o00__o00_s01?ie=UTF8&psc=1
 * 2/19/2019
 * 
 * I also used an LCD from MakerFocus that uses the i2C standard. I was able to use the SSD1306 Library example code to get started!
 * https://www.amazon.com/gp/product/B079BN2J8V/ref=ppx_yo_dt_b_asin_title_o00__o00_s00?ie=UTF8&psc=1
 * 
 * Then, I used a variety of example Bitshift songs to create the various songs. 
 * Most can be found and read about on this reddit thread
 * https://www.reddit.com/r/programming/comments/mbakl/experimental_oneline_algorithmic_music/
 * 
 * Board features the LCD connected to A4 and A5 (Check IC2 for where SDA and SCL ports go)
 * 
 * 3 Potentiometers (Slide pot on 2, two Radial ones on A1 and A3
 * 
 * Digital Output speaker on 9!!! (This has to be the OC1A port in this case from the ATMega328, might be different for other boards) You can use other OCXX ports but youlll have to do some trickery.
 * Check here https://bigdanzblog.wordpress.com/2015/01/30/cant-get-i2c-to-work-on-an-arduino-nano-pinout-diagrams/ is where I found the information.
 */


//This is for the LCD
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// this is the Width and Height of Display which is 128 xy 32 
#define LOGO16_GLCD_HEIGHT 32
#define LOGO16_GLCD_WIDTH  128 

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

static int song = 0;
static int param1 = 0;
static int param2 = 0;

static String songtext = ""; // Author of the song
static String OriginalParam1Text = ""; //So we know the original Value of Param 1
static String OriginalParam2Text = ""; //So we know the original Value of Param 2

static unsigned char s1 = 0;
static unsigned char p1 = 0;
static unsigned char p2 = 0;

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  
  //  //At this point I don't really quite understand what's going on, besides i'm doing some funky stuff with the OC1B pin (pin 10) with the ATMega368
  Serial.begin(112500);
  checkParamsChanged();


  //Using the Arduino's TIMER on pin 9 to create sounds
  TCCR1B = (TCCR1A & 0xf8) | 1;
  analogWrite(9,1);
}

//Send value to Pin9
void putb(byte b){
  static long m;
  long t;
  while((t=micros())-m < 125);
  m=t;
  
  OCR1A=b;
}

void checkParamsChanged(){
  if(param1 != analogRead(1)||param2 != analogRead(3)||song!= analogRead(2)){
    param1 = analogRead(1);
    param2 = analogRead(3);
    song = analogRead(2);
    updateP();
  }
}

void updateP(){
  p1 = param1/(1024/128) +1;
  p2 = param2/(1024/128) +1;
  s1 = (song /(1024/16));
  textUpdate();
}

void loop() {
  
  static int debounce=0;
  for(long t=0;;t++){

    //Alllll the songs
    switch(s1){
      case 0:
        putb( t*((t>>9|t>>p2)&p1&t>>6));
        songtext = "#1 Visy";
        OriginalParam1Text = "(25)";
        OriginalParam2Text = "(13)";
        break;
      case 1:
        putb( 
          (t>>6|t|t>>(t>>16))*p1+((t>>11)&p2)
          );
        songtext = "#2 Anon 1";
        OriginalParam1Text = "(10)";
        OriginalParam2Text = "(7)";
        
        break;
      case 2:
        putb( 
          (t*(t>>8+t>>9)*p2)+sin(t*p1)
        );
        songtext = "#3 Viznut Ex.";
        OriginalParam1Text = "(1)";
        OriginalParam2Text = "(100)";
        break;
      case 3:
        putb( 
          t * p1 & (t >> 5) | t * p2 & (t >> 10)
        );
        songtext = "#4 Anon 2";
        OriginalParam1Text = "(10)";
        OriginalParam2Text = "(20)";
        break;
      case 4:
        putb( 
          t * 2 & (t >> 10) | t * 3 & (t >> 5) | t * 5 & (t >> 7)
        );
        songtext = "#5 ianbishop";
        OriginalParam1Text = "NA";
        OriginalParam2Text = "NA";
        break;
      case 5:
        putb( 
          t * ((t>>2||t>>3||t>>5||t>>7)&p2&t>>11) + (1- p1*sin(t))
        );
        songtext = "#6 ianbishop 2";
        OriginalParam1Text = "(1)";
        OriginalParam2Text = "(73)";
        break;
      case 6:
        putb( 
          t * (t/p1)/(p2*2) & (byte)(t-sin(t))
        );
        songtext = "#7 DrtyRobo";
        OriginalParam1Text = "(128)";
        OriginalParam2Text = "(128)";
        break;
      case 7:
        putb( 
          (((t>>3)&(t>>9))^(p1)*(byte)sin(t)*p2*(byte)cos(t))*t
        );
        songtext = "#8 Anon 3";
        OriginalParam1Text = "(1)";
        OriginalParam2Text = "(1)";
        break;
      case 8:
        putb( 
          t * ((t>>7|t>>3)&p1&t>>p2)
        );
        songtext = "#9 Mattho";
        OriginalParam1Text = "(27)";
        OriginalParam2Text = "(13)";
        break;
      case 9:
        putb( 
          (t*(4|7&t>>13)>>((~t>>11)&1)&p1) + p2*((t)*(t>>11&t>>13)*((~t>>9)&3)&127)
        );
        songtext = "#10 Stimmer";
        OriginalParam1Text = "(128)";
        OriginalParam2Text = "(1)";
        break;
      case 10:
        putb( 
          p1*t*5&(t>>7)|p2*t*3&(t*4>>10)
        );
        songtext = "#11 Miiro";
        OriginalParam1Text = "(1)";
        OriginalParam2Text = "(1)";
        break;
      case 11:
        putb( 
          p2*(t|(t>>9|t>>7))*t&(t>>p1|t>>9)
        );
        songtext = "#12 red";
        OriginalParam1Text = "(11)";
        OriginalParam2Text = "(1)";
        break;
      case 12:
        putb( 
          ((t*(t>>8|t>>9)&p1&t>>8))^p2*(t&t>>13|t>>6)
        );
        songtext = "#13 xpansive";
        OriginalParam1Text = "(46)";
        OriginalParam2Text = "(1)";
        break;
      case 13:
        putb( 
          ((t&4096/p2)?((t*(t^t%255)|(t>>4))>>1):(t>>3)|((t&8192/p1)?t<<2:t))
        );
        songtext = "#14 skurk";
        OriginalParam1Text = "(1)";
        OriginalParam2Text = "(1)";
        break;
      case 15:
        putb( 
          ((-t&4095)*(255&t*(t&t>>13))>>12)+(p1&t*(234&t>>8&t>>3)>>(3&t>>p2))
        );
        songtext = "#15 Tejeez";
        OriginalParam1Text = "(127)";
        OriginalParam2Text = "(14)";
        break;
      default:
        songtext = "No Song";
        OriginalParam1Text = "NA";
        OriginalParam2Text = "NA";
        break;
    }

    //How often to check for updates... I think this causes a bit of a jump
    if(debounce==0){
      debounce = 4000;
      checkParamsChanged();
    }else{ 
      debounce--;
    }
  }
}

void textUpdate(){
  display.clearDisplay();
  screenText("Author:", 3, 3, 1, false); //Slide Pot values
  screenText(songtext, 44, 3, 1, false);
  
  screenText("Param 1:", 3, 12, 1, false); //Right Pot Values
  screenText(String(p1), 60, 12, 1, false);
  screenText(OriginalParam1Text, 88, 12, 1, false);
  
  screenText("Param 2:", 3, 21, 1, false); //Left Pot Values
  screenText(String(p2), 60, 21, 1, false);
  screenText(OriginalParam2Text, 88, 21, 1, false);
  
  //display.drawLine(1, 37, 100, 37, WHITE);
 display.drawRect(0, 0, 127,32, WHITE);
 display.display();
}

void screenText(String text, int x, int y,int size, boolean d) {

  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(text);
  if(d){
    display.display();
  }
} 
