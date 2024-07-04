//#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoRS485.h>
#include "arduino_secrets.h"
#include "webpageCode.h"
#include "httpHandler.h"
#include "rs485Handler.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const int blueLed = 27;
const int redLed = 25;
int led = LED_BUILTIN;
int BlueLedState = LOW;
int RedLedState = LOW;
int status = WL_IDLE_STATUS;
bool sendMotorSpeed = false;
int validResultCounter = 0;
int reciveTransmit = 1; // 1 == receive, 2 == Transmit (rs485)

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Access Point Web Server");

  pinMode(led, OUTPUT);
  WiFiDrv::pinMode(redLed, OUTPUT);
  WiFiDrv::pinMode(blueLed, OUTPUT);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    while (true);
  }

  delay(10000); // wait for connection
  server.begin();
  printWiFiStatus();
  RS485.begin(57600);
  Serial.println("RS485 communication initialized.");
}

void loop() {
  checkWifiStatus();
  WiFiClient client = server.available();
  if (client) {
    handleClient(client);
  }

  if (reciveTransmit == 1) {
    receiveData();
  }

  if (validResultCounter >= 12) {
    sendMotorSpeed = false;
    validResultCounter = 0;
  }
}

void checkWifiStatus() {
    if (status != WiFi.status()) {
    status = WiFi.status();
    if (status == WL_AP_CONNECTED) {
      Serial.println("Device connected to AP");
    } else {
      Serial.println("Device disconnected from AP");
    }
  }
}
void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
