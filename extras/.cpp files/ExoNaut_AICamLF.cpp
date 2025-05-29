/*
 * ExoNaut_AICamLF.cpp
 *
 * Author:Andrew Gafford
 * Updated: April 2025 (Simplified Line Following Functions)
 *
 * Implementation of the ExoNaut_AICamLF class for the Space Trek
 * ExoNaut Robot.
 */

 #include "ExoNaut_AICamLF.h"

 ExoNaut_AICamLF::ExoNaut_AICamLF() : _robot(nullptr), _camera(nullptr), _initialized(false), _baseSpeed(40.0f)
 {
     for (int i = 0; i <= MAX_LINE_IDS; i++)
     {
         _lastLineStatus[i] = LINE_STATUS_NONE;
     }
 }
 
 bool ExoNaut_AICamLF::begin(exonaut *robot, ExoNaut_AICam *camera)
 {
     if (robot == nullptr || camera == nullptr)
     {
         return false;
     }
 
     _robot = robot;
     _camera = camera;
 
     char firmwareVersion[16];
     if (!_camera->firmwareVersion(firmwareVersion))
     {
         return false;
     }
 
     if (!_camera->changeFunc(APPLICATION_LINEFOLLOW))
     {
         return false;
     }
 
     _camera->setLed(WONDERCAM_LED_ON);
 
     _initialized = true;
     return true;
 }
 
 bool ExoNaut_AICamLF::update()
 {
     if (!_initialized)
     {
         return false;
     }
 
     _camera->updateResult();
     return true;
 }
 
 uint8_t ExoNaut_AICamLF::getLineStatus(uint8_t lineId)
 {
     if (!_initialized || lineId > MAX_LINE_IDS)
     {
         return LINE_STATUS_NONE;
     }
 
     if (_camera->lineIdDetected(lineId))
     {
         WonderCamLineResult lineData;
         if (_camera->lineId(lineId, &lineData))
         {
             if (abs(lineData.offset) < LINE_FOLLOW_THRESHOLD)
             {
                 _lastLineStatus[lineId] = LINE_STATUS_CENTERED;
             }
             else if (lineData.offset < 0)
             {
                 _lastLineStatus[lineId] = LINE_STATUS_LEFT;
             }
             else
             {
                 _lastLineStatus[lineId] = LINE_STATUS_RIGHT;
             }
         }
     }
     else if (_lastLineStatus[lineId] != LINE_STATUS_NONE)
     {
         _lastLineStatus[lineId] = LINE_STATUS_LOST;
     }
 
     return _lastLineStatus[lineId];
 }
 
 int16_t ExoNaut_AICamLF::getLineAngle(uint8_t lineId)
 {
     if (!_initialized || lineId > MAX_LINE_IDS)
     {
         return 0;
     }
 
     WonderCamLineResult lineData;
     if (_camera->lineIdDetected(lineId) && _camera->lineId(lineId, &lineData))
     {
         return lineData.angle;
     }
 
     return 0;
 }
 
 int16_t ExoNaut_AICamLF::getLineOffset(uint8_t lineId)
 {
     if (!_initialized || lineId > MAX_LINE_IDS)
     {
         return 0;
     }
 
     WonderCamLineResult lineData;
     if (_camera->lineIdDetected(lineId) && _camera->lineId(lineId, &lineData))
     {
         return lineData.offset;
     }
 
     return 0;
 }
 
 bool ExoNaut_AICamLF::getLineData(uint8_t lineId, WonderCamLineResult *data)
 {
     if (!_initialized || lineId > MAX_LINE_IDS || data == nullptr)
     {
         return false;
     }
 
     return _camera->lineIdDetected(lineId) && _camera->lineId(lineId, data);
 }
 
 bool ExoNaut_AICamLF::isLineDetected(uint8_t lineId)
 {
     if (!_initialized || lineId > MAX_LINE_IDS)
     {
         return false;
     }
 
     return _camera->lineIdDetected(lineId);
 }
 
 uint8_t ExoNaut_AICamLF::getLineCount()
 {
     if (!_initialized)
     {
         return 0;
     }
 
     return _camera->numOfLineDetected();
 }
 
 void ExoNaut_AICamLF::followLine(uint8_t lineId, float baseSpeed, float turnFactor)
 {
     if (!_initialized || lineId > MAX_LINE_IDS)
     {
         return;
     }
 
     WonderCamLineResult lineData;
     if (_camera->lineIdDetected(lineId) && _camera->lineId(lineId, &lineData))
     {
         float centerX = LINE_FOLLOW_CENTER;
         float errorStartX = (lineData.start_x - centerX) / centerX;
         float errorAngle = (float)(lineData.angle) / 90.0f;
         float correction = (OFFSET_WEIGHT * errorStartX) + (ANGLE_WEIGHT * errorAngle);
         correction *= turnFactor;
 
         float leftSpeed = baseSpeed + correction;
         float rightSpeed = baseSpeed - correction;
 
         leftSpeed = constrain(leftSpeed, -100.0f, 100.0f);
         rightSpeed = constrain(rightSpeed, -100.0f, 100.0f);
 
         _robot->set_motor_speed(leftSpeed, rightSpeed);
     }
 }
 
 void ExoNaut_AICamLF::setBaseSpeed(float speed)
 {
     _baseSpeed = speed;
 }
 
 void ExoNaut_AICamLF::simpleFollowLine()
 {
     if (!_initialized)
         return;
 
     static unsigned long lastScan = 0;
     static unsigned long lostLineTimer = 0;
     static bool recovering = false;
     static int lastKnownAngle = 0;
 
     if (millis() - lastScan >= SCAN_INTERVAL_MS)
     {
         lastScan = millis();
         update();
 
         uint8_t lineStatus = getLineStatus(1);
 
         if (lineStatus == LINE_STATUS_CENTERED || lineStatus == LINE_STATUS_LEFT || lineStatus == LINE_STATUS_RIGHT)
         {
             WonderCamLineResult lineData;
             if (getLineData(1, &lineData))
             {
                 float centerX = LINE_FOLLOW_CENTER;
                 float errorStartX = (lineData.start_x - centerX) / centerX;
                 float errorAngle = (float)(lineData.angle) / 90.0f;
                 float correction = (OFFSET_WEIGHT * errorStartX) + (ANGLE_WEIGHT * errorAngle);
                 correction *= TURN_FACTOR;
 
                 float speed = _baseSpeed;
                 if (abs(lineData.angle) > SHARP_TURN_THRESHOLD)
                 {
                     speed = SLOW_SPEED;
                 }
 
                 float leftSpeed = speed + correction;
                 float rightSpeed = speed - correction;
 
                 leftSpeed = constrain(leftSpeed, -100.0f, 100.0f);
                 rightSpeed = constrain(rightSpeed, -100.0f, 100.0f);
 
                 _robot->set_motor_speed(leftSpeed, rightSpeed);
 
                 recovering = false;
                 lastKnownAngle = lineData.angle;
             }
         }
         else if (lineStatus == LINE_STATUS_LOST)
         {
             if (!recovering)
             {
                 recovering = true;
                 lostLineTimer = millis();
             }
 
             if (millis() - lostLineTimer < LOST_RECOVERY_TIMEOUT)
             {
                 if (lastKnownAngle > 0)
                 {
                     _robot->set_motor_speed(PIVOT_SPEED, -PIVOT_SPEED);
                 }
                 else
                 {
                     _robot->set_motor_speed(-PIVOT_SPEED, PIVOT_SPEED);
                 }
             }
             else
             {
                 _robot->set_motor_speed(0, 0);
             }
         }
         else
         {
             _robot->set_motor_speed(0, 0);
         }
     }
 }
 