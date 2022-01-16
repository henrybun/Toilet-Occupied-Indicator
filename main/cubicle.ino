#include "cubicle.h"
Cubicle::Cubicle() {}
Cubicle::Cubicle(int HES, int LED1, int LED2) {
  HESPin = HES;
  LED1Pin = LED1;
  LED2Pin = LED2;
  pinMode(HESPin, INPUT);
  pinMode(LED1Pin, OUTPUT);
  pinMode(LED2Pin, OUTPUT);
}
int Cubicle::getDoorState() {
  HSEValue = analogRead(HESPin);
  if (doorState) {  //door is locked
    if (HSEValue < 100)
      doorState = 0;
  } else {  //door is unlocked
    if (HSEValue > 1000) {
      doorState = 1;
      startLocking = millis();
    }
  }
  //doorStatePrev = doorState;
  return doorState;
}
void Cubicle::LEDloop() {
  if (lockedTime >= lockedTooLong) {
    if (currMillis - prevMillis >= 1000) {
      prevMillis = currMillis;
      LED2State ^= LED2State;
      digitalWrite(LED2Pin, LED2State);
    }
  } else if (doorState) {
    digitalWrite(LED1Pin, HIGH);
    digitalWrite(LED2Pin, LOW);
  } else {
    digitalWrite(LED1Pin, LOW);
    digitalWrite(LED2Pin, LOW);
  }
}
void Cubicle::setLockedTime() {
  currMillis = millis();
  lockedTime = currMillis - startLocking;
}
void Cubicle::resetLockedTime() {
  lockedTime = 0;
}
unsigned long Cubicle::getLockedTime() {
  return lockedTime;
}