/*
 * L35_MP3_Playback.ino
 *
 * This is a kid-friendly, interactive sketch to control the ExoNaut MP3 player.
 * It provides a simple, easy-to-understand interface for controlling the
 * MP3 module via the Arduino Serial Monitor. It is heavily commented to serve
 * as an educational tool for beginners.
 *
 * SD Card Setup (Very Important!):
 * 1. Format your SD card as FAT16 or FAT32.
 * 2. Create a folder named exactly "mp3" (all lowercase) on the card.
 * 3. Place your MP3 files inside the "mp3" folder.
 * 4. Rename your files to be three digits, like "001.mp3", "002.mp3", etc.
 *
 * Controls:
 * - Serial commands to control playback:
 * p = play
 * s = stop (pause)
 * n = next track
 * b = back (previous track)
 * + = volume up
 * - = volume down
 * A number (e.g., 5) = go to that track number
 *
 * Author: Space Trek Education
 * Email:  info@spacetrek.com
 * Date:   July 3, 2025
 *
 * Notes:
 * - Requires the ExoNaut_MP3 library with the integrated goToTrack() function.
 * - Set the Serial Monitor baud rate to 115200.
 */

// First, we need to tell our robot what tools we want to use.
// #include is like telling the robot: "Hey, go get your MP3 player tool."
#include <ExoNaut.h>
#include <ExoNaut_MP3.h>

// Now, let's give our MP3 player a name. We'll call it "music".
// This creates our very own remote control object named "music".
// Whenever we want to do something, we'll use this name.
ExoNaut_MP3 music;


// The setup() function runs only ONCE when your robot first turns on.
// It's for getting everything ready to go.
void setup() {

  // Start the Serial Monitor. This is the screen on your computer where
  // you can type commands and see messages from the robot.
  // 115200 is the speed of the connection, like how fast you talk.
  Serial.begin(115200);

  // We'll wait for a second and a half to make sure everything is ready.
  delay(1500);

  // Let's get our music player ready!
  // This line tells the MP3 player to turn on and get ready for commands.
  music.begin();

  // Now, let's print some instructions to the screen so you know what to do!
  Serial.println("Hello! I'm your robot's DJ booth!");
  Serial.println("Type a command and press Enter to control the music.");
  Serial.println("===================================================");
  Serial.println("COMMANDS:");
  Serial.println("  p  -> Play the music");
  Serial.println("  s  -> Stop (pause) the music");
  Serial.println("  n  -> Go to the Next song");
  Serial.println("  b  -> Go Back to the last song");
  Serial.println("  +  -> Turn the volume UP");
  Serial.println("  -  -> Turn the volume DOWN");
  Serial.println("  5  -> To jump to song number 5 (or any number!)");
  Serial.println("===================================================");
  Serial.println("Ready for your first command!");

} // The setup() function is now finished.


// The loop() function runs over and over and over again, forever!
// It's always waiting for you to give it a new command.
void loop() {

  // First, let's check if you've typed anything into the Serial Monitor.
  if (Serial.available() > 0) {

    // Hooray! You typed something. Let's read it.
    // This line reads everything you typed until you pressed the Enter key.
    String command = Serial.readStringUntil('\n');

    // Sometimes there are extra spaces. Let's get rid of them.
    command.trim();

    Serial.print("You typed: '");
    Serial.print(command);
    Serial.println("'");


    // Now, let's figure out what command you gave.
    // We'll use if-statements to check.

    if (command == "p") {
      Serial.println("Okay, playing the music!");
      music.play(); // This is the command to tell the library to play.

    } else if (command == "s") {
      Serial.println("Okay, pausing the music.");
      music.pause(); // This tells the library to pause.

    } else if (command == "n") {
      Serial.println("On to the next song!");
      music.next(); // This tells the library to go to the next track.

    } else if (command == "b") {
      Serial.println("Going back to the last song!");
      music.previous(); // This tells the library to go to the previous track.

    } else if (command == "+") {
      Serial.println("Volume UP!");
      music.volumeUp(); // This tells the library to make the music louder.

    } else if (command == "-") {
      Serial.println("Volume DOWN!");
      music.volumeDown(); // This tells the library to make the music quieter.

    } else {
      // If the command wasn't one of the letters above,
      // maybe it was a number for a song track!

      // This line converts the text you typed (like "5") into a real number.
      long trackNumber = command.toInt();

      // We only want to do this if it's a valid number.
      if (trackNumber > 0) {
        Serial.print("Okay, jumping to song number ");
        Serial.println(trackNumber);
        music.goToTrack(trackNumber); // This is our special library command to find a track!
      } else {
        Serial.println("Hmm, I don't recognize that command. Try again!");
      }
    }
    Serial.println("------------------------------------");
  } // End of the if-statement that checks for a command.

} // The loop() function starts over again, waiting for your next command.
