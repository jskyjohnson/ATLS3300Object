
//This is for the LCD
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

// this is the Width and Height of Display which is 128 xy 32 
#define LOGO16_GLCD_HEIGHT 32
#define LOGO16_GLCD_WIDTH  128 

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//This is for the Sound System
int song = 0;
int param1 = 0;
int param2 = 0;
unsigned char s1 = 0;
unsigned char p1 = 0;
unsigned char p2 = 0;

#define SAMPLE_MAX (65535.0)
#define SAMPLE_FREQUENCY (8000.0)
#define TIMER1_FREQUENCY 2000000
#define UPDATE_RATE 8000

const int pot1pin = A1; 
const int pot2pin = A2; 
const int pot3pin = A3; 

int pot1val = 0;
int pot2val = 0;
int pot3val = 0;

long t = 0;
/*
SIGNAL (TIMER1_COMPA_vect) 
{
 OCR1A += (TIMER1_FREQUENCY/UPDATE_RATE);
 
 t++;
 switch(s1)
 {
   default:
   case 0:
     OCR0A =0;   break;
   case 1:

     putb(((-t&4095)*(p1&t*(t&t>>13))>>12)+(p2&t*(p1+p2&t>>8&t>>3)>>(3&t>>14))); // by tejeez
     
     break;
   case 2:
     putb(t*(t>>p1&t>>8&p2&t>>p1+p2)); // by tejeez
     break;
   case 3:
     putb(t*((t>>p1|t>>p2)&p1+p2&t>>6));   // by visy
     break;
   case 4:
     OCR0A = (t*(t>>p1|t>>p2))>>(t>>p1+p2);   // by tejeez
     break;
   case 5:
     OCR0A = ((t*(t>>p1|t>>9)&p2&t>>8))^(t&t>>p1+p2|t>>6); // by xpansive
     break;
   case 6:
   
     OCR0A = ((t&p1)?((t*(t^t%p2)|(t>>4))>>1):(t>>3)|((t&param1)?t<<2:t)); // by skurk (raer's version)
     break;
   case 7:
     OCR0A = (t>>7|t|t>>6)*p1+p2*(t&t>>p1+p2|t>>6); // by viznut, xpansive, varjohukka
     break;
   case 8:
     OCR0A = t*p1&(t>>p2)|t*3&(t*p1+p2>>10); // by miiro
     break;
   case 9:
     OCR0A = (t|(t>>p1|t>>p2))*t&(t>>p1+p2|t>>9); // by red
     break;
   case 10:
     long v;
     OCR0A = v=(v>>1)+(v>>4)+t*(((t>>p1)|(t>>6))&(p2&(t>>p1+p2))); // by pyryp
     break;
   case 11: 
     OCR0A = (t>>6|t|t>>(t>>p1))*p2+((t>>p1+p2)&7);  //by viznut
     break;
   case 12:
     OCR0A = (t*(4|p1&t>>13)>>((~t>>11)&1)&p1) + ((t)*(t>>p1&t>>13)*((~t>>9)&3)&127); // by stimmer
     break;
   case 13:
     OCR0A = (t>>p2|t|t>>(t>>p1))*p2   ;
     break;
   case 14:
      OCR0A = (sin((float)t*p1)+1.0f)*(float)p2;
     break;
   case 15:
     // free to use
     break;
     // any more and we need another bit from PORTB
   //  OCR0A = OCR0A/4;
 }
}
*/
void setup()   {        
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

  /*
  TCCR1A=0x02;          // set the timer prescaler to 8 = 16/8 = 2MHz
  //TCCR1B=0x02;         // set the timer prescaler to 8 = 16/8 = 2MHz
  TIMSK1 |= (1<<OCIE1A);   // Enable output compare match interrupt on OCR1A
  TCCR0A=0B10000011;                                    //-8 bit audio PWM
  TCCR0B=0x01;          // Set to clock frequency, no prescaler
  OCR0B=127;            // set in the middle - do we need this ? probably not.
  DDRB|=1<6;          // Set digital pin 6 to output - channels 2 and 3
  */
  TCCR0A = 0B10000011;
  pinMode(6, OUTPUT);
  checkParamsChanged();
}

