/*
 * ExoNaut WiFi Remote Control
 * This is the title of our project! It's a remote control for the ExoNaut robot that uses WiFi.
 */

#include <Arduino.h>      // Includes the main Arduino library, which is the basic toolkit for our robot's brain.
#include <ExoNaut.h>      // Includes the special library for the ExoNaut robot, giving us commands for its motors and lights.
#include <WiFi.h>         // Includes the WiFi library, so our robot can create its own WiFi network.
#include <WebServer.h>    // Includes the Web Server library, which lets the robot host a webpage.

#define UP 1      // We're giving the number 1 a nickname: "UP". It's easier to remember.
#define DOWN 2    // The number 2 gets the nickname "DOWN".
#define LEFT 3    // The number 3 gets the nickname "LEFT".
#define RIGHT 4   // The number 4 gets the nickname "RIGHT".
#define STOP 0    // The number 0 gets the nickname "STOP".

const char* ssid = "ExoNaut";      // This will be the name of the WiFi network our robot creates.
const char* password = "123456789"; // This is the password for the robot's WiFi network.

WebServer server(80);     // We're creating a web server that will listen on port 80, the standard port for websites.
exonaut robot;            // We're creating our robot object and just calling it "robot".

uint8_t speed = 50;                     // This variable holds the robot's speed. It starts at 50.
unsigned long lastCommandTime = 0;      // This variable will remember the last time we sent the robot a command.
const unsigned long COMMAND_TIMEOUT = 1000; // This is a safety timer. If the robot doesn't get a command for 1000 milliseconds (1 second), it will stop.

// We'll build the webpage inside the handleRoot function below.

// This set of instructions (a function) tells the robot how to move.
void moveCar(int inputValue) {
  Serial.printf("Movement command: %d\n", inputValue); // Prints the command number to the computer screen so we can see what's happening.
  lastCommandTime = millis();                          // Resets our safety timer because we just received a command.

  // This `switch` statement is like a multi-choice question. It checks which direction we want to go.
  switch(inputValue) {
    case UP: // In case the command is UP (which is 1)...
      robot.set_motor_speed(speed, speed); // ...tell both motors to move forward at the current speed.
      Serial.println("Moving FORWARD");    // Print a message to the computer.
      break; // We're done with our choice, so we break out of the switch.

    case DOWN: // In case the command is DOWN (which is 2)...
      robot.set_motor_speed(-speed, -speed); // ...tell both motors to move backward (negative speed).
      Serial.println("Moving BACKWARD");     // Print a message.
      break; // Done.

    case LEFT: // In case the command is LEFT (which is 3)...
      robot.set_motor_speed(-speed/2, speed/2); // ...tell the left motor to go backward and the right motor to go forward to make a turn.
      Serial.println("Turning LEFT");           // Print a message.
      break; // Done.

    case RIGHT: // In case the command is RIGHT (which is 4)...
      robot.set_motor_speed(speed/2, -speed/2); // ...tell the right motor to go backward and the left motor to go forward.
      Serial.println("Turning RIGHT");          // Print a message.
      break; // Done.

    case STOP:   // In case the command is STOP (which is 0)...
    default:     // ...or if it's any other number we don't recognize...
      robot.stop_motor(0);      // ...tell the robot to stop all motors.
      Serial.println("STOPPED"); // Print a message.
      break; // Done.
  }
}

