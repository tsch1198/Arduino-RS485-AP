#ifndef WEBPAGECODE_H
#define WEBPAGECODE_H

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <title>Motor Control</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
      body { font-family: Arial, sans-serif; margin: 0; padding: 0; text-align: center; background-color: #2e3440; color: #d8dee9; }
      .container { padding: 20px; max-width: 600px; margin: auto; background-color: #4c566a; box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1); border-radius: 8px; }
      h1 { color: #88c0d0; }
      .btn { display: inline-block; margin: 10px 5px; padding: 10px 20px; font-size: 16px; color: #2e3440; background-color: #81a1c1; border: none; border-radius: 5px; text-decoration: none; }
      .btn:hover { background-color: #8fbcbb; }
      .flex-container { display: flex; flex-direction: column; gap: 10px;}
      .flex-row { display: flex; flex-direction: row; gap: 5px; align-items: center; justify-content: center; }
      .footer { position: fixed; left: 0; bottom: 0; width: 100%; background-color: #bf616a; color: white; text-align: center; }
      input[type=range], input[type=number], input[type=text] { width: calc(100% - 110px); padding: 10px; margin: 10px 0; border-radius: 5px; border: 1px solid #88c0d0; background-color: #4c566a; color: #d8dee9; }
      input[type=submit] { width: 100px; padding: 10px; font-size: 16px; color: #2e3440; background-color: #81a1c1; border: none; border-radius: 5px; }
      input[type=submit]:hover { background-color: #8fbcbb; }
  </style>
</head>
<body>
  <div class="container">
    <h1>AG05 Control</h1>
    <div class="flex-container">
      <form action="/motorposition" method="GET">
        Set Position exmpl: -0010000, RPM exmpl: -0000050: <input type="number" name="motorposition" value="-0010000">
        <input type="submit" value="Submit">
      </form>
      <form action="/motorInformation" method="GET">
        Send Anything (String, send): <input type="text" name="motorInformation">
        <input type="submit" value="Submit">
      </form>
    </div>
    <div class="flex-container">
      <!-- <div class="flex-row">
        <a href="/RT" class="btn">Toggle Red LED</a>
        <a href="/BT" class="btn">Toggle Blue LED</a>
       </div> -->
      <form action="/" method="GET"> <!-- Form for refreshing the page -->
        <input type="submit" value="Refresh" class="btn">
      </form>
    </div>
    <!-- <div class="flex-container"> 
      <p>Blue LED is currently: <span id="ledState">%BlueLedState%</span></p>
    </div> -->
    <div class="flex-container">
      <p>Current Motor Mode: <span id="motormode">%motormode%</span></p>
    </div>
    <div class="flex-container">
      <div class="flex-row">
        <a href="/hardwareversion" class="btn">a0: Hardwareversion</a>
        <a href="/productiondate" class="btn">a6: production date</a>
        <a href="/currentsetpoint" class="btn">e0: Current setpoint</a>
      </div>
      <div class="flex-row">
        <a href="/stopmotor" class="btn">stopmotor (o)</a>
        <a href="/resumemotor" class="btn">resume / start motor (m)</a>
        <a href="/currentposition" class="btn">currentposition (z)</a>
      </div>
      <div class="flex-row">
        <a href="/clearlist" class="btn">Clear List</a>
        <a href="/setmodepos" class="btn">Set Mode Pos (x0)</a>
        <a href="/setmoderpm" class="btn">Set Mode RPM (x1)</a>
      </div>
    </div>
    <div class="flex-container">
      <div class="flex-row">
        <div>
          <h3>Request (1)</h3>
          <ul>
            %List1Items%
          </ul>
        </div>
        <div>
          <h3>Response (2)</h3>
          <ul>
            %List2Items%
          </ul>
        </div>
      </div>
    </div>
  </div>
  <div class="footer">
    <p>Made By: https://github.com/tsch1198</p>
  </div>
</body>
</html>
)rawliteral";

#endif
