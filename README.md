<div align="center">
  <p>
      <img width="20%" src="/images/arduino-icon.svg">
  </p>
  <b><h3> Arduino RS485 </h3></b>
  <p> Arduino Hosted Website Control for RS485 commands used to Control a RS485 Device</a></p>
</div>
<br>

<br>

**Hey, this is a little Project of mine, that i have been working on the past week. Its an Arduino Script wich sets up an AP and hosts a Website with wich you can control a RS485 Device.**

# Items Used
1. [Arduino MKR 1010 Wifi](https://store.arduino.cc/products/arduino-mkr-wifi-1010)
2. [Arduino MKR 485 Shield](https://store.arduino.cc/products/arduino-mkr-485-shield)
3. RS485 Device
4. RS485 to USB Converter (for debugging and testing)
5. Cables

# Apps and Tools
 &nbsp;  &nbsp; <a href="https://www.arduino.cc/en/software"><img src="/images/arduino-ide-logo.svg" width="40"></a>

Libraries:
- [WifiNINA](https://www.arduino.cc/reference/en/libraries/wifinina/)
- [ArduinoRS485](https://www.arduino.cc/reference/en/libraries/arduinors485/)

# Instructions

## Step 1: Test the Arduino
The first step is to test if the Arduino works as it should.

1. Plug the MKR Wifi 1010 in your PC and Boot up the Arduino IDE
2. Install the WiFiNINA Library
3. Open the WiFiNINA Example "AP_SimpleWebServer"
4. Upload the Example to the Board
5. Connect to the AP and open the Website to check if everything works

## Step 2: Connect and Test the RS485 Shield
1. Plug the RS485 Shield directly on top of the Arduino
<img src="/images/IMG_20240606_081745620.jpg" alt="Pluged in RS485 Shield" width="800"/>
(Ignore the Black Cable)
2. The Switches need to be in this Position:
   1. Off
   2. Off
   3. ON
3. Then Connect your Y and Z Cables to your RS485 Device

For Testing:
1. Connect the other ends of the Y and Z cable to your RS485 to USB adapter.
2. Install the ArduinoRS485 Library in your IDE and Upload this code to your Arduino:
```Cpp
#include <ArduinoRS485.h>

int counter = 0;

void setup() {
  RS485.begin(9600);
}

void loop() {
  RS485.beginTransmission();
  RS485.print("hello ");
  RS485.println(counter);
  RS485.endTransmission();

  counter++;

  delay(1000);
}
```
3. After it's finished uploading, open a Serial Monitor on the Port your RS485 to USB Device is Connected (don't forget to set the Same Baud rate)
4. If everything is set up correctly, you should see a bunch of "hello xx"

(for more details, visit -> [Arduino RS485 Shield Tutorial](https://docs.arduino.cc/tutorials/mkr-485-shield/mkr-485-communication/))

## Step 3: Set Up the Main code
1. Open the given files in your IDE and Change the [AP Settings](./src/arduino_secrets.h) to your liking.
2. In the [Main File](./src/ArduinoWLAN_1_2.ino) change the Serial (line 25) and RS485 (line 58) Baud rates to your needs.
3. Upload the Code to your Arduino and Open the Website. The IP address is Printed in the Serial Monitor.
4. Now you should see the Website I made [Explanation of the Website](#website).
5. To Change it to your Needs, check out [Personalize](#personalize).

## Website:
<p align="center" width="100%">
    <img src="/images/website.png" alt="Default Website">
</p>

In the Basic Configuration, the website works this way, everything you put in or click is being send via RS485.
In the Request(1) List it shows the action you have clicked and in the Response(2) it shows the Response.

**Note, the webserver runs on Arduino, so it's not very fast. You have to refresh after every interaction to see a Response**

- Set Position:
Sends a f0<-given number-> used for setting the Destination Position
- Send Anything:
Sends your input

- Current Motor Mode:
Live updated Value for current Motormode

- The 8 Buttons:
They send the command which is labeled on them used for said things

- Clear List (The ninth button):
Clearing the list

- Request(1):
Shows the action you have clicked

- Response(2):
Shows the Response for your command


# Personalize
You can customize the code so that it fits your needs.

## How it Works
The whole Website to Server Communication works with /something calls.
For Example:
```HTML
      <div class="flex-row">
        <a href="/hardwareversion" class="btn">a0: Hardwareversion</a>
        <a href="/productiondate" class="btn">a6: production date</a>
        <a href="/currentsetpoint" class="btn">e0: Current setpoint</a>
      </div>
```
If you click the one of the buttons, a /something is being sent. This will be caught by the [httphandler](/src/httpHandler.ino) script.

The ```handleClientRequest``` function checks if the request is for a specific endpoints (e.g., /RT, /BT, /hardwareversion, etc.) and performs corresponding actions like toggling LEDs or sending data.

The Variable Values are marked with %something% in the [website code](/src/webpageCode.h). In the ```sendHttpResponse``` function in [httpHandler](/src/httpHandler.ino) these values are replaced with the real up-to-Date values.

## Website
The Website's can be edited with HTML and CSS.

Source Code for the Website -> [webpageCode.h](/src/webpageCode.h)
The Websites Code has to be saved in the Webpage Char.

### Example: Change a Button
To Change for Example the ```a0: Hardwareversion``` Button, you would:
1. Change the ```/hardwareversion``` in [WebpageCode](/src/webpageCode.h) to something else.
2. In the [httpHandler](/src/httpHandler.ino) you have to change the Entry in ```handleClientRequest``` to a new name.
3. Then either put your code directly in the else if or call a function.

### Example: Change Header
1. Open [webpageCode.h](/src/webpageCode.h).
2. Navigate to the Body and find the H1 Section.
3. Change AG05 Control to something Different.
```HTML
    <h1>AG05 Control</h1>
```
