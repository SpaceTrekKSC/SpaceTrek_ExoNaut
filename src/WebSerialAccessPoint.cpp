#include "WebSerialAccessPoint.h"

static String page = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Web Serial Monitor</title>
      <meta charset="UTF-8">
      <style>
        body { font-family: monospace; background-color: #111; color: #0f0; margin: 0; padding: 20px; }
        #output { white-space: pre-wrap; word-wrap: break-word; height: 70vh; overflow-y: auto; border: 1px solid #0f0; padding: 10px; margin-bottom: 20px; }
        button { font-size: 16px; padding: 10px; margin-right: 10px; margin-top: 10px; background-color: #222; color: #0f0; border: 1px solid #0f0; cursor: pointer; }
        button:hover { background-color: #0f0; color: #111; }
      </style>
    </head>
    <body>
      <h1>Web Serial Monitor</h1>
      <div id="output"></div>
    
      <div>
        <button onclick="clearOutput()">Clear</button>
        <button onclick="toggleAutoscroll()">Autoscroll: <span id='scrollStatus'>ON</span></button>
        <button onclick="sendPlay()">Play</button>
        <button onclick="resetESP()">Reset</button>
      </div>
    
    <script>
      let output = document.getElementById('output');
      let scrollStatus = document.getElementById('scrollStatus');
      let ws;
      let autoScroll = true;
    
      function connectWebSocket() {
        ws = new WebSocket('ws://' + location.hostname + '/ws');
    
        ws.onopen = function(event) {
          console.log("[WebSocket] Connected");
        };
    
        ws.onmessage = function(event) {
          output.innerText += event.data + '\n';
          if (autoScroll) {
            output.scrollTop = output.scrollHeight;
          }
        };
    
        ws.onclose = function(event) {
          console.log("[WebSocket] Disconnected, trying to reconnect in 1s...");
          setTimeout(connectWebSocket, 1000);  // Try to reconnect after 1 second
        };
    
        ws.onerror = function(event) {
          console.error("[WebSocket] Error, closing socket.");
          ws.close();
        };
      }
    
      connectWebSocket();  // Start the first connection
    
      function clearOutput() {
        output.innerText = '';
      }
    
      function toggleAutoscroll() {
        autoScroll = !autoScroll;
        scrollStatus.innerText = autoScroll ? "ON" : "OFF";
      }
    
      function sendPlay() {
        if (ws.readyState === WebSocket.OPEN) {
          ws.send('PLAY');
        } else {
          console.warn("[WebSocket] Not connected, cannot send PLAY yet.");
        }
      }
    
      function resetESP() {
        if (confirm("Are you sure you want to reset the device?")) {
          if (ws.readyState === WebSocket.OPEN) {
            ws.send('RESET');
          } else {
            console.warn("[WebSocket] Not connected, cannot send RESET.");
          }
        }
      }
    </script>
    </body>
    </html>
    )rawliteral";

AsyncWebSocket ws("/ws");

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_DATA)
    {
        String incoming = "";
        for (size_t i = 0; i < len; i++)
        {
            incoming += (char)data[i];
        }

        incoming.trim(); // Clean input

        if (incoming == "PLAY")
        {
            WebSerial.setReadyToRun(true); // Just play
        }
        else if (incoming == "RESET")
        {
            ESP.restart(); // Soft reboot
        }
    }
}

void WebSerialAccessPointClass::begin(const char *ssid, const char *password)
{
    WiFi.softAP(ssid, password);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", page); });
    ws.onEvent(onWebSocketEvent);
    server.addHandler(&ws);
    server.begin();

    // Wait until Play is pressed
    waitingForPlay = true;
    readyToRun = false;

    Serial.println("[WebSerial] Waiting for Play...");

    while (!readyToRun)
    {
        ws.cleanupClients(); // Allow WebSocket traffic
        delay(100);
    }

    waitingForPlay = false;
    Serial.println("[WebSerial] Play received, continuing program...");
}

void WebSerialAccessPointClass::print(const String &message)
{
    ws.textAll(String(message));
}

void WebSerialAccessPointClass::print(int number)
{
    print(String(number));
}

void WebSerialAccessPointClass::print(float number)
{
    print(String(number));
}

void WebSerialAccessPointClass::print(double number)
{
    print(String(number));
}

void WebSerialAccessPointClass::print(unsigned int number)
{
    print(String(number));
}

void WebSerialAccessPointClass::println(const String &message)
{
    ws.textAll(String(message));
}

void WebSerialAccessPointClass::println(int number)
{
    println(String(number));
}

void WebSerialAccessPointClass::println(float number)
{
    println(String(number));
}

void WebSerialAccessPointClass::println(double number)
{
    println(String(number));
}

void WebSerialAccessPointClass::println(unsigned int number)
{
    println(String(number));
}

void WebSerialAccessPointClass::setReadyToRun(bool state)
{
    readyToRun = state;
}

WebSerialAccessPointClass WebSerial;
