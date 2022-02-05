#pragma once
class Cubicle {
  public:
    Cubicle();
    Cubicle(int HES, int LED1, int LED2) {
      HESPin = HES;
      LED1Pin = LED1;
      LED2Pin = LED2;
      pinMode(HESPin, INPUT);
      pinMode(LED1Pin, OUTPUT);
      pinMode(LED2Pin, OUTPUT);
    };
    int getDoorState() {
      HSEValue = analogRead(HESPin);
      //Serial.println(HSEValue);
      if (doorState == 0) { //door is locked
        if ((HSEValue <= 460) || (HSEValue >= 560)) {
          doorState = 1;
          startLocking = millis();
          //Serial.println((String)"startLocking: " + startLocking);
        }
      }
      else {//door is unlocked
        if ((HSEValue < 560) && (HSEValue > 460)) {
          doorState = 0;
        }
      }
      return doorState;
    }
    void LEDloop() {
      if (lockedTime >= lockedTooLong) {
        if (currMillis - prevMillis >= 1000) {
          prevMillis = currMillis;
          LED2State ^= 1;
          digitalWrite(LED2Pin, LED2State);
          //Serial.println((String)"prevMillis: " + prevMillis);
        }
      }
      else if (doorState) {
        digitalWrite(LED1Pin, LOW);
        digitalWrite(LED2Pin, HIGH);
      }
      else {
        digitalWrite(LED1Pin, HIGH);
        digitalWrite(LED2Pin, LOW);
      }
    }
    void setLockedTime() {
      currMillis = millis();
      lockedTime = currMillis - startLocking;
      //Serial.println((String)"currMillis: " + currMillis);
      //Serial.println((String)"lockedTime: " + lockedTime);
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
    int doorState = 0;//0 means unlocked and 1 means locked
    int doorStatePrev = 0;
    unsigned long startLocking = 0;
    unsigned long lockedTime = 0;
    unsigned long lockedTooLong = 10 * 1000;
    //unsigned long lockedTooLong = 15 * 60 * 1000;
    unsigned long currMillis, prevMillis = 0;
};
