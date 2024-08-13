// Load Wi-Fi library
#include <WiFi.h>
#include <ExoNaut.h>

//Once connected to your robot via WiFi the address of your robot will be 192.168.4.1

// Replace with unique SSID and password so others can't access your robot
const char* ssid     = "SpaceTrek-ExoNaut";
const char* password = "123456789";

WiFiServer server(80);                          // Set web server port number to 80

exonaut robot;                                  //create the robot object

String header;                                  //variable to store the HTTP request
String ledState = "off";                        //variable to store the current LED output state

void setup(){                                   //setup() runs once at the beginning of the program
  Serial.begin(115200);                           //start the serial connection to the PC
  robot.begin();                                //start the robot object
  delay(2000);                                  //wait for 2 seconds

  Serial.println("Setting up the AP…");         //send text to the PC so the user knows what is happening
  WiFi.softAP(ssid, password);                  //remove the password parameter, if you want the Access Point to be open

  IPAddress IP = WiFi.softAPIP();               //get the IP address of the robot and store it in IP
  Serial.print("ExoNaut's IP address: ");       //output text to the PC so user knows the robots IP address
  Serial.println(IP);                           //output the IP
  server.begin();                               //start the web server
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if(client){                               // If a new client connects,
    // Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while(client.connected()){              // loop while the client's connected
      if(client.available()){               // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if(c == '\n') {                     // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if(currentLine.length() == 0){
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the LEDs on and off
            if(header.indexOf("GET /LEDs/on") >= 0){
              ledState = "on";
              robot.setColorAll(0, 0, 150);
            }
            else if(header.indexOf("GET /LEDs/off") >= 0){
              ledState = "off";
              robot.setColorAll(0, 0, 0);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // CSS to style the on/off buttons 
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #0000FF; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1>ExoNaut Web Server</h1>");              // Web Page Heading
            client.println("<p>BLUE LED - State " + ledState + "</p>");       // Display current state, and ON/OFF buttons for LEDs 
            
            if(ledState=="off"){                                              // If the ledState is off, it displays the ON button
              client.println("<p><a href=\"/LEDs/on\"><button class=\"button\">ON</button></a></p>");
            }else{
              client.println("<p><a href=\"/LEDs/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 

            client.println("</body></html>");
            client.println();               // The HTTP response ends with another blank line
            // Break out of the while loop
            break;
          }else{ 
            currentLine = "";               // if you got a newline, then clear currentLine
          }
        } else if(c != '\r'){               // if you got anything else but a carriage return character,
          currentLine += c;                 // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    //client.stop();
    // Serial.println("Client disconnected.");
    // Serial.println("");
  }
}