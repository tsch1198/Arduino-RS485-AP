String list1Items = "";
String list2Items = "";
bool haveSendList = false;
int listcounter = 0;
String motormode = "Position";

void handleClient(WiFiClient &client) {
  Serial.println("New client connected");
  String request = "";
  bool currentLineIsBlank = true;

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      request += c; // build whole request

      if (c == '\n' && currentLineIsBlank) {
        // When you get a blank line (i.e., two newlines in a row), the HTTP request has ended
        sendHttpResponse(client);
        handleClientRequest(request);
        break;
      }
      if (c == '\n') {
        currentLineIsBlank = true;
      } else if (c != '\r') {
        currentLineIsBlank = false;
      }
    }
  }

  client.stop();
  Serial.println("Client disconnected");
}

void sendHttpResponse(WiFiClient &client) {
  
  String htmlResponse = String(webpage);
  htmlResponse.replace("%hardwareversion%", Incommingresult); // replace placeholder Value in the html code with actual Values
  htmlResponse.replace("%List1Items%", list1Items);
  htmlResponse.replace("%List2Items%", list2Items);
  htmlResponse.replace("%motormode%", motormode);
  
  if (Incommingresult.length() > 0 & haveSendList == true){
    list2Items += "<li>" + String(Incommingresult) + "</li>";
    manageListSize(list2Items);
  }
  haveSendList = false;
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.print(htmlResponse);
  client.println();
}

void handleClientRequest(String &request) {
  String requestType = "";

  if (request.indexOf("GET /RT") >= 0) {
    toggleRedLed();
    requestType = "GET /RT";
  } else if (request.indexOf("GET /BT") >= 0) {
    toggleBlueLed();
    requestType = "GET /BT";
  } else if (request.indexOf("GET /hardwareversion") >= 0) {
    sendData("a0");
    requestType = "GET /hardwareversion";
  } else if (request.indexOf("GET /currentsetpoint") >= 0) {
    sendData("e0");
    requestType = "GET /currentsetpoint";
  } else if (request.indexOf("GET /productiondate") >= 0) {
    sendData("a6");
    requestType = "GET /productiondate";
  } else if (request.indexOf("GET /stopmotor") >= 0) {
    sendData("o");
    requestType = "GET /stopmotor";
  } else if (request.indexOf("GET /resumemotor") >= 0) {
    sendData("m");
    requestType = "GET /resumemotor";
  } else if (request.indexOf("GET /clearlist") >= 0) {
    list1Items = "";
    list2Items = "";
    requestType = "GET /clearlist";
  } else if (request.indexOf("GET /currentposition") >= 0) {
    sendData("z");
    requestType = "GET /currentposition";
  } else if (request.indexOf("GET /setmodepos") >= 0) {
    sendData("x0");
    motormode = "Position";
    requestType = "GET /setmodepos";
  } else if (request.indexOf("GET /setmoderpm") >= 0) {
    sendData("x1");
    motormode = "RPM";
    requestType = "GET /setmoderpm";
  } else if (request.indexOf("GET /motorposition?motorposition=") >= 0) {
    handleMotorPositionRequest(request);
    requestType = "GET /motorposition";
  } else if (request.indexOf("GET /motorInformation?motorInformation=") >= 0) {
    handleMotorInformationRequest(request);
    requestType = "GET /motorInformation";
  }

  if (requestType.length() > 0) {
    if (requestType != "GET /clearlist"){
      list1Items += "<li>" + requestType + "</li>";
      listcounter += 1;
      manageListSize(list1Items);
      haveSendList = true;
    } 
    if (requestType == "GET /stopmotor" 
    || requestType == "GET /resumemotor" || requestType == "GET /setmodepos" || requestType == "GET /setmoderpm") {
      list2Items += "<li>no response</li>";
      listcounter += 1;
      manageListSize(list2Items);
    }
  }
}

void handleMotorPositionRequest(String &request) {
  int startIndex = request.indexOf("motorposition=") + 14;
  int endIndex = request.indexOf(" HTTP");
  String motorpositionValue = request.substring(startIndex, endIndex);
  if (motorpositionValue[0] != '-') { // when no leading sign then make it plus
    motorpositionValue = "+" + motorpositionValue;
  }
  // Parse motorpositionValue manually while preserving leading zeros
  int motorposition = atoi(motorpositionValue.c_str());

  if (motorposition != 0 || motorpositionValue.equals("0")) { // Check if conversion was successful or the input is "0"
    Serial.println("motorposition:" + motorpositionValue); // Print the motor position with leading zeros

    String motorcommand = "f0" + motorpositionValue;
    
    sendData(motorcommand);
    Serial.println("starting action");
    delay(1000);
    sendData("m");
  } else {
    Serial.println("Invalid motor position format");
  }
}

void handleMotorInformationRequest(String &request) {
  int startIndex = request.indexOf("motorInformation=") + 17;
  int endIndex = request.indexOf(" HTTP");
  String motorInformationValue = request.substring(startIndex, endIndex);

  Serial.println("motorInformation:" + motorInformationValue);
  sendData(motorInformationValue);
}

void toggleBlueLed() {
  if (BlueLedState == LOW) {
    WiFiDrv::analogWrite(blueLed, 255);
    BlueLedState = HIGH;
  } else {
    WiFiDrv::analogWrite(blueLed, 0);
    BlueLedState = LOW;
  }
}

void toggleRedLed(){
  if (RedLedState == LOW) {
    WiFiDrv::analogWrite(redLed, 255);
    RedLedState = HIGH;
  } else {
    WiFiDrv::analogWrite(redLed, 0);
    RedLedState = LOW;
  }
}

void manageListSize(String &list) {
  int count = 0;
  int pos = 0;

  // Count the number of <li> or </li> items in the list
  while ((pos = list.indexOf("<li>", pos)) >= 0 || (pos = list.indexOf("</li>", pos)) >= 0) {
    count++;
    pos += 4; // Move past the current <li> or </li> tag
  }

  // If there are more than 5 items, remove the oldest ones until the count is 5
  while (count > 5) {
    int firstLiPos = list.indexOf("<li>");
    list.remove(firstLiPos, list.indexOf("</li>", firstLiPos) + 5 - firstLiPos);
    count--; // Decrement count after removing an item
  }
}
