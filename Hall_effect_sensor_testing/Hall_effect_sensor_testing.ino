int sensorPin = A7;
int sensorValue = 0;
int state = 0;


void setup() {
  Serial.begin(9600);

}

void loop() {
  sensorValue = analogRead(sensorPin);
  if (sensorValue < 200)
    state = 0;
  else state = 1;
  Serial.print(state);
  Serial.print("\n");
  delay(2000);
}
