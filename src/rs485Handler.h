#ifndef RS485HANDLER_H
#define RS485HANDLER_H

#include <ArduinoRS485.h>

extern String completeIncomingByte;
extern String Incommingresult;
extern int reciveTransmit;
extern int validResultCounter;

template<typename T>
void sendData(const T& message);
void receiveData();

#endif
