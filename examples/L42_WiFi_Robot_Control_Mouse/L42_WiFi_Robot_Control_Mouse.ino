#include <Arduino.h>
#include <ExoNaut.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <iostream>
#include <sstream>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define STOP 0

// #define RIGHT_MOTOR 0
// #define LEFT_MOTOR 1

// #define FORWARD 1
// #define BACKWARD -1

const char* ssid     = "ExoNaut";
const char* password = "123456789";

AsyncWebServer server(80);
AsyncWebSocket wsCarInput("/CarInput");

exonaut robot;                                  //create the robot object

uint8_t speed = 50;

const char* htmlHomePage PROGMEM = R"HTMLHOMEPAGE(
<!DOCTYPE html>
<html>
  <head>
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
    <style>
    .arrows {
      font-size:40px;
      color:red;
    }
    td.button {
      background-color:black;
      border-radius:25%;
      box-shadow: 5px 5px #888888;
    }
    td.button:active {
      transform: translate(5px,5px);
      box-shadow: none; 
    }

    .noselect {
      -webkit-touch-callout: none; /* iOS Safari */
        -webkit-user-select: none; /* Safari */
         -khtml-user-select: none; /* Konqueror HTML */
           -moz-user-select: none; /* Firefox */
            -ms-user-select: none; /* Internet Explorer/Edge */
                user-select: none; /* Non-prefixed version, currently
                                      supported by Chrome and Opera */
    }

    .slidecontainer {
      width: 100%;
    }

    .slider {
      -webkit-appearance: none;
      width: 100%;
      height: 20px;
      border-radius: 5px;
      background: #d3d3d3;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
    }

    .slider:hover {
      opacity: 1;
    }
  
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 40px;
      height: 40px;
      border-radius: 50%;
      background: red;
      cursor: pointer;
    }

    .slider::-moz-range-thumb {
      width: 40px;
      height: 40px;
      border-radius: 50%;
      background: red;
      cursor: pointer;
    }

    </style>
  
  </head>
  <body class="noselect" align="center" style="background-color:white">
     
    <h1 style="color: teal;text-align:center;">Hash Include Electronics</h1>
    <h2 style="color: teal;text-align:center;">WiFi Tank Control</h2>
    
    <table id="mainTable" style="width:400px;margin:auto;table-layout:fixed" CELLSPACING=10>
      <tr>
        <td></td>
        <td class="button" onmousedown='sendButtonInput("MoveCar","1")' onmouseup='sendButtonInput("MoveCar","0")'><span class="arrows" >&#8679;</span></td>
        <td></td>
      </tr>
      <tr>
        <td class="button" onmousedown='sendButtonInput("MoveCar","3")' onmouseup='sendButtonInput("MoveCar","0")'><span class="arrows" >&#8678;</span></td>
        <td class="button"></td>    
        <td class="button" onmousedown='sendButtonInput("MoveCar","4")' onmouseup='sendButtonInput("MoveCar","0")'><span class="arrows" >&#8680;</span></td>
      </tr>
      <tr>
        <td></td>
        <td class="button" onmousedown='sendButtonInput("MoveCar","2")' onmouseup='sendButtonInput("MoveCar","0")'><span class="arrows" >&#8681;</span></td>
        <td></td>
      </tr>
      <tr/><tr/>
      <tr/><tr/>
      <tr/><tr/>
      <tr>
        <td style="text-align:left;font-size:25px"><b>Speed:</b></td>
        <td colspan=2>
         <div class="slidecontainer">
            <input type="range" min="0" max="120" value="60" class="slider" id="Speed" oninput='sendButtonInput("Speed",value)'>
          </div>
        </td>
      </tr>       
    </table>
  
    <script>
      var webSocketCarInputUrl = "ws:\/\/" + window.location.hostname + "/CarInput";      
      var websocketCarInput;
      
      function initCarInputWebSocket() 
      {
        websocketCarInput = new WebSocket(webSocketCarInputUrl);
        websocketCarInput.onopen    = function(event)
        {
          var speedButton = document.getElementById("Speed");
          sendButtonInput("Speed", speedButton.value);
        };
        websocketCarInput.onclose   = function(event){setTimeout(initCarInputWebSocket, 2000);};
        websocketCarInput.onmessage = function(event){};        
      }
      
      function sendButtonInput(key, value) 
      {
        var data = key + "," + value;
        websocketCarInput.send(data);
      }
    
      window.onload = initCarInputWebSocket;
      document.getElementById("mainTable").addEventListener("touchend", function(event){
        event.preventDefault()
      });      
    </script>
  </body>    
</html>
)HTMLHOMEPAGE";

void moveCar(int inputValue){
  Serial.printf("Got value as %d\n", inputValue);  
  switch(inputValue){
    case UP:
      robot.setColorAll(0, 0, 150);               //Turn LEDs BLUE for UP
      robot.set_motor_speed(speed, speed);
      break;
  
    case DOWN:
      robot.setColorAll(0, 150, 0);               //Turn LEDs GREEN for DOWN
      robot.set_motor_speed(-speed, -speed);
      break;
  
    case LEFT:
      robot.setColorAll(150, 100, 0);             //Turn LEDs ORANGE for LEFT
      robot.set_motor_speed(-1*(speed/2), (speed/2));
      break;
  
    case RIGHT:
      robot.setColorAll(150, 0, 100);             //Turn LEDs PURPLE for RIGHT
      robot.set_motor_speed((speed/2), -1*(speed/2));
      break;
 
    case STOP:
      robot.setColorAll(150, 0, 0);             //Turn LEDs ORANGE for LEFT
      robot.stop_motor(0);
      break;
  
    default:
      robot.setColorAll(0, 0, 0);               //Turn LEDs OFF for Default
      robot.stop_motor(0);
      break;
  }
}

void handleRoot(AsyncWebServerRequest *request){
  request->send_P(200, "text/html", htmlHomePage);
}

void handleNotFound(AsyncWebServerRequest *request){
    request->send(404, "text/plain", "File Not Found");
}

void onCarInputWebSocketEvent(AsyncWebSocket *server, 
                      AsyncWebSocketClient *client, 
                      AwsEventType type,
                      void *arg, 
                      uint8_t *data, 
                      size_t len) 
{                      
  switch (type){
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      moveCar(STOP);
      break;
    case WS_EVT_DATA:
      AwsFrameInfo *info;
      info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT){
        std::string myData = "";
        myData.assign((char *)data, len);
        std::istringstream ss(myData);
        std::string key, value;
        std::getline(ss, key, ',');
        std::getline(ss, value, ',');
        Serial.printf("Key [%s] Value[%s]\n", key.c_str(), value.c_str()); 
        int valueInt = atoi(value.c_str());     
        if (key == "MoveCar"){
          moveCar(valueInt);        
        }
        else if (key == "Speed"){
          speed = valueInt;
        }
      }
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
    default:
      break;  
  }
}

void setup(void){
  //setUpPinModes();
  Serial.begin(115200);
  robot.begin();                                      //start the robot object

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);
      
  wsCarInput.onEvent(onCarInputWebSocketEvent);
  server.addHandler(&wsCarInput);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(){
  wsCarInput.cleanupClients(); 
}
