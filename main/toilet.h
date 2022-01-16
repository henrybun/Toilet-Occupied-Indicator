#pragma once
class Toilet {
  private:
    int cubicleState[3] = { 0, 0, 0 };//0 means unlocked and 1 means locked
    int lightSensorPin[3];  //light resistor
    int lightSensorState[3];                 //1 means queuing, 0 means not queuing
    unsigned long recentLockedTime[100];
    int lockedTimeIndex = 0;
    double processingTime;
    unsigned long currTime;
    unsigned long prevTime;
  public:
    Toilet(int pin0, int pin1, int pin2);
    void setCubicleState(int index, int state);
    int getCubicleState(int index);
    void setLightSensorState(int index);
    int getLightSensorState(int index);
    int getUnlockedCubicle();
    int getQueuingLength() ;
    void updateTime();
    unsigned long latestUpdate();//in millisecond
};
