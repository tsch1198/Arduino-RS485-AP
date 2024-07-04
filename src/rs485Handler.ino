#include "RS485Handler.h"

String lastSentMessage = "";
String Incommingresult = "";
String completeIncomingByte = "";

template<typename T>
void sendData(const T& message) {
  lastSentMessage = String(message);
  RS485.beginTransmission();
  RS485.print(message);
  RS485.endTransmission();

  Serial.print("Sent: ");
  Serial.println(message);

  delay(10);
  RS485.receive();
  delay(200);

  reciveTransmit = 1;
}

void receiveData() {
  RS485.receive();
  Serial.println("Receiving: ");
  delay(1000);

  if (RS485.available()) {
    completeIncomingByte = ""; // Clear previous data
    Incommingresult = ""; // Clear previous data
    while (RS485.available()) {
      int incomingByte = RS485.read();

      Serial.write(incomingByte);
      Serial.print(" (");
      Serial.print(incomingByte, DEC);
      Serial.print(") ");

      completeIncomingByte += char(incomingByte);
      Serial.print("Received: ");
      Serial.println(completeIncomingByte);
    }
    Serial.println("Complete incoming byte: " + completeIncomingByte);

    // Subtract the sent message from the received message
    int pos = completeIncomingByte.indexOf(lastSentMessage);
    if (pos != -1) {
      Incommingresult = completeIncomingByte.substring(0, pos) + completeIncomingByte.substring(pos + lastSentMessage.length());
      Incommingresult.trim();
      if (Incommingresult.endsWith(">")) {
        Incommingresult.remove(Incommingresult.length() - 1);
      }
      Serial.println("Message after subtraction: " + Incommingresult);
    } else {
      Serial.println("Sent message not found in the received message.");
    }
  } else {
    Serial.println("No data received.");
  }

  delay(10);
  RS485.beginTransmission(); // Restart transmission mode
}
