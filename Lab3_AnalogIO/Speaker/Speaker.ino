const int speakerOut = 10;
const int photos = A0;

int value;

void setup() {
  // put your setup code here, to run once:
  pinMode(10, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(photos);
  
  Serial.println(value);

  //Average 460,
  //low 130
  //high 900

  int hzout = map(value, 130, 900, 10, 1000);
  tone(speakerOut, hzout);
  delay(10);
  
}
