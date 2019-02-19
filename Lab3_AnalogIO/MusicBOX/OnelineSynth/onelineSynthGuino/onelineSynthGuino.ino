/*
  GUINO DASHBOARD TEMPLATE FOR THE ARDUINO. 
 Done by Mads Hobye as a part of Instructables (AIR Program) & Medea (PhD Student).
 Licens: Creative Commons — Attribution-ShareAlike
 
 It should be used with the GUINO Dashboard app.
 
 More info can be found here: www.hobye.dk
 
 # This is your main template to edit.
 */

// 


// Dummy example variables
int song = 0;
int param1 = 0;
int param2 = 0;
int param3 = 0;
unsigned char s1 = 0;
unsigned char p1 = 0;
unsigned char p2 = 0;
unsigned char p3 = 0;
int graphValue = 0;
int guinoControl = false;


// SOUND SYSTEM


#define SAMPLE_MAX (65535.0)
#define SAMPLE_FREQUENCY (8000.0)
#define TIMER1_FREQUENCY 2000000
#define UPDATE_RATE 8000

// by keeping t global we can use it to drive the visualiser as well
long t;


// iterate the grains and LFO 
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

     OCR0A = ((-t&4095)*(p1&t*(t&t>>13))>>12)+(p2&t*(p3&t>>8&t>>3)>>(3&t>>14)); // by tejeez
     
     break;
   case 2:
     OCR0A = t*(t>>p1&t>>8&p2&t>>p3); // by tejeez
     break;
   case 3:
     OCR0A = t*((t>>p1|t>>p2)&p3&t>>6);   // by visy
     break;
   case 4:
     OCR0A = (t*(t>>p1|t>>p2))>>(t>>p3);   // by tejeez
     break;
   case 5:
     OCR0A = ((t*(t>>p1|t>>9)&p2&t>>8))^(t&t>>p3|t>>6); // by xpansive
     break;
   case 6:
   
     OCR0A = ((t&p1)?((t*(t^t%p2)|(t>>4))>>1):(t>>3)|((t&param3)?t<<2:t)); // by skurk (raer's version)
     break;
   case 7:
     OCR0A = (t>>7|t|t>>6)*p1+p2*(t&t>>p3|t>>6); // by viznut, xpansive, varjohukka
     break;
   case 8:
     OCR0A = t*p1&(t>>p2)|t*3&(t*p3>>10); // by miiro
     break;
   case 9:
     OCR0A = (t|(t>>p1|t>>p2))*t&(t>>p3|t>>9); // by red
     break;
   case 10:
     long v;
     OCR0A = v=(v>>1)+(v>>4)+t*(((t>>p1)|(t>>6))&(p2&(t>>p3))); // by pyryp
     break;
   case 11: 
     OCR0A = (t>>6|t|t>>(t>>p1))*p2+((t>>p3)&7);  //by viznut
     break;
   case 12:
     OCR0A = (t*(4|p1&t>>13)>>((~t>>11)&1)&p1) + ((t)*(t>>p3&t>>13)*((~t>>9)&3)&127); // by stimmer
     break;
   case 13:
     OCR0A = (t>>p3|t|t>>(t>>p1))*p2   ;
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

void setupSound()
{
  TCCR1A=0x0;          // set the timer prescaler to 8 = 16/8 = 2MHz
  TCCR1B=0x02;          // set the timer prescaler to 8 = 16/8 = 2MHz
  TIMSK1 |= (1<<OCIE1A);   // Enable output compare match interrupt on OCR1A
   
  TCCR0A=0B10000011;                                    //-8 bit audio PWM
TCCR0B=0x01;          // Set to clock frequency, no prescaler
  OCR0A=127;            // set in the middle - do we need this ? probably not.
  DDRB|=1<9;          // Set digital pin 6 to output - channels 2 and 3

  DDRB &= (~15); // set digital pins 8,9,10,11 as inputs 
  DDRD |= ((1<<2) | (1<<3) | (1<<4) | (1<<5)); // set digital pins 2,3,4,5 as outputs for the visualiser
  DDRC &= (15); 
}

void paramChanged(int * param, int value)
{
  if(!guinoControl)
  {
    *param = value;
    gUpdateValue(param);
    
  }
}

void updateP()
{
   p1 = param1/(1024/255);
  p2 = param2/(1024/255);
  p3 = param3/(1024/255); 
  s1 = song /(1025/14);
}

void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
  // Start the guino dashboard interface.
  // The number is your personal key for saving data. This should be unique for each sketch
  // This key should also be changed if you change the gui structure. Hence the saved data vill not match.
 delay(100);
  gBegin(3436); 
  delay(100);
  setupSound();

}


void loop(){
  // **** Main update call for the guino
  guino_update();
   paramChanged(&song,analogRead(0));
   paramChanged(&param1,analogRead(1));
   paramChanged(&param2,analogRead(2));
   paramChanged(&param3,analogRead(3));
   if(!guinoControl) updateP();
   

  unsigned char output = OCR0A;
  graphValue = OCR0A;

  // Send the value to the gui.
  gUpdateValue(&graphValue);
//gUpdateValue(random(0,100));

 

}

// This is where you setup your interface 
void gInit()
{

  gAddLabel("SLIDERS",1);
  gAddSpacer(1);
  gAddSlider(0,1024,"Song",&song);
  gAddSlider(0,1024,"Param 1",&param1);
  gAddSlider(0,1024,"Param 2",&param2);
  gAddSlider(0,1024,"Param 3",&param3);
 
  gAddToggle("Guino control", &guinoControl);

  gAddColumn();

  gAddLabel("GRAPHS",1);
  gAddSpacer(1);

  // Last parameter in moving graph defines the size 10 = normal
  gAddMovingGraph("SINUS",0,255, &graphValue, 20);
  gAddSlider(-255,255,"VALUE",&graphValue);

  // The graphs take up two columns we are going to add two
  gAddColumn();


}

// Method called everytime a button has been pressed in the interface.
void gButtonPressed(int id)
{

}

void gItemUpdated(int id)
{
   updateP();
}
