#pragma once
class Cubicle {
  public:
    Cubicle();
    Cubicle(int HES, int LED1, int LED2);
    int getDoorState();
    void LEDloop();
    void setLockedTime();
    void resetLockedTime();
    unsigned long getLockedTime();
  private:
    int HESPin;//hall effect sensor
    int LED1Pin;//green
    int LED2Pin;//red
    int LED2State = 0;
    int HSEValue = 0;
    int doorState = 0;//0 means unlocked and 1 means locked
    int doorStatePrev = 0;
    unsigned long startLocking = 0;
    unsigned long lockedTime = 0;
    unsigned long lockedTooLong = 15 * 60 * 1000;
    unsigned long currMillis, prevMillis = 0;
};
/*
  class Cubicle {
  public:
    Cubicle() {}
    Cubicle(int HES, int LED1, int LED2) {
      HESPin = HES;
      LED1Pin = LED1;
      LED2Pin = LED2;
      pinMode(HESPin, INPUT);
      pinMode(LED1Pin, OUTPUT);
      pinMode(LED2Pin, OUTPUT);
    }
    int getDoorState() {
      HSEValue = analogRead(HESPin);
      if (doorState) {//door is locked
        if (HSEValue < 100)
          doorState = 0;
      }
      else {//door is unlocked
        if (HSEValue > 1000) {
          doorState = 1;
          startLocking = millis();
        }
      }
      //doorStatePrev = doorState;
      return doorState;
    }
    void LEDloop() {
      if (lockedTime >= lockedTooLong) {
        if (currMillis - prevMillis >= 1000) {
          prevMillis = currMillis;
          LED2State ^= LED2State;
          digitalWrite(LED2Pin, LED2State);
        }
      }
      else if (doorState) {
        digitalWrite(LED1Pin, HIGH);
        digitalWrite(LED2Pin, LOW);
      }
      else {
        digitalWrite(LED1Pin, LOW);
        digitalWrite(LED2Pin, LOW);
      }
    }
    void setLockedTime() {
      currMillis = millis();
      lockedTime = currMillis - startLocking;
    }
    void resetLockedTime() {
      lockedTime = 0;
    }
    unsigned long getLockedTime() {
      return lockedTime;
    }
  private:
    int HESPin;//hall effect sensor
    int LED1Pin;//green
    int LED2Pin;//red
    int LED2State = 0;
    int HSEValue = 0;
    int doorState = 0;//0 meaans unlocked and 1 means locked
    int doorStatePrev = 0;
    unsigned long startLocking = 0;
    unsigned long lockedTime = 0;
    unsigned long lockedTooLong = 15 * 60 * 1000;
    unsigned long currMillis, prevMillis = 0;
  };

*/
