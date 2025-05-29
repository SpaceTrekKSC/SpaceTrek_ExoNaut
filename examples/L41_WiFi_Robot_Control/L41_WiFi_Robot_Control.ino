/*
 * ExoNaut WiFi Remote Control
 */

#include <Arduino.h>
#include <ExoNaut.h>
#include <WiFi.h>
#include <WebServer.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define STOP 0

const char* ssid = "ExoNaut";
const char* password = "123456789";

WebServer server(80);
exonaut robot;

uint8_t speed = 50;
unsigned long lastCommandTime = 0;
const unsigned long COMMAND_TIMEOUT = 1000; // Stop robot if no command for 1 second

// Remove the large HTML string - we'll build it dynamically in handleRoot()

void moveCar(int inputValue) {
  Serial.printf("Movement command: %d\n", inputValue);
  lastCommandTime = millis();
  
  switch(inputValue) {
    case UP:
      robot.set_motor_speed(speed, speed);
      Serial.println("Moving FORWARD");
      break;
  
    case DOWN:
      robot.set_motor_speed(-speed, -speed);
      Serial.println("Moving BACKWARD");
      break;
  
    case LEFT:
      robot.set_motor_speed(-speed/2, speed/2);
      Serial.println("Turning LEFT");
      break;
  
    case RIGHT:
      robot.set_motor_speed(speed/2, -speed/2);
      Serial.println("Turning RIGHT");
      break;
 
    case STOP:
    default:
      robot.stop_motor(0);
      Serial.println("STOPPED");
      break;
  }
}

