#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include "arduino_secrets.h"
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
//sys[0].Cubicle(A5, 2, 3);
//sys[1].Cubicle(A6, 4, 5);
//sys[2].Cubicle(A7, 6, 7);

Toilet toilet(A0, A1, A2);
/*
  int doorState[3] = { 0, 0, 0 };
  int lightSensorPin[3] = { A0, A1, A2 };  //light resistor
  int lightSensorState[3];                 //1 means queuing, 0 means not queuing
  unsigned long recentLockedTime[100];
  int lockedTimeIndex = 0;
  double processingTime;
*/

int status = WL_IDLE_STATUS;
//WiFi
char ssid[] = SECRET_SSID;  // your network SSID (name)
char pass[] = SECRET_PASS;  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;           // your network key index number (needed only for WEP)

unsigned int localPort = 2390;  // local port to listen on

char packetBuffer[256];     //buffer to hold incoming packet
char ReplyBuffer[] = "hi";  // a string to send back
WiFiUDP Udp;

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

  /*
    for (int i = 0; i < 100; i++) recentLockedTime[i] = 0;  //init
    for (int i = 0; i < 3; i++) pinMode(lightSensorPin[i], INPUT);
  */

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:
    delay(5000);
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 3; i++) {
    toilet.setCubicleState(i, sys[i].getDoorState());
    //doorState[i] = sys[i].getDoorState();
    if (toilet.getCubicleState(i))
      sys[i].setLockedTime();
    else {
      /*
        recentLockedTime[lockedTimeIndex] = sys[i].getLockedTime();
        if (lockedTimeIndex < 100)
        lockedTimeIndex++;
        else
        lockedTimeIndex = 0;
      */
      sys[i].resetLockedTime();
    }
    sys[i].LEDloop();
    toilet.setLightSensorState(i);
  }
  toilet.updateTime();
  /*
    processingTime = 0;
    for (int i = 0; i < 100; i++) {
    if (recentLockedTime[i] > 60 * 1000)
      processingTime += recentLockedTime[i];
    }
  */
  //average queuing length
  //Little's formula, L=lamda*Wq
  String printMessage = (String) "Cubicle Available:" + (String)toilet.getUnlockedCubicle() +
                        (String) "\nQueuing length: " + (String)toilet.getQueuingLength() +
                        (String) "\nLatest Update Time: " + (String)millis2Time(toilet.latestUpdate());
  String readMessage =  (String) "Toilet 2 State: " +
                        (String) "\nLatest Update Time: " +
                        (String) "\nToilet 3 State: " +
                        (String) "\nLatest Update Time: ";
  Serial.println(printMessage);
  Serial.println(readMessage);
  /*
    Serial.println("Door State:");
    Serial.println("Door 1: " + printDoorState(doorState[0]));
    Serial.println("Door 2: " + printDoorState(doorState[1]));
    Serial.println("Door 3: " + printDoorState(doorState[2]));
    Serial.println("Queuing length: " + (String)printQueuingLength(lightSensorState));
  */

  myScroll(printMessage, readMessage);

  int packetSize = Udp.parsePacket();
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.println(printMessage);
  /*
    Udp.println("I'm 192.168.0.101"); //Message send to server
    Serial.println("I'm 192.168.0.101"); //Print message send to server
  */

  int len = Udp.read(packetBuffer, 255);
  if (len > 0) {
    packetBuffer[len] = 0;
  }
  Serial.println("Received contents:");
  Serial.println(packetBuffer);
  Udp.endPacket();
  delay(1000);
}
/*
  int getLightSensorState(int index) {
  int temp = analogRead(lightSensorPin[index]);
  if (temp < 100)  //light sensor is covered
    return 1;      //one person is queuing
  else
    return 0;
  }
  String printDoorState(int state) {
  if (state)
    return "close";
  else
    return "open";
  }
  int printQueuingLength(int lightSensorState[]) {
  int temp = 0;
  for (int i = 0; i < 3; i++) {
    if (lightSensorState[i])
      temp++;
  }
  return temp;
  }
  int getUnlockedDoor(int doorState[]) {
  int temp = 0;
  for (int i = 0; i < 3; i++) {
    if (doorState == 0)
      temp++;
  }
  return temp;
  }
*/
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
  /*
    display.println("Door State:");
    display.println("Door 1: " + printDoorState(doorState[0]));
    display.println("Door 2: " + printDoorState(doorState[1]));
    display.println("Door 3: " + printDoorState(doorState[2]));
    display.println("Queuing length: " + (String)printQueuingLength(lightSensorState));
    display.display();      // Show initial text
    delay(100);
  */

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0xFF);
  /*
    delay(10000);
    display.stopscroll();
    delay(1000);
    display.startscrollleft(0x00, 0xFF);
    delay(2000);
    display.stopscroll();
    delay(1000);
  */
}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
