/*
 * L62_AICamLineFollower.ino
 *
 * This sketch demonstrates how to easily use the AI camera line following 
 * functionality on the Space Trek ExoNaut Robot using the simplified functions.
 * 
 * The program automatically detects lines with the camera and controls the robot's
 * movement to follow the line, including slowing down for sharp turns and 
 * pivoting to recover if the line is lost.
 *
 * The camera module is an i2c device. It must be plugged
 * into port 3, 4, 5, or 9. It will not work in any other ports.
 *
 * Author: Ryan Bori
 * Email: ryan.bori@spacetrek.com
 * Date: April 2025
 *
 * Commands:
 * exonaut robot;                        //This command creates the main robot instance
 *                                       //This is the object that handles motors and core functions
 *
 * ExoNaut_AICam camera;                 //This command creates an AI Camera object
 *                                       //Provides access to the camera's computer vision functions
 *
 * ExoNaut_AICamLF lineFollower;         //This command creates a specialized line follower object
 *                                       //Provides simplified easy-to-use line following commands
 *
 * robot.begin();                        //This command initializes the robot systems
 *
 * lineFollower.begin(&robot, &camera);  //This command initializes the line follower with robot and camera references
 *                                       //Returns true if initialization was successful
 *
 * lineFollower.setBaseSpeed(speed);     //This command sets the robot's base driving speed
 *                                       //Speed value from 0 to 100
 *
 * lineFollower.simpleFollowLine();      //This command automatically handles line following
 *                                       //It follows the line, slows down for sharp turns, and pivots if the line is lost
 */
