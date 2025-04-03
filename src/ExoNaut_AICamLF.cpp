/*
 * ExoNaut_AICamLF.cpp
 *
 * Author: Based on Andrew Gafford's ExoNaut library
 * Date: March 12, 2025
 *
 * Implementation of the ExoNaut_AICamLF class for the Space Trek
 * ExoNaut Robot.
 */

#include "ExoNaut_AICamLF.h"

ExoNaut_AICamLF::ExoNaut_AICamLF() : _robot(nullptr), _camera(nullptr), _initialized(false)
{
    // Initialize last line status array to LINE_STATUS_NONE
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

    // Check if camera is available
    char firmwareVersion[16];
    if (!_camera->firmwareVersion(firmwareVersion))
    {
        return false;
    }

    // Set camera to line following mode
    if (!_camera->changeFunc(APPLICATION_LINEFOLLOW))
    {
        return false;
    }

    // Turn on the camera LED
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

    // Update camera results
    _camera->updateResult();
    return true;
}

uint8_t ExoNaut_AICamLF::getLineStatus(uint8_t lineId)
{
    if (!_initialized || lineId > MAX_LINE_IDS)
    {
        return LINE_STATUS_NONE;
    }

    // Check if the specified line is detected
    if (_camera->lineIdDetected(lineId))
    {
        WonderCamLineResult lineData;
        if (_camera->lineId(lineId, &lineData))
        {
            // Update line status based on offset
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
        // Line was detected before but now it's lost
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

void ExoNaut_AICamLF::printLineData(uint8_t lineId)
{
    if (!_initialized || lineId > MAX_LINE_IDS)
    {
        Serial.println("Line follower not initialized or invalid line ID!");
        return;
    }

    if (_camera->lineIdDetected(lineId))
    {
        WonderCamLineResult lineData;
        if (_camera->lineId(lineId, &lineData))
        {
            Serial.print("Line ID ");
            Serial.print(lineId);
            Serial.println(" data:");

            Serial.print("  Start point: (");
            Serial.print(lineData.start_x);
            Serial.print(", ");
            Serial.print(lineData.start_y);
            Serial.println(")");

            Serial.print("  End point: (");
            Serial.print(lineData.end_x);
            Serial.print(", ");
            Serial.print(lineData.end_y);
            Serial.println(")");

            Serial.print("  Angle: ");
            Serial.print(lineData.angle);
            Serial.println(" degrees");

            Serial.print("  Offset: ");
            Serial.println(lineData.offset);

            // Print line status
            Serial.print("  Status: ");
            switch (getLineStatus(lineId))
            {
            case LINE_STATUS_CENTERED:
                Serial.println("CENTERED");
                break;
            case LINE_STATUS_LEFT:
                Serial.println("LEFT");
                break;
            case LINE_STATUS_RIGHT:
                Serial.println("RIGHT");
                break;
            default:
                Serial.println("UNKNOWN");
                break;
            }
        }
        else
        {
            Serial.print("Error reading data for Line ID ");
            Serial.println(lineId);
        }
    }
    else
    {
        Serial.print("Line ID ");
        Serial.print(lineId);
        Serial.println(" not detected");
    }
}

void ExoNaut_AICamLF::printAllLines()
{
    if (!_initialized)
    {
        Serial.println("Line follower not initialized!");
        return;
    }

    uint8_t lineCount = getLineCount();
    Serial.print("Detected ");
    Serial.print(lineCount);
    Serial.println(" line(s)");

    if (lineCount > 0)
    {
        // Print data for each detected line
        for (uint8_t id = 1; id <= MAX_LINE_IDS; id++)
        {
            if (_camera->lineIdDetected(id))
            {
                printLineData(id);
            }
        }
    }
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

    int16_t offset = getLineOffset(lineId);
    int16_t angle = getLineAngle(lineId);

    // Calculate motor speeds based on line position
    float leftSpeed = baseSpeed;
    float rightSpeed = baseSpeed;

    if (_camera->lineIdDetected(lineId))
    {
        // Apply proportional control based on offset and angle
        float offsetCorrection = (offset / 160.0) * turnFactor;
        float angleCorrection = (angle / 90.0) * turnFactor * 0.5;

        // Combine corrections
        float correction = offsetCorrection + angleCorrection;

        // Apply correction to motor speeds
        leftSpeed += correction;
        rightSpeed -= correction;
    }
    else
    {
        // No line detected, stop or perform recovery behavior
        leftSpeed = 0;
        rightSpeed = 0;
    }

    // Set motor speeds
    _robot->set_motor_speed(leftSpeed, rightSpeed);
}