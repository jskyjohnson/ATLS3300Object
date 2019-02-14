int pot;
int light;

const int potentpin = A0;
const int lightpin = A1;

const int light1pin = 9;
const int light2pin = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  pot = analogRead(potentpin);
  light = analogRead(lightpin);

  Serial.print("pot: ");
  Serial.print(pot);
  Serial.print("\t light: ");
  Serial.println(light);

  int potOut = map(pot, 0, 1023, 0, 255);
  int lightOut = map(light, 400, 900, 0, 255);
  analogWrite(light1pin, potOut);
  analogWrite(light2pin, lightOut);
}
