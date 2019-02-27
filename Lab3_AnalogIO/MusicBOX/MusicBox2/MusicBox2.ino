#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4978
#define NOTE_DS8 4978

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

int updatedecay = 1000;
long lastupdate = 0;

//This is for the Sound System
int note = 0;
String notetext = "C#";
int param1 = 0;
int param2 = 0;
unsigned char n1 = 0;
unsigned char p1 = 0;
unsigned char p2 = 0;

int timer = 0;
int notedeath = 1000;

int basenote = NOTE_C2;
double PENTATONICSCALE[6] = {1, 1.122, 1.259, 1.496, 1.679, 2};

int currentnote = NOTE_C4;
int currentlength = 1000;

int nextnote = NOTE_D4;
long nextnotelength = 1000;


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
  checkParamsChanged();
  
  pinMode(9, OUTPUT);
}

void checkParamsChanged()
{
  if(param1 != analogRead(1)||param2 != analogRead(3)||note!= analogRead(2)){
    param1 = analogRead(1);
    param2 = analogRead(3);
    note = analogRead(2);
    updateP();
  }
}

void updateP()
{
  p1 = param1/(1024/255);
  p2 = param2/(1024/255);
  n1 = (note /(1024/12)) +1;

  switch(n1){
    case 1:
      basenote = NOTE_B0;
      notetext = "B";
      break;
    case 2:
      basenote = NOTE_C1;
      notetext = "C";
      break;
    case 3:
      basenote = NOTE_CS1;
      notetext = "C#";
      break;
    case 4:
      basenote = NOTE_D1;
      notetext = "D";
      break;
    case 5:
      basenote = NOTE_DS1;
      notetext = "D#";
      break;
    case 6:
      basenote = NOTE_E1;
      notetext = "E";
      break;
    case 7:
      basenote = NOTE_F1;
      notetext = "F";
      break;
    case 8:
      basenote = NOTE_FS1;
      notetext = "F#";
      break;
    case 9:
      basenote = NOTE_G1;
      notetext = "G";
      break;
    case 10:
      basenote = NOTE_GS1;
      notetext = "G#";
      break;
    case 11:
      basenote = NOTE_A1;
      notetext = "A";
      break;
    case 12:
      basenote = NOTE_AS1;
      notetext = "A#";
      break; 
  }

  textUpdate();
  lastupdate = millis();
}


void loop() {
  Serial.print(timer);
  Serial.print(" ");
  Serial.println(nextnotelength);
  
  if(millis() > nextnotelength){
    
    tone(9, nextnote);
    currentnote = nextnote;
    nextnote = basenote * PENTATONICSCALE[(int) random(0, 5)] * random(1, 4);
    int randval = (int) random(0, 5);
    nextnotelength = millis() + 250*((int) random(1, 4));
  }
  checkParamsChanged();
}

void textUpdate(){
  display.clearDisplay();
  screenText("Base Note:          ", 4, 3, 1, false); //pot1
  screenText(notetext, 72, 3, 1, false);
  
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
