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
    void print(int number);
    void print(float number);
    void print(double number);
    void print(unsigned int number);
    void println(const String &message);
    void println(int number);
    void println(float number);
    void println(double number);
    void println(unsigned int number);
    void setReadyToRun(bool state);

private:
    AsyncWebServer server{80};
    bool readyToRun = false;
    bool waitingForPlay = false;
};

extern WebSerialAccessPointClass WebSerial;

#endif
