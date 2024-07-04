<div align="center">
  <p>
      <img width="20%" src="/images/arduino-icon.svg">
  </p>
  <b><h3> Arduino RS485 </h3></b>
  <p> Arduino Hosted Website Controll for RS485 commands used to Controll a SIKO AG05</a></p>
</div>
<br>

<br>

# Items Used
1. [Arduino MKR 1010 Wifi](https://store.arduino.cc/products/arduino-mkr-wifi-1010)
2. [Arduino MKR 485 Shield](https://store.arduino.cc/products/arduino-mkr-485-shield)
3. [SIKO GmbH AG05](https://www.siko-global.com/de-de/produkte/driveline-positionierantriebe/feldbus-stellantriebe/ag05-feldbus) (choose any RS485 device you want to controll)
4. RS485 to USB Converter (for debuging and testing)
5. Cables

# Apps and Tools
 &nbsp;  &nbsp; <a href="https://www.arduino.cc/en/software"><img src="/images/arduino-ide-logo.svg" width="40"></a>

Libraries:
- [WifiNINA](https://www.arduino.cc/reference/en/libraries/wifinina/)
- [ArduinoRS485](https://www.arduino.cc/reference/en/libraries/arduinors485/)

# Instructions
**I will describe how to Hook up a RS485 device to the Arduino. In my Case its a SIKO GmbH AG05 but you can choose any other device.**

**This Program is only testet on the AG05 tho**

## Step 1: Test the Arduino
The first Step is to Test if the Arduino works as it should.

1. Plug the MKR Wifi 1010 in your PC and Boot up the Arduino IDE
2. Install the WiFiNINA Library
3. Open the WiFiNINA Example "AP_SimpleWebServer"
4. Upload the Example to the Board
5. Connect to the AP and open the Website to check if everything works

## Step 2: Connect and Test the RS485 Shield
1. Plug the RS485 Shield directly on top of the Arduino
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
3. After its finished uploading, open a Serial Monitor on the Port your RS485 to USB Device is Connected (dont forget to set the Same Baudrate)
4. If everything is set up correctly you should see a bunch of "hello xx"

(for a more details visit -> [Arduino RS485 Shield Tutorial](https://docs.arduino.cc/tutorials/mkr-485-shield/mkr-485-communication/))

## Step 3: Set Up the Main code
1. Open the given files in your IDE and Change the [AP Settings](./src/arduino_secrets.h) to your liking
2. In the [Main File](./src/ArduinoWLAN_1_2.ino) change the Serial (line 25) and RS485 (line 58) Baudrate to your needs
3. Upload the Code to your Arduino and Open the Website. The IP adress is Printed in the Serial Monitor.
4. Now you should see the Website i made for the Controll of the AG05. [Explanation of the Website](#website)
5. To Change it to your Needs check out [Personalize](#personalize)


## Website:
![Default Website](/images/website.png "Website")

In the Basic Configuration the website works this way, everything you but in or click is being send via RS485.
In the Request(1) it shows the command you have send and in the Response(2) it shows the Response.

**Note, the webserver runs on Arduino so its not very fast. You have to refresh after every interaction to see a Response**

- Set Position:
Sends a f0<-given number-> used for setting the Destination Position
- Send Anything:
Sends your input

- Current Motor Mode
Shows if the AG05 is in RPM or Positioning Mode

- The 8 Buttons
They send the command which is labled on them used for said things

- Clear List (The ninth button)
Clearing the list

- Request(1)
Shows the commands you have send

- Response(2)
Shows the Response for your command


# Personalize
You can / have to customize the code so that it works for your needs.

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
If you click the one of the buttons a /something is beeing sent. This will be catched by the [httphandler](/src/httpHandler.ino) script.

In the ```handleClientRequest``` function the Requests are beeing processed. It checks for the /something and calls a function or does something else.

The Variable Values are marked with %something% in the [website code](/src/webpageCode.h). In the ```sendHttpResponse``` function in [httpHandler](/src/httpHandler.ino) these values are replaced with the real up to Date values.

## Website
The Website can be edited with HTML and CSS. I tried to add JS but didnt get it to work properly.

Source Code for the Website -> [webpageCode.h](/src/webpageCode.h)
The Websites Code has to be saved in the Webpage Char.

### Example: Change a Button
To Change for Example the ```Hardwareversion``` Button, you would:
1. Change the ```/hardwareversion``` in [WebpageCode](/src/webpageCode.h) to someting else
2. In the [httpHandler](/src/httpHandler.ino) you have to change the Entry in ```handleClientRequest``` to the new name.
3. Then eather put your code direcly in the else if or call a function.

### Example: Change Header
1. Open [webpageCode.h](/src/webpageCode.h)
2. Navigate to the Body and find the H1 Section
3. Change AG05 Controll to something Differant
```HTML
    <h1>AG05 Control</h1>
```
