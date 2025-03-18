#ifndef MAIN_H
#define MAIN_H

#include "config.h"
#include "Arduino.h"
#include "Wav.h"
#include "I2S.h"
#include "Arduino_JSON.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "ESPAsyncWebServer.h"
#include "esp_task_wdt.h"
#include <SD.h>

#define TIME_FOR_CONNECTING 20000
#define I2S_MODE I2S_MODE_ADC_BUILT_IN

#define WAV_PATH "/data.wav"

#define DEBUG Serial
#define ARDUINO Serial2

#define SEND_COMMAND(cmd) ARDUINO.print(cmd)

#define _DEBUG
#ifdef _DEBUG
#define DEBUG_LOG(msg)  Serial.println(msg)
#else
#define DEBUG_LOG(msg)
#endif

////////////////////////////////////////
//    ARDUINO Commands
////////////////////////////////////////

#define ENABLE_LIGHT "L"
#define DISABLE_LIGHT "Q"
#define SPEAK_TEMPERATURE "T"
#define MOVE_FORWARD "F"
#define MOVE_BACKWARD "B"
#define MOVE_LEFT "l"
#define MOVE_RIGHT "r"
#define ROBOCODE_POWER "f"
#define TIRED "t"
#define WHATS_YOUR_NAME "n"
#define WHERE_ARE_YOU_FROM "w"
#define IP_ADDR "I"
#define SPEAK "S"
#define RECOGNIZE "R"

////////////////////////////////////////

const char* ESP_SSID = "Wall-e Access Point";
const char* ESP_SSID_PASS = "12345678";

const char* WIFI_SSID;
const char* WIFI_PASS;

const int record_time = 3;  // second
const int headerSize = 44;
const int waveDataSize = record_time * 32000;
const int numCommunicationData = 8000;
const int numPartWavData = numCommunicationData / 4;

#endif // MAIN_H