void putb(byte b)
{
 
 static long m;
 long k;
 while((k=micros())-m < 125);
 m=k;
 OCR0A=b;
}

void checkParamsChanged()
{
  if(param1 != analogRead(1)||param2 != analogRead(3)||song!= analogRead(2)){
    param1 = analogRead(1);
    param2 = analogRead(3);
    song = analogRead(2);
    updateP();
  }
}

void updateP()
{
  p1 = param1/(1024/255);
  p2 = param2/(1024/255);
  s1 = song /(1025/14);
  
  if(t%10000 == 0){
    textUpdate();
  }
}

byte g(long i,int x, y,o){         //function g returns waveheight, given time increment, note chord, volumn???, octave, 
 return((3&x&(i*( (3&i>>16?\"BY}6YB6%\":\"Qj}6jQ6%\")[y%8]+51)>>o))<<4);
}; 

void loop() {
  //putb( g(i,1,n=i>>14,12)+ g(i,s=i>>17,n^i>>13,10)+ g(i,s/3,n+((i>>11)%3),10)+ g(i,s/5,8+n-((i>>10)%3),9)));
  /*
  switch(s1){
   default:
   case 0:
     OCR0A =0;   break;
   case 1:

     putb( p1*t*((t>>12|t>>8)&63&t>>4) ); // by tejeez
     
     break;
   case 2:
     putb( ((-t&4095)*(255&t*(t&t>>13))>>12)+(127&t*(234&t>>8&t>>3)>>(3&t>>14)) ); // by tejeez
     break;
   case 3:
     putb( p1*t*(t>>11&t>>8&123&t>>3) );   // by visy
     break;
   case 4:
     putb( p1*t*((t>>9|t>>13)&25&t>>6) );   // by tejeez
     break;
   case 5:
     putb( p1*(t*(t>>5|t>>8))>>(t>>16) ); // by xpansive
     break;
   case 6:
   
     putb( p1*(t*(t>>8|t>>9)&46&t>>8) ); // by skurk (raer's version)
     break;
   case 7:
     putb( p1*(t&4096)?((t*(t^t%255)|(t>>4))>>1):(t>>3)|((t&8192)?t<<2:t) ); // by viznut, xpansive, varjohukka
     break;
   case 8:
     putb( p1*(t>>7|t|t>>6)*10+4*(t&t>>13|t>>6) ); // by miiro
     break;
   case 9:
     putb( p1*(t*5&(t>>7)|t*3&(t*4>>10)) ); // by red
     break;
   case 10:
     long v;
     putb( v= p1*(v>>1)+(v>>4)+t*(((t>>16)|(t>>6))&(69&(t>>9))) ); // by pyryp
     break;
   case 11: 
     putb( p1*(t|(t>>9|t>>7))*t&(t>>11|t>>9) );  //by viznut
     break;
   case 12:
     putb( p1*(t>>6|t|t>>(t>>16))*10+((t>>11)&7) ); // by stimmer
     break;
   case 13:
     putb( p1*(t*(4|7&t>>13)>>((~t>>11)&1)&128) );
     break;
   case 14:
      putb( (sin((float)t*p1)+1.0f)*(float)p2 );
     break;
   case 15:
     // free to use
     break;
     // any more and we need another bit from PORTB
   //  OCR0A = OCR0A/4;
  } 
  */
  
  //checkParamsChanged();
  t++;
}
     


void textUpdate(){
  display.clearDisplay();
  screenText("Song:          ", 4, 3, 1, false); //pot1
  screenText(String(s1), 72, 3, 1, false);
  
  screenText("Param 1:          ", 4, 11, 1, false); //pot2
  screenText(String(p1), 72, 11, 1, false);
  
  screenText("Param 2:          ", 4, 21, 1, false); //pot3
  screenText(String(p2), 72, 21, 1, false);
  
  //display.drawLine(1, 37, 100, 37, WHITE);
 display.drawRect(1, 1, 126,31, WHITE);
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
  
  //delay(100);
}
