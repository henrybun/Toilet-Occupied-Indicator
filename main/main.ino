#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "cubicle.h"
#include "toilet.h"
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include "arduino_secrets.h"

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

IPAddress ip(192, 168, 0, 110);

int status = WL_IDLE_STATUS;

//WiFi
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key index number (needed only for WEP)

unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[256]; //buffer to hold incoming packet
char  ReplyBuffer[] = "hi";       // a string to send back
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
  delay(100);

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(100);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  WiFi.config(ip);
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
    //Serial.print((String)i + ": ");
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

  String printMessage = (String) "Cubicle Available:" + (String)toilet.getUnlockedCubicle() +
                        (String) "\nQueuing length: " + (String)toilet.getQueuingLength() +
                        (String) "\nLUT: " + (String)millis2Time(toilet.latestUpdate()) +
                        (String) "\nWaiting Time: " + (String)millis2Time(toilet.getWaitTime());
  String readMessage =  (String) "Toilet 2 State: " +
                        (String) "\nToilet 3 State: ";
  //Serial.println(printMessage);
  //Serial.println(readMessage);
  printScreen(printMessage);
  
  int packetSize = Udp.parsePacket();
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.println(printMessage); //Message send to server
  Serial.println(WiFi.localIP()); //Print message send to server

  int len = Udp.read(packetBuffer, 255);
  if (len > 0) {
    packetBuffer[len] = 0;
  }
  Serial.println("Received contents:");
  Serial.println(packetBuffer);
  Udp.endPacket();

  delay(1000);
}
String millis2Time(unsigned long t) {
  unsigned long totalSecond = t / 1000;
  int hour = totalSecond / 3600;
  int minute = (totalSecond % 3600) / 60;
  int second = (totalSecond % 3600) % 60;
  return String(hour) + String("h") + String(minute) + String("m") + String(second) + String("s");
}
void printScreen(String message) {
  display.clearDisplay();

  display.setTextSize(1);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(message);
  display.display();      // Show initial text
}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:

  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