// This function runs when you open the main webpage on your phone or computer.
void handleRoot() {
  Serial.println("Serving enhanced control page..."); // Print a message that we're sending the webpage.

  // This giant block of text is the HTML and CSS code that creates the remote control webpage.
  String html = R"HTML(
<!DOCTYPE html> <html>
<head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1'> <title>ExoNaut Control</title> <style> body { /* Styles for the whole page. */
    font-family: Arial, sans-serif; /* Sets the font. */
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); /* A nice purple gradient background. */
    margin: 0; /* No extra space around the edges. */
    padding: 20px; /* Some space inside the body. */
    text-align: center; /* Center all the text. */
    color: white; /* Make the text white. */
  }
  
  h1 { /* Styles for the main title. */
    color: #fff; /* White color. */
    text-shadow: 2px 2px 4px rgba(0,0,0,0.5); /* A cool shadow effect. */
    margin-bottom: 30px; /* Space below the title. */
    font-size: 28px; /* Make the title font bigger. */
  }
  
  .control-section { /* Styles for the boxes that hold our controls. */
    background: rgba(255,255,255,0.1); /* A see-through white background. */
    border-radius: 15px; /* Rounded corners. */
    padding: 20px; /* Space inside the box. */
    margin: 20px auto; /* Space outside the box and center it. */
    max-width: 400px; /* The box won't get wider than 400 pixels. */
    backdrop-filter: blur(10px); /* A cool blurry glass effect for what's behind it. */
    box-shadow: 0 8px 32px rgba(0,0,0,0.3); /* A shadow to make it pop. */
  }
  
  .section-title { /* Styles for the titles of each control box. */
    font-size: 18px; /* The font size. */
    font-weight: bold; /* Make the text bold. */
    margin-bottom: 15px; /* Space below the title. */
    color: #fff; /* White text. */
  }
  
  /* --- Movement Controls --- */
  .movement-grid { /* Styles for the grid that holds the arrow buttons. */
    display: grid; /* Use a grid layout. */
    grid-template-columns: 1fr 1fr 1fr; /* Create three equal columns. */
    gap: 10px; /* Space between the grid items. */
    max-width: 300px; /* The grid won't get too wide. */
    margin: 0 auto; /* Center the grid. */
  }
  
  .move-btn { /* Styles for the movement buttons. */
    background: linear-gradient(145deg, #4a90e2, #357abd); /* A nice blue gradient. */
    border: none; /* No ugly border. */
    border-radius: 15px; /* Rounded corners. */
    color: white; /* The arrow symbols will be white. */
    font-size: 24px; /* Make the arrows bigger. */
    padding: 20px; /* Space inside the buttons to make them bigger. */
    cursor: pointer; /* The mouse cursor turns into a hand. */
    transition: all 0.2s; /* A smooth animation for any changes. */
    box-shadow: 0 4px 15px rgba(0,0,0,0.2); /* A shadow for the button. */
    user-select: none; /* Prevents text selection on the button. */
  }
  
  .move-btn:active { /* When a button is being pressed... */
    transform: scale(0.95); /* ...make it slightly smaller. */
    box-shadow: 0 2px 8px rgba(0,0,0,0.3); /* Change the shadow. */
  }
  
  .move-btn:hover { /* When you hover the mouse over a button... */
    background: linear-gradient(145deg, #5ba0f2, #4080cd); /* ...make it a lighter blue. */
  }
  
  /* --- LED Controls --- */
  .led-controls { /* Styles for the area with the LED color buttons. */
    display: flex; /* A flexible layout. */
    justify-content: space-around; /* Space the buttons out evenly. */
    gap: 10px; /* Add some space between them. */
    flex-wrap: wrap; /* Let the buttons wrap to the next line if there's not enough space. */
  }
  
  .led-btn { /* Styles for each LED color button. */
    border: none; /* No border. */
    border-radius: 10px; /* Rounded corners. */
    color: white; /* White text. */
    font-size: 14px; /* Font size for the text. */
    font-weight: bold; /* Bold text. */
    padding: 15px 20px; /* Space inside the buttons. */
    cursor: pointer; /* Hand cursor on hover. */
    transition: all 0.2s; /* Smooth animations. */
    box-shadow: 0 4px 15px rgba(0,0,0,0.2); /* A shadow. */
    min-width: 80px; /* Make sure the buttons aren't too narrow. */
  }
  
  .led-red { background: linear-gradient(145deg, #ff6b6b, #ee5a52); } /* Red button gradient. */
  .led-green { background: linear-gradient(145deg, #51cf66, #40c057); } /* Green button gradient. */
  .led-blue { background: linear-gradient(145deg, #339af0, #228be6); } /* Blue button gradient. */
  
  .led-btn:active { transform: scale(0.95); } /* Make the button smaller when pressed. */
  
  /* --- Speed Control --- */
  .speed-container { margin: 20px 0; } /* Space around the speed slider. */
  
  .speed-value { /* Styles for the text that shows the speed percentage. */
    font-size: 24px; /* Big font. */
    font-weight: bold; /* Bold font. */
    color: #fff; /* White color. */
    margin: 10px 0; /* Space above and below. */
    text-shadow: 1px 1px 2px rgba(0,0,0,0.5); /* A slight shadow on the text. */
  }
  
  .speed-slider { /* Styles for the speed slider bar. */
    width: 100%; /* Make the slider as wide as its container. */
    height: 8px; /* How thick the slider bar is. */
    border-radius: 5px; /* Rounded ends for the bar. */
    background: rgba(255,255,255,0.3); /* A see-through white color. */
    outline: none; /* No blue outline when clicked. */
    -webkit-appearance: none; /* Remove default styling on some browsers. */
  }
  
  .speed-slider::-webkit-slider-thumb { /* Styles for the little circle you drag on the slider (for Chrome/Safari). */
    -webkit-appearance: none; /* Remove default styling. */
    appearance: none; /* Remove default styling. */
    width: 25px; /* Width of the circle. */
    height: 25px; /* Height of the circle. */
    border-radius: 50%; /* Make it a perfect circle. */
    background: #fff; /* White color. */
    cursor: pointer; /* Hand cursor. */
    box-shadow: 0 2px 6px rgba(0,0,0,0.3); /* A shadow for the circle. */
  }
  
  .speed-slider::-moz-range-thumb { /* Styles for the slider circle (for Firefox). */
    width: 25px; /* Width of the circle. */
    height: 25px; /* Height of the circle. */
    border-radius: 50%; /* Make it a circle. */
    background: #fff; /* White color. */
    cursor: pointer; /* Hand cursor. */
    border: none; /* No border. */
    box-shadow: 0 2px 6px rgba(0,0,0,0.3); /* A shadow. */
  }
  
  .empty { visibility: hidden; } /* This is a trick to create empty spaces in our movement grid. */
</style>
</head>
<body> <h1>ExoNaut Robot Control</h1> <div class='control-section'>
  <div class='section-title'>Movement</div> <div class='movement-grid'> <div class='empty'></div> <button class='move-btn' onmousedown='moveRobot(1)' onmouseup='stopRobot()' ontouchstart='moveRobot(1)' ontouchend='stopRobot()'>
      &#8593; </button>
    <div class='empty'></div> <button class='move-btn' onmousedown='moveRobot(3)' onmouseup='stopRobot()' ontouchstart='moveRobot(3)' ontouchend='stopRobot()'>
      &#8592; </button>
    <button class='move-btn' onclick='stopRobot()'>
      &#9632; </button>
    <button class='move-btn' onmousedown='moveRobot(4)' onmouseup='stopRobot()' ontouchstart='moveRobot(4)' ontouchend='stopRobot()'>
      &#8594; </button>
    
    <div class='empty'></div> <button class='move-btn' onmousedown='moveRobot(2)' onmouseup='stopRobot()' ontouchstart='moveRobot(2)' ontouchend='stopRobot()'>
      &#8595; </button>
    <div class='empty'></div> </div>
</div>

<div class='control-section'>
  <div class='section-title'>LED Colors</div> <div class='led-controls'> <button class='led-btn led-red' onclick='setLEDColor("red")'>RED</button> <button class='led-btn led-green' onclick='setLEDColor("green")'>GREEN</button> <button class='led-btn led-blue' onclick='setLEDColor("blue")'>BLUE</button> </div>
</div>

<div class='control-section'>
  <div class='section-title'>Speed Control</div> <div class='speed-container'> <div class='speed-value'>Speed: <span id='speedValue'>50</span>%</div> <input type='range' min='10' max='100' value='50' class='speed-slider' id='speedSlider' oninput='updateSpeed(this.value)'>
  </div>
</div>

<script> // This is JavaScript code that runs in the browser to make the buttons work.
// This function sends a message to the robot to move in a certain direction.
function moveRobot(direction) {
  fetch('/move?dir=' + direction) // 'fetch' sends a web request to the robot at the address '/move?dir=...'
    .catch(error => console.log('Move error:', error)); // If something goes wrong, log an error.
}

// This function sends a message to the robot to stop.
function stopRobot() {
  fetch('/move?dir=0') // It tells the robot to move in direction 0, which we defined as STOP.
    .catch(error => console.log('Stop error:', error));
}

// This function sends a message to the robot to change the LED color.
function setLEDColor(color) {
  fetch('/led?color=' + color) // Sends a request like '/led?color=red'.
    .catch(error => console.log('LED error:', error));
}

// This function is called when you move the speed slider.
function updateSpeed(value) {
  document.getElementById('speedValue').innerHTML = value; // This updates the number on the screen to show the new speed.
  fetch('/speed?val=' + value) // This sends the new speed value to the robot.
    .catch(error => console.log('Speed error:', error));
}

// This code helps the buttons work better on touch screens.
document.addEventListener('touchstart', function(e) {
  if (e.target.classList.contains('move-btn')) { // If you touch a move button...
    e.preventDefault(); // ...prevent the phone from doing other things, like scrolling.
  }
}, {passive: false});

document.addEventListener('touchend', function(e) {
  if (e.target.classList.contains('move-btn')) { // If you lift your finger from a move button...
    e.preventDefault(); // ...prevent any default actions.
  }
}, {passive: false});
</script>

</body>
</html>
)HTML";
  
  server.send(200, "text/html", html); // Sends the whole HTML page to the browser. 200 means "OK".
  Serial.println("Enhanced control page sent successfully"); // Confirms that the page was sent.
}

// This function handles requests to the "/move" URL.
void handleMove() {
  Serial.println("Move command received"); // A message for us on the computer.
  if (server.hasArg("dir")) { // Checks if the request has a direction ("dir") parameter.
    int direction = server.arg("dir").toInt(); // Gets the direction value and turns it into a number.
    Serial.printf("Direction: %d\n", direction); // Prints the direction.
    moveCar(direction); // Calls our `moveCar` function to actually move the robot.
    server.send(200, "text/plain", "OK"); // Sends a simple "OK" message back to the webpage.
  } else { // If there was no direction parameter...
    Serial.println("No direction parameter"); // Print an error.
    server.send(400, "text/plain", "Bad Request - No direction"); // Send an error back to the browser. 400 means "Bad Request".
  }
}

// This function handles requests to the "/speed" URL.
void handleSpeed() {
  Serial.println("Speed command received"); // A message for us.
  if (server.hasArg("val")) { // Checks if the request has a speed value ("val").
    int newSpeed = server.arg("val").toInt(); // Gets the speed value and turns it into a number.
    if (newSpeed >= 10 && newSpeed <= 100) {  // Makes sure the speed is within a safe range (10 to 100).
      speed = newSpeed; // Updates our global speed variable.
      Serial.printf("Speed changed to: %d\n", speed); // Prints the new speed.
      server.send(200, "text/plain", "OK"); // Sends "OK" back to the browser.
    } else { // If the speed value is not in the range...
      Serial.println("Invalid speed value"); // Print an error.
      server.send(400, "text/plain", "Invalid Speed Range (10-100)"); // Send an error message back.
    }
  } else { // If there was no speed value parameter...
    Serial.println("No speed parameter"); // Print an error.
    server.send(400, "text/plain", "Bad Request - No speed"); // Send an error back to the browser.
  }
}

// This function handles requests to the "/led" URL.
void handleLED() {
  Serial.println("LED command received"); // A message for us.
  if (server.hasArg("color")) { // Checks if the request has a color parameter.
    String color = server.arg("color"); // Gets the color name (like "red").
    Serial.printf("LED Color: %s\n", color.c_str()); // Prints the color.
    
    if (color == "red") { // If the color is "red"...
      robot.setColorAll(255, 0, 0); // ...set the LEDs to full Red, no Green, no Blue.
      Serial.println("LEDs set to RED"); // Print a message.
    } else if (color == "green") { // If the color is "green"...
      robot.setColorAll(0, 255, 0); // ...set the LEDs to full Green.
      Serial.println("LEDs set to GREEN");
    } else if (color == "blue") { // If the color is "blue"...
      robot.setColorAll(0, 0, 255); // ...set the LEDs to full Blue.
      Serial.println("LEDs set to BLUE");
    } else { // For any other color...
      robot.setColorAll(0, 0, 0);   // ...just turn the LEDs off.
      Serial.println("LEDs turned OFF");
    }
    
    robot.show();   // This command tells the LEDs to light up with the new color we just set.
    server.send(200, "text/plain", "LED updated"); // Sends a confirmation message back.
  } else { // If there was no color parameter...
    Serial.println("No color parameter"); // Print an error.
    server.send(400, "text/plain", "Bad Request - No color"); // Send an error back to the browser.
  }
}

// This function runs if you try to go to a webpage that doesn't exist.
void handleNotFound() {
  String message = "File Not Found\n\n"; // Start building an error message.
  message += "URI: "; // Add the URL that was requested.
  message += server.uri();
  message += "\nMethod: "; // Add the method (usually GET).
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: "; // Add any arguments that were sent.
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++) { // List all the arguments one by one.
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  Serial.println("404 Error: " + message); // Print the detailed error message for us.
  server.send(404, "text/plain", message); // Send a "404 Not Found" error to the browser.
}

// The setup() function runs only once, when the robot first powers on.
void setup() {
  Serial.begin(115200); // Starts the communication with the computer so we can see print messages.
  Serial.println("Starting ExoNaut WiFi Control..."); // A startup message.
  
  // Initialize the robot.
  robot.begin(); // This wakes up the robot's hardware.
  Serial.println("Robot initialized"); // A confirmation message.
  
  // Start the WiFi Access Point.
  Serial.println("Setting up WiFi Access Point..."); // A status message.
  WiFi.softAP(ssid, password); // Turns the robot into a WiFi hotspot with our chosen name and password.
  IPAddress IP = WiFi.softAPIP(); // Gets the robot's IP address.
  Serial.print("AP IP address: "); // Prints a label.
  Serial.println(IP); // Prints the IP address you need to connect to (usually 192.168.4.1).
  
  // Set up the web server routes (the different pages).
  Serial.println("Setting up web server routes..."); // A status message.
  server.on("/", HTTP_GET, handleRoot); // When someone asks for the main page ("/"), run the handleRoot function.
  server.on("/move", HTTP_GET, handleMove); // When a "/move" request comes, run handleMove.
  server.on("/speed", HTTP_GET, handleSpeed); // For "/speed", run handleSpeed.
  server.on("/led", HTTP_GET, handleLED); // For "/led", run handleLED.
  
  // Add a simple test route to make sure the server is working.
  server.on("/test", HTTP_GET, []() { // For "/test"...
    server.send(200, "text/plain", "Test route working!"); // ...just send a simple message.
  });
  
  server.onNotFound(handleNotFound); // If a requested page doesn't match any above, run handleNotFound.
  
  // Start the server.
  server.begin(); // Now the server is officially running and listening for connections.
  Serial.println("HTTP server started successfully!"); // A success message.
  Serial.println("======================================"); // Some lines to make the instructions stand out.
  Serial.println("Connect to WiFi: 'ExoNaut'"); // Instructions for the user.
  Serial.println("Password: '123456789'");
  Serial.println("Then open browser to: http://192.168.4.1");
  Serial.println("======================================");
}

// The loop() function runs over and over again forever after setup() is done.
void loop() {
  server.handleClient(); // This constantly checks if a browser is trying to send a command.
  
  // This is a safety feature to stop the robot if it loses connection.
  if (millis() - lastCommandTime > COMMAND_TIMEOUT) { // Checks if more than 1 second has passed since the last command.
    static bool hasTimedOut = false; // A special variable to remember if we've already timed out.
    if (!hasTimedOut) { // If we haven't already sent the stop command for this timeout...
      moveCar(STOP); // ...stop the robot.
      hasTimedOut = true; // And remember that we've sent the stop command.
    }
    // This part resets the timeout flag once a new command arrives.
    if (millis() - lastCommandTime < COMMAND_TIMEOUT) { // If a new command has come in...
      hasTimedOut = false; // ...reset the flag so the safety stop can work again later.
    }
  }
  
  delay(10); // A very small pause to let the robot's brain rest and handle tasks smoothly.
}