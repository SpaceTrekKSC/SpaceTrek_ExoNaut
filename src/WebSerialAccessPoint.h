#ifndef WEBSERIALACCESSPOINT_H
#define WEBSERIALACCESSPOINT_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class WebSerialAccessPointClass
{
public:
    void begin(const char *ssid = "WebSerial", const char *password = "12345678");
    void print(const String &message);
    void println(const String &message);
    void setReadyToRun(bool state);

private:
    AsyncWebServer server{80};
    bool readyToRun = false;
    bool waitingForPlay = false;
};

extern WebSerialAccessPointClass WebSerial;

#endif
