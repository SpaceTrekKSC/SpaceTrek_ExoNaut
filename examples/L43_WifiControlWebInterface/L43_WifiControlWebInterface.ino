/*
 * L43_WiFiControlWebInterface.ino
 *
 * This example creates a web-based control interface for the ExoNaut robot
 * using WiFi connectivity. Users can control the robot remotely through a
 * browser by connecting to its IP address.
 *
 * Features:
 * - Forward/backward/left/right movement controls
 * - Speed adjustment slider
 * - LED color control
 * - Status feedback
 *
 * Requirements:
 * - WiFi network connection
 * - Web browser to access the control interface
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: March 30, 2025
 *
 * Commands:
 * exonaut bot;                          //This command creates the main robot instance
 *                                       //This is the object that handles motors and core functions
 *
 * WebServer server(80);                 //This command creates a web server on port 80
 *                                       //The server handles incoming HTTP requests
 *
 * WiFi.begin(ssid, password);           //This command connects to a WiFi network
 *                                       //Parameters: network name, network password
 *
 * server.on(path, handler);             //This command sets up routes for the web server
 *                                       //Parameters: URL path, function to handle the request
 *
 * server.begin();                       //This command starts the web server
 *                                       //Must be called after routes are set up
 *
 * server.handleClient();                //This command processes incoming client requests
 *                                       //Should be called regularly in the loop function
 *
 * server.arg(name);                     //This command retrieves an argument from an HTTP request
 *                                       //Parameter: name of the argument to retrieve
 *
 * server.send(code, type, content);     //This command sends a response to the client
 *                                       //Parameters: HTTP status code, content type, content
 *
 * bot.set_motor_speed(left, right);     //This command sets the motor speeds
 *                                       //Parameters: left motor speed, right motor speed (-100 to 100)
 *
 * bot.setColorAll(r, g, b);             //This command sets all LEDs to the same color
 *                                       //Parameters: red, green, blue values (0-255)
 *
 * bot.show();                           //This command updates the physical LED display
 */

#include <WiFi.h>
#include <WebServer.h>
#include "ExoNaut.h"

// WiFi credentials
const char* ssid = "WiFi-SSID";
const char* password = "Password";

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
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
"    <title>ExoNaut Control Panel</title>"
"    <style>"
"        body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 20px; }"
"        h1 { color: #333; }"
"        .controls { max-width: 500px; margin: 0 auto; padding: 20px; border: 1px solid #ddd; border-radius: 10px; }"
"        .button-row { display: flex; justify-content: center; margin: 10px 0; }"
"        button { background-color: #4CAF50; border: none; color: white; padding: 15px 20px; "
"                text-align: center; text-decoration: none; display: inline-block; "
"                font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 5px; }"
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
"            <button class=\"forward\" onclick=\"move('forward')\">Forward</button>"
"        </div>"
"        <div class=\"button-row\">"
"            <button class=\"left\" onclick=\"move('left')\">Left</button>"
"            <button class=\"stop\" onclick=\"move('stop')\">STOP</button>"
"            <button class=\"right\" onclick=\"move('right')\">Right</button>"
"        </div>"
"        <div class=\"button-row\">"
"            <button class=\"backward\" onclick=\"move('backward')\">Backward</button>"
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
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("Connected to WiFi! IP Address: ");
    Serial.println(WiFi.localIP());
    
    // Setup server routes
    server.on("/", handleRoot);
    server.on("/move", handleMove);
    server.on("/speed", handleSpeed);
    server.on("/led", handleLED);
    
    // Start server
    server.begin();
    Serial.println("HTTP server started");
    
    // Initialize with LEDs set to green
    bot.setColorAll(0, 50, 0);
    bot.show();
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
