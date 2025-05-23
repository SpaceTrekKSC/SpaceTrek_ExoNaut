#include <WiFi.h>
#include <WebServer.h>
#include "ExoNaut.h"

// Access Point settings
const char* ap_ssid = "ExoNaut-Robot";  // Name of the robot's WiFi network
const char* ap_password = "exonaut123";  // Password for the robot's WiFi (min 8 chars)
IPAddress local_ip(192, 168, 4, 1);     // Default AP IP address
IPAddress gateway(192, 168, 4, 1);     
IPAddress subnet(255, 255, 255, 0);

// Web server on port 80
WebServer server(80);

// Create ExoNaut instance
exonaut bot;

// Motor control variables
float motorSpeed = 50.0; // Default speed (out of 100)

// HTML content for the web interface with controls
const char* htmlContent = 
"<!DOCTYPE html>"
"<html>"
"<head>"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no\">"
"    <title>ExoNaut Control Panel</title>"
"    <style>"
"        body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 20px; }"
"        h1 { color: #333; }"
"        .controls { max-width: 500px; margin: 0 auto; padding: 20px; border: 1px solid #ddd; border-radius: 10px; }"
"        .button-row { display: flex; justify-content: center; margin: 10px 0; }"
"        button { background-color: #4CAF50; border: none; color: white; padding: 15px 20px; "
"                text-align: center; text-decoration: none; display: inline-block; "
"                font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 5px; "
"                user-select: none; -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none; }"
"        button:hover { background-color: #45a049; }"
"        .stop { background-color: #f44336; }"
"        .stop:hover { background-color: #d32f2f; }"
"        .forward { background-color: #2196F3; }"
"        .backward { background-color: #2196F3; }"
"        .left { background-color: #ff9800; }"
"        .right { background-color: #ff9800; }"
"        .speed-control { margin: 20px 0; }"
"        .slider { width: 80%; height: 25px; }"
"        .status { margin-top: 20px; padding: 10px; background-color: #f1f1f1; border-radius: 5px; }"
"    </style>"
"</head>"
"<body>"
"    <h1>ExoNaut Robot Control</h1>"
"    <div class=\"controls\">"
"        <div class=\"button-row\">"
"            <button class=\"forward\" onpointerdown=\"move('forward')\" onpointerup=\"move('stop')\" onpointerout=\"move('stop')\">Forward</button>"
"        </div>"
"        <div class=\"button-row\">"
"            <button class=\"left\" onpointerdown=\"move('left')\" onpointerup=\"move('stop')\" onpointerout=\"move('stop')\">Left</button>"
"            <button class=\"right\" onpointerdown=\"move('right')\" onpointerup=\"move('stop')\" onpointerout=\"move('stop')\">Right</button>"
"        </div>"
"        <div class=\"button-row\">"
"            <button class=\"backward\" onpointerdown=\"move('backward')\" onpointerup=\"move('stop')\" onpointerout=\"move('stop')\">Backward</button>"
"        </div>"
"        "
"        <div class=\"speed-control\">"
"            <h3>Speed Control: <span id=\"speedValue\">50</span>%</h3>"
"            <input type=\"range\" min=\"0\" max=\"100\" value=\"50\" class=\"slider\" id=\"speedSlider\" oninput=\"updateSpeed(this.value)\">"
"        </div>"
"        "
"        <div class=\"led-control\">"
"            <h3>LED Control</h3>"
"            <button onclick=\"controlLED('red')\">Red</button>"
"            <button onclick=\"controlLED('green')\">Green</button>"
"            <button onclick=\"controlLED('blue')\">Blue</button>"
"            <button onclick=\"controlLED('off')\">Off</button>"
"        </div>"
"        "
"        <div class=\"status\" id=\"statusBox\">"
"            Status: Ready"
"        </div>"
"    </div>"
"    <script>"
"        function move(direction) {"
"            fetch('/move?dir=' + direction)"
"                .then(response => response.text())"
"                .then(data => {"
"                    document.getElementById('statusBox').innerHTML = 'Status: ' + data;"
"                });"
"        }"
"        "
"        function updateSpeed(value) {"
"            document.getElementById('speedValue').innerHTML = value;"
"            fetch('/speed?value=' + value)"
"                .then(response => response.text())"
"                .then(data => {"
"                    document.getElementById('statusBox').innerHTML = 'Status: ' + data;"
"                });"
"        }"
"        "
"        function controlLED(color) {"
"            fetch('/led?color=' + color)"
"                .then(response => response.text())"
"                .then(data => {"
"                    document.getElementById('statusBox').innerHTML = 'Status: ' + data;"
"                });"
"        }"
"    </script>"
"</body>"
"</html>";

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    
    // Initialize ExoNaut
    bot.begin();
    
    // Set up WiFi Access Point
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(ap_ssid, ap_password);
    
    Serial.println("WiFi Access Point Mode Initialized");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("SSID: ");
    Serial.println(ap_ssid);
    Serial.print("Password: ");
    Serial.println(ap_password);
    
    // Setup server routes
    server.on("/", handleRoot);
    server.on("/move", handleMove);
    server.on("/speed", handleSpeed);
    server.on("/led", handleLED);
    
    // Start server
    server.begin();
    Serial.println("HTTP server started");
    
    // Initialize with LEDs set to blue to indicate AP mode
    bot.setColorAll(0, 0, 255);
    bot.show();
    
    // Blink LED pattern to indicate ready status
    for (int i = 0; i < 3; i++) {
        bot.setColorAll(0, 0, 0);
        bot.show();
        delay(200);
        bot.setColorAll(0, 0, 255);
        bot.show();
        delay(200);
    }
}

void loop() {
    server.handleClient();
}

// Handler for root path - serves the HTML interface
void handleRoot() {
    server.send(200, "text/html", htmlContent);
}

// Handler for movement commands
void handleMove() {
    String direction = server.arg("dir");
    String status = "Moving " + direction;
    
    if (direction == "forward") {
        bot.set_motor_speed(motorSpeed, motorSpeed);
    } 
    else if (direction == "backward") {
        bot.set_motor_speed(-motorSpeed, -motorSpeed);
    } 
    else if (direction == "left") {
        bot.set_motor_speed(-motorSpeed, motorSpeed);
    } 
    else if (direction == "right") {
        bot.set_motor_speed(motorSpeed, -motorSpeed);
    } 
    else if (direction == "stop") {
        bot.set_motor_speed(0, 0);
        status = "Stopped";
    }
    
    server.send(200, "text/plain", status);
}

// Handler for speed adjustment
void handleSpeed() {
    String value = server.arg("value");
    motorSpeed = value.toFloat();
    String status = "Speed set to " + value + "%";
    
    server.send(200, "text/plain", status);
}

// Handler for LED control
void handleLED() {
    String color = server.arg("color");
    String status = "LED set to " + color;
    
    if (color == "red") {
        bot.setColorAll(255, 0, 0);
    } 
    else if (color == "green") {
        bot.setColorAll(0, 255, 0);
    } 
    else if (color == "blue") {
        bot.setColorAll(0, 0, 255);
    } 
    else if (color == "off") {
        bot.setColorAll(0, 0, 0);
        status = "LED turned off";
    }
    
    bot.show();
    server.send(200, "text/plain", status);
}
