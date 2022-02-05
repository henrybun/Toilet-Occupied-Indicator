#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "cubicle.h"
#include "toilet.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI 11
#define OLED_CLK 13
#define OLED_DC 12
#define OLED_CS 10
#define OLED_RESET 9
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

Cubicle sys[3] = { Cubicle(A5, 2, 3), Cubicle(A6, 4, 5), Cubicle(A7, 6, 7) };
Toilet toilet(A0, A1, A2);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(500);

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 3; i++) {
    Serial.print((String)i + ": ");
    int temp = sys[i].getDoorState();
    toilet.setCubicleState(i, temp);//~300 or ~700 in analogRead
    if (toilet.getCubicleState(i)) {
      sys[i].setLockedTime();//update Cubicle.currMillis
    }
    else {
      sys[i].resetLockedTime();
    }
    sys[i].LEDloop();
    toilet.setLightSensorState(i);
  }
  toilet.updateTime();
  //average queuing length
  //Little's formula, L=lamda*Wq
  String printMessage = (String) "Cubicle Available:" + (String)toilet.getUnlockedCubicle() +
                        (String) "\nQueuing length: " + (String)toilet.getQueuingLength() +
                        (String) "\nLUT: " + (String)millis2Time(toilet.latestUpdate()) +
                        (String) "\nWaiting Time: " + (String)millis2Time(toilet.getWaitTime());
  String readMessage =  (String) "Toilet 2 State: " +
                        (String) "\nToilet 3 State: ";
  //Serial.println(printMessage);
  //Serial.println(readMessage);
  myScroll(printMessage, readMessage);
  delay(500);
}
String millis2Time(unsigned long t) {
  unsigned long totalSecond = t / 1000;
  int hour = totalSecond / 3600;
  int minute = (totalSecond % 3600) / 60;
  int second = (totalSecond % 3600) % 60;
  return String(hour) + String("h") + String(minute) + String("m") + String(second) + String("s");
}
void myScroll(String message1, String message2) {
  display.clearDisplay();

  display.setTextSize(1);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(message1);
  display.println(message2);
  display.display();      // Show initial text
  //delay(100);
  // Scroll in various directions, pausing in-between:
  //delay(1000);
}
