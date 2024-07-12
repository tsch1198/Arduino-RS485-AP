<div align="center">
  <p>
      <img width="20%" src="/images/arduino-icon.svg">
  </p>
  <b><h3> Arduino RS485 </h3></b>
  <p> Arduino Hosted Website Control for RS485 commands used to Control a RS485 device</a></p>
</div>
<br>

<br>

**Hey, this is a little project of mine, that i've been working on the past week. Its an Arduino script, which sets up an AP and hosts a website with which you can control a RS485 device.**

# Items used
1. [Arduino MKR 1010 Wifi](https://store.arduino.cc/products/arduino-mkr-wifi-1010)
2. [Arduino MKR 485 Shield](https://store.arduino.cc/products/arduino-mkr-485-shield)
3. RS485 device
4. RS485 to USB Converter (for debugging and testing)
5. Cables

# Apps and tools
 &nbsp;  &nbsp; <a href="https://www.arduino.cc/en/software"><img src="/images/arduino-ide-logo.svg" width="40"></a>

Libraries:
- [WifiNINA](https://www.arduino.cc/reference/en/libraries/wifinina/)
- [ArduinoRS485](https://www.arduino.cc/reference/en/libraries/arduinors485/)

# Instructions

## Step 1: Test the Arduino
The first step is to test if the Arduino works as it should.

1. Plug the MKR Wifi 1010 in your PC and boot up the Arduino IDE
2. Install the WiFiNINA library
3. Open the WiFiNINA example "AP_SimpleWebServer"
4. Upload the example to the board
5. Connect to the AP and open the website to make sure, that everything is working correctly

## Step 2: Connect and test the RS485 shield
1. Plug the RS485 shield directly on top of the Arduino
<img src="/images/IMG_20240606_081745620.jpg" alt="Pluged in RS485 Shield" width="800"/>
(Ignore the black cable)
2. Make sure that the switches are in the following Position:
   1. OFF
   2. OFF
   3. ON
3. Then connect your Y and Z cables to your RS485 device

For testing:
1. Connect the other ends of the Y and Z cables to your USB adapter.
2. Install the ArduinoRS485 library and upload this code to your Arduino:
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
1. After the upload was successful, open a serial monitor on the port of your RS485 to USB device (don't forget to use matching Baud rates)
2. If everything is set up correctly, you should see a bunch of "hello xx"

(for more details, visit -> [Arduino RS485 Shield Tutorial](https://docs.arduino.cc/tutorials/mkr-485-shield/mkr-485-communication/))

## Step 3: Set up the main code
1. Open the given files in your IDE and change the [AP Settings](./src/arduino_secrets.h) to your liking.
2. In the [main file](./src/ArduinoWLAN_1_2.ino) change the serial (line 25) and RS485 (line 58) Baud rates to your needs.
3. Upload the code to your Arduino and open the website. The IP address is printed in the serial monitor.
4. Now you should see the website I made [explanation of the website](#website).
5. To change it to your needs, check out [Personalize](#personalize).

## Website:
<p align="center" width="100%">
    <img src="/images/website.png" alt="Default Website">
</p>

In the basic configuration, the website works this way, everything you put in or click is being send via RS485.
In the Request(1) List it shows the action you have clicked and in the Response(2) it shows the response.

**Note, the webserver runs on Arduino, so it's not very fast. You have to manually refresh after every interaction to see a response**

- Set Position:
Sends a f0<-given number-> used for setting the destination position
- Send Anything:
Sends your input

- Current Motor Mode:
Live updated value for current Motormode

- The 8 Buttons:
They send the command which is labeled on them used for said things

- Clear List (The ninth button):
Clearing the list

- Request(1):
Shows the action you have clicked

- Response(2):
Shows the response for your command
# Personalize
You can customize the code so that it fits your needs.

## How it Works
The whole website to server communication works with /something calls.
For example:
```HTML
      <div class="flex-row">
        <a href="/hardwareversion" class="btn">a0: Hardwareversion</a>
        <a href="/productiondate" class="btn">a6: production date</a>
        <a href="/currentsetpoint" class="btn">e0: Current setpoint</a>
      </div>
```
If you click the one of the buttons, a /something is being sent. This will be caught by the [httphandler](/src/httpHandler.ino) script.

The ```handleClientRequest``` function checks if the request is for a specific endpoints (e.g., /RT, /BT, /hardwareversion, etc.) and performs corresponding actions like toggling LEDs or sending data.

The variable values are marked with %something% in the [website code](/src/webpageCode.h). In the ```sendHttpResponse``` function in [httpHandler](/src/httpHandler.ino) these values are replaced with the real up-to-date values.

## Website
The website's Code can be edited with HTML and CSS.

Source code for the website -> [webpageCode.h](/src/webpageCode.h)
The websites code has to be saved in the webpage char.

### Example: Change a Button
To change for example the ```a0: Hardwareversion``` button, you would:
1. Change the ```/hardwareversion``` in [WebpageCode](/src/webpageCode.h) to something else.
2. In the [httpHandler](/src/httpHandler.ino) you have to change the entry in ```handleClientRequest``` to a new name.
3. Then either put your code directly in the else if or call a function.

### Example: Change Header
1. Open [webpageCode.h](/src/webpageCode.h).
2. Navigate to the body and find the H1 section.
3. Change AG05 Control to something different.
```HTML
    <h1>AG05 Control</h1>
```
