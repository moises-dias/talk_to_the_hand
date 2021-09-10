void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  //Serial.print(4100);
  //Serial.print(" ");
  Serial.print(analogRead(25));
  Serial.print(" ");
  Serial.print(analogRead(33));
  Serial.print(" ");
  Serial.print(analogRead(32));
  Serial.print(" ");
  Serial.print(analogRead(35));
  Serial.print(" ");
  Serial.println(analogRead(34));
  delay(100);
}
