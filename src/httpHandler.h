#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <WiFiNINA.h>
#include "webpageCode.h"

extern int BlueLedState;
extern int RedLedState;
extern const int blueLed;
extern const int redLed;
extern int validResultCounter;
extern int reciveTransmit;
extern bool sendMotorSpeed;

void handleClient(WiFiClient &client);
void sendHttpResponse(WiFiClient &client);
void handleClientRequest(String &request);
void handleMotorPositionRequest(String &request);
void handleMotorInformationRequest(String &request);
void toggleBlueLed();
void toggleRedLed();
void manageListSize(String &list);

#endif
