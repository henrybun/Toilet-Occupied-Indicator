#pragma once
#define CUBICLE_COUNT 3
#define HOLD_TIME 7.5*60*1000
class Toilet {
  private:
    int cubicleState[CUBICLE_COUNT] = { 0, 0, 0 };//0 means unlocked and 1 means locked
    int lightSensorPin[3];  //light resistor
    int lightSensorState[3];                 //1 means queuing, 0 means not queuing
    int queuingLength = 0;
    int unlockedCubicle = 0;
    unsigned long recentLockedTime[100];
    int lockedTimeIndex = 0;
    double processingTime;
    unsigned long currTime;
    unsigned long prevTime;
    unsigned long waitTime;
  public:
    Toilet(int pin0, int pin1, int pin2) {
      lightSensorPin[0] = pin0;
      lightSensorPin[1] = pin1;
      lightSensorPin[2] = pin2;
      for (int i = 0; i < 3; i++) {
        pinMode(lightSensorPin[i], INPUT);
      }
    }
    void setCubicleState(int index, int state) {
      cubicleState[index] = state;
    }
    int getCubicleState(int index) {
      return cubicleState[index];
    }
    void setLightSensorState(int index) {
      int temp = analogRead(lightSensorPin[index]);
      if (temp < 100)  //light sensor is covered
        lightSensorState[index] = 1;      //someone is queuing
      else
        lightSensorState[index] = 0;
    }
    int getLightSensorState(int index) {
      return lightSensorState[index];
    }
    int getUnlockedCubicle() {
      unlockedCubicle = 0;
      for (int i = 0; i < 3; i++) {
        if (cubicleState[i] == 0)
          unlockedCubicle++;
      }
      return unlockedCubicle;
    }
    int getQueuingLength() {
      queuingLength = 0;
      for (int i = 0; i < 3; i++) {
        if (lightSensorState[i])
          queuingLength++;
      }
      return queuingLength;
    }
    void updateTime() {
      prevTime = currTime;
      currTime = millis();
    }
    unsigned long latestUpdate() {
      return currTime - prevTime; //in millisecond
    }
    unsigned long getWaitTime() {
      if (unlockedCubicle > 0)
        return 0;
      waitTime = (queuingLength + 1) * HOLD_TIME / CUBICLE_COUNT;
      return waitTime;
    }
};
