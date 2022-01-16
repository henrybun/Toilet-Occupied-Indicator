#include "toilet.h"
Toilet::Toilet(int pin0, int pin1, int pin2) {
  lightSensorPin[0] = pin0;
  lightSensorPin[1] = pin1;
  lightSensorPin[2] = pin2;
  for (int i = 0; i < 3; i++) {
    pinMode(lightSensorPin[i], INPUT);
  }
}
void Toilet::setCubicleState(int index, int state) {
  cubicleState[index] = state;
}
int Toilet::getCubicleState(int index) {
  return cubicleState[index];
}
void Toilet::setLightSensorState(int index) {
  int temp = analogRead(lightSensorPin[index]);
  if (temp < 100)  //light sensor is covered
    lightSensorState[index] = 1;      //one person is queuing
  else
    lightSensorState[index] = 0;
}
int Toilet::getLightSensorState(int index) {
  return lightSensorState[index];
}
int Toilet::getUnlockedCubicle() {
  int temp = 0;
  for (int i = 0; i < 3; i++) {
    if (cubicleState[i] == 0)
      temp++;
  }
  return temp;
}
int Toilet::getQueuingLength() {
  int temp = 0;
  for (int i = 0; i < 3; i++) {
    if (lightSensorState[i])
      temp++;
  }
  return temp;
}
void Toilet::updateTime() {
  prevTime = currTime;
  currTime = millis();
}
unsigned long Toilet::latestUpdate() {
  return currTime - prevTime;
}