void handleRoot() {
  Serial.println("Serving enhanced control page...");
  
  String html = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1'>
<title>ExoNaut Control</title>
<style>
  body {
    font-family: Arial, sans-serif;
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    margin: 0;
    padding: 20px;
    text-align: center;
    color: white;
  }
  
  h1 {
    color: #fff;
    text-shadow: 2px 2px 4px rgba(0,0,0,0.5);
    margin-bottom: 30px;
    font-size: 28px;
  }
  
  .control-section {
    background: rgba(255,255,255,0.1);
    border-radius: 15px;
    padding: 20px;
    margin: 20px auto;
    max-width: 400px;
    backdrop-filter: blur(10px);
    box-shadow: 0 8px 32px rgba(0,0,0,0.3);
  }
  
  .section-title {
    font-size: 18px;
    font-weight: bold;
    margin-bottom: 15px;
    color: #fff;
  }
  
  /* Movement Controls */
  .movement-grid {
    display: grid;
    grid-template-columns: 1fr 1fr 1fr;
    gap: 10px;
    max-width: 300px;
    margin: 0 auto;
  }
  
  .move-btn {
    background: linear-gradient(145deg, #4a90e2, #357abd);
    border: none;
    border-radius: 15px;
    color: white;
    font-size: 24px;
    padding: 20px;
    cursor: pointer;
    transition: all 0.2s;
    box-shadow: 0 4px 15px rgba(0,0,0,0.2);
    user-select: none;
  }
  
  .move-btn:active {
    transform: scale(0.95);
    box-shadow: 0 2px 8px rgba(0,0,0,0.3);
  }
  
  .move-btn:hover {
    background: linear-gradient(145deg, #5ba0f2, #4080cd);
  }
  
  /* LED Controls */
  .led-controls {
    display: flex;
    justify-content: space-around;
    gap: 10px;
    flex-wrap: wrap;
  }
  
  .led-btn {
    border: none;
    border-radius: 10px;
    color: white;
    font-size: 14px;
    font-weight: bold;
    padding: 15px 20px;
    cursor: pointer;
    transition: all 0.2s;
    box-shadow: 0 4px 15px rgba(0,0,0,0.2);
    min-width: 80px;
  }
  
  .led-red {
    background: linear-gradient(145deg, #ff6b6b, #ee5a52);
  }
  
  .led-green {
    background: linear-gradient(145deg, #51cf66, #40c057);
  }
  
  .led-blue {
    background: linear-gradient(145deg, #339af0, #228be6);
  }
  
  .led-btn:active {
    transform: scale(0.95);
  }
  
  /* Speed Control */
  .speed-container {
    margin: 20px 0;
  }
  
  .speed-value {
    font-size: 24px;
    font-weight: bold;
    color: #fff;
    margin: 10px 0;
    text-shadow: 1px 1px 2px rgba(0,0,0,0.5);
  }
  
  .speed-slider {
    width: 100%;
    height: 8px;
    border-radius: 5px;
    background: rgba(255,255,255,0.3);
    outline: none;
    -webkit-appearance: none;
  }
  
  .speed-slider::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 25px;
    height: 25px;
    border-radius: 50%;
    background: #fff;
    cursor: pointer;
    box-shadow: 0 2px 6px rgba(0,0,0,0.3);
  }
  
  .speed-slider::-moz-range-thumb {
    width: 25px;
    height: 25px;
    border-radius: 50%;
    background: #fff;
    cursor: pointer;
    border: none;
    box-shadow: 0 2px 6px rgba(0,0,0,0.3);
  }
  
  /* Empty grid cells */
  .empty { visibility: hidden; }
</style>
</head>
<body>

<h1>ExoNaut Robot Control</h1>

<!-- Movement Controls -->
<div class='control-section'>
  <div class='section-title'>Movement</div>
  <div class='movement-grid'>
    <div class='empty'></div>
    <button class='move-btn' onmousedown='moveRobot(1)' onmouseup='stopRobot()' ontouchstart='moveRobot(1)' ontouchend='stopRobot()'>
      &#8593;
    </button>
    <div class='empty'></div>
    
    <button class='move-btn' onmousedown='moveRobot(3)' onmouseup='stopRobot()' ontouchstart='moveRobot(3)' ontouchend='stopRobot()'>
      &#8592;
    </button>
    <button class='move-btn' onclick='stopRobot()'>
      &#9632;
    </button>
    <button class='move-btn' onmousedown='moveRobot(4)' onmouseup='stopRobot()' ontouchstart='moveRobot(4)' ontouchend='stopRobot()'>
      &#8594;
    </button>
    
    <div class='empty'></div>
    <button class='move-btn' onmousedown='moveRobot(2)' onmouseup='stopRobot()' ontouchstart='moveRobot(2)' ontouchend='stopRobot()'>
      &#8595;
    </button>
    <div class='empty'></div>
  </div>
</div>

<!-- LED Controls -->
<div class='control-section'>
  <div class='section-title'>LED Colors</div>
  <div class='led-controls'>
    <button class='led-btn led-red' onclick='setLEDColor("red")'>RED</button>
    <button class='led-btn led-green' onclick='setLEDColor("green")'>GREEN</button>
    <button class='led-btn led-blue' onclick='setLEDColor("blue")'>BLUE</button>
  </div>
</div>

<!-- Speed Control -->
<div class='control-section'>
  <div class='section-title'>Speed Control</div>
  <div class='speed-container'>
    <div class='speed-value'>Speed: <span id='speedValue'>50</span>%</div>
    <input type='range' min='10' max='100' value='50' class='speed-slider' id='speedSlider' oninput='updateSpeed(this.value)'>
  </div>
</div>

<script>
function moveRobot(direction) {
  fetch('/move?dir=' + direction)
    .catch(error => console.log('Move error:', error));
}

function stopRobot() {
  fetch('/move?dir=0')
    .catch(error => console.log('Stop error:', error));
}

function setLEDColor(color) {
  fetch('/led?color=' + color)
    .catch(error => console.log('LED error:', error));
}

function updateSpeed(value) {
  document.getElementById('speedValue').innerHTML = value;
  fetch('/speed?val=' + value)
    .catch(error => console.log('Speed error:', error));
}

// Prevent default touch behaviors
document.addEventListener('touchstart', function(e) {
  if (e.target.classList.contains('move-btn')) {
    e.preventDefault();
  }
}, {passive: false});

document.addEventListener('touchend', function(e) {
  if (e.target.classList.contains('move-btn')) {
    e.preventDefault();
  }
}, {passive: false});
</script>

</body>
</html>
)HTML";
  
  server.send(200, "text/html", html);
  Serial.println("Enhanced control page sent successfully");
}

void handleMove() {
  Serial.println("Move command received");
  if (server.hasArg("dir")) {
    int direction = server.arg("dir").toInt();
    Serial.printf("Direction: %d\n", direction);
    moveCar(direction);
    server.send(200, "text/plain", "OK");
  } else {
    Serial.println("No direction parameter");
    server.send(400, "text/plain", "Bad Request - No direction");
  }
}

void handleSpeed() {
  Serial.println("Speed command received");
  if (server.hasArg("val")) {
    int newSpeed = server.arg("val").toInt();
    if (newSpeed >= 10 && newSpeed <= 100) {  // Changed range to match slider
      speed = newSpeed;
      Serial.printf("Speed changed to: %d\n", speed);
      server.send(200, "text/plain", "OK");
    } else {
      Serial.println("Invalid speed value");
      server.send(400, "text/plain", "Invalid Speed Range (10-100)");
    }
  } else {
    Serial.println("No speed parameter");
    server.send(400, "text/plain", "Bad Request - No speed");
  }
}

void handleLED() {
  Serial.println("LED command received");
  if (server.hasArg("color")) {
    String color = server.arg("color");
    Serial.printf("LED Color: %s\n", color.c_str());
    
    if (color == "red") {
      robot.setColorAll(255, 0, 0);
      Serial.println("LEDs set to RED");
    } else if (color == "green") {
      robot.setColorAll(0, 255, 0);
      Serial.println("LEDs set to GREEN");
    } else if (color == "blue") {
      robot.setColorAll(0, 0, 255);
      Serial.println("LEDs set to BLUE");
    } else {
      robot.setColorAll(0, 0, 0);  // Turn off for unknown colors
      Serial.println("LEDs turned OFF");
    }
    
    robot.show();  // Make sure to update the LEDs
    server.send(200, "text/plain", "LED updated");
  } else {
    Serial.println("No color parameter");
    server.send(400, "text/plain", "Bad Request - No color");
  }
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  Serial.println("404 Error: " + message);
  server.send(404, "text/plain", message);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting ExoNaut WiFi Control...");
  
  // Initialize robot
  robot.begin();
  Serial.println("Robot initialized");
  
  // Start WiFi Access Point
  Serial.println("Setting up WiFi Access Point...");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  // Set up web server routes
  Serial.println("Setting up web server routes...");
  server.on("/", HTTP_GET, handleRoot);
  server.on("/move", HTTP_GET, handleMove);
  server.on("/speed", HTTP_GET, handleSpeed);
  server.on("/led", HTTP_GET, handleLED);
  
  // Add a simple test route
  server.on("/test", HTTP_GET, []() {
    server.send(200, "text/plain", "Test route working!");
  });
  
  server.onNotFound(handleNotFound);
  
  // Start the server
  server.begin();
  Serial.println("HTTP server started successfully!");
  Serial.println("======================================");
  Serial.println("Connect to WiFi: 'ExoNaut'");
  Serial.println("Password: '123456789'");
  Serial.println("Then open browser to: http://192.168.4.1");
  Serial.println("======================================");
}

void loop() {
  server.handleClient();
  
  // Safety feature: stop robot if no commands received for a while
  if (millis() - lastCommandTime > COMMAND_TIMEOUT) {
    static bool hasTimedOut = false;
    if (!hasTimedOut) {
      moveCar(STOP);
      hasTimedOut = true;
    }
    // Reset timeout flag when new command comes in
    if (millis() - lastCommandTime < COMMAND_TIMEOUT) {
      hasTimedOut = false;
    }
  }
  
  delay(10); // Small delay to prevent watchdog issues
}