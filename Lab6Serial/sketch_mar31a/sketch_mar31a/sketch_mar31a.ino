const int sampleWindow = 2; // Sample window width in mS (250 mS = 4Hz)
int ledPin = 9;

void setup() 
{
   Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() 
{
 


 Serial.println(analogRead(A0));
}
