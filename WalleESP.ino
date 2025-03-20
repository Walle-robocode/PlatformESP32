#include "main.h"

AsyncWebServer server(80);
HTTPClient http;

byte header[headerSize];
char communicationData[numCommunicationData];
char partWavData[numPartWavData];

void setup() {
  DEBUG.begin(115200);
  ARDUINO.begin(9600, SERIAL_8N1, 16, 17);
  WiFi.mode(WIFI_MODE_APSTA);
  if (!SD.begin()) {
    DEBUG.println("Error, while init SD card!");
  }
  connectToWiFiWithSavedSettings();
  createAccessPoint();
  setupServer();

  SD.remove(WAV_PATH);

  I2S_Init(I2S_MODE, I2S_BITS_PER_SAMPLE_16BIT);

  http.begin(serverName);
}

long lastCheckTime = 0;
bool isAcknowledge = true;

void loop() {
  if (ARDUINO.available()) {
    char cmd = ARDUINO.read();
    DEBUG.print("We have a command: ");
    DEBUG.println(cmd);
    if (cmd == ARDUINO_ACK_CODE) isAcknowledge = true;
    ARDUINO.flush();
  }
  if (WiFi.status() == WL_CONNECTED) {

    if (!isAcknowledge) return;
    isAcknowledge = false;

    DEBUG_LOG("Plese, speak");
    SEND_COMMAND(SPEAK);
    record();
    DEBUG_LOG("We try ro recognize speech");
    SEND_COMMAND(RECOGNIZE);
    String text = recognize();
    JSONVar obj;
    DEBUG_LOG(text);
    text = getValue(text, '\n', 1);
    obj = JSON.parse(text);
    DEBUG_LOG(text);
    text = String(obj["result"][0]["alternative"][0]["transcript"]);
    text.toLowerCase();
    DEBUG_LOG(text);

    if ((text.indexOf("увімкни") >= 0 || text.indexOf("Увімкни") >= 0) && (text.indexOf("світло") >= 0 || text.indexOf("Свет") >= 0)) {
      DEBUG_LOG(ENABLE_LIGHT);
      SEND_COMMAND(ENABLE_LIGHT);
    } else if ((text.indexOf("вимкни") >= 0 || text.indexOf("Вимкни") >= 0) && (text.indexOf("світло") >= 0 || text.indexOf("Свет") >= 0)) {
      DEBUG_LOG(DISABLE_LIGHT);
      SEND_COMMAND(DISABLE_LIGHT);
    } else if ((text.indexOf("Температур") >= 0 || text.indexOf("температур") >= 0)) {
      DEBUG_LOG(SPEAK_TEMPERATURE);
      SEND_COMMAND(SPEAK_TEMPERATURE);
    } else if ((text.indexOf("Вперед") >= 0 || text.indexOf("вперед") >= 0)) {
      DEBUG_LOG(MOVE_FORWARD);
      SEND_COMMAND(MOVE_FORWARD);
    } else if ((text.indexOf("Назад") >= 0 || text.indexOf("назад") >= 0)) {
      DEBUG_LOG(MOVE_BACKWARD);
      SEND_COMMAND(MOVE_BACKWARD);
    } else if ((text.indexOf("Праворуч") >= 0 || text.indexOf("праворуч") >= 0)) {
      DEBUG_LOG(MOVE_RIGHT);
      SEND_COMMAND(MOVE_RIGHT);
    } else if ((text.indexOf("Ліворуч") >= 0 || text.indexOf("ліворуч") >= 0)) {
      DEBUG_LOG(MOVE_LEFT);
      SEND_COMMAND(MOVE_LEFT);
    } else if ((text.indexOf("Сила") >= 0 || text.indexOf("сила") >= 0) && (text.indexOf("робоко") >= 0 || text.indexOf("Робоко") >= 0)) {
      DEBUG_LOG(ROBOCODE_POWER);
      SEND_COMMAND(ROBOCODE_POWER);
    } else if ((text.indexOf("Зморився") >= 0 || text.indexOf("зморився") >= 0)) {
      DEBUG_LOG(TIRED);
      SEND_COMMAND(TIRED);
    } else if ((text.indexOf("Як") >= 0 || text.indexOf("як") >= 0) && (text.indexOf("Тебе") >= 0 || text.indexOf("тебе") >= 0) && (text.indexOf("Звати") >= 0 || text.indexOf("звати") >= 0)) {
      DEBUG_LOG(WHATS_YOUR_NAME);
      SEND_COMMAND(WHATS_YOUR_NAME);
    } else if ((text.indexOf("Ти") >= 0 || text.indexOf("ти") >= 0) && (text.indexOf("Звідки") >= 0 || text.indexOf("звідки") >= 0)) {
      DEBUG_LOG(WHERE_ARE_YOU_FROM);
      SEND_COMMAND(WHERE_ARE_YOU_FROM);
    } else {
      DEBUG_LOG(EMPTY_COMMAND);
      SEND_COMMAND(EMPTY_COMMAND);
    }
  } else {
    if (millis() > lastCheckTime + 5000) {
      String str = IP_ADDR + WiFi.softAPIP().toString() + "P";
      DEBUG_LOG(str);
      SEND_COMMAND(str);
      lastCheckTime = millis();
    }
  }
}

String recognize() {
  File f = SD.open(WAV_PATH, FILE_READ);
  http.addHeader("Content-Type", "audio/l16; rate=16000");

  int httpResponseCode = http.sendRequest("POST", &f, f.size());

  String resp = "";
  if (httpResponseCode > 0) {
    resp = http.getString();
  }

  f.close();
  return resp;
}

void record() {
  File f = SD.open(WAV_PATH, FILE_WRITE);
  CreateWavHeader(header, waveDataSize);
  f.write(header, headerSize);
  for (uint32_t j = 0; j < waveDataSize / numPartWavData; ++j) {
    I2S_Read(communicationData, numCommunicationData);
    for (uint32_t i = 0; i < numCommunicationData / 8; ++i) {
      partWavData[2 * i] = communicationData[8 * i + 2];
      partWavData[2 * i + 1] = communicationData[8 * i + 3];
    }
    f.write((const byte*)partWavData, numPartWavData);
  }
  f.close();
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String processor(const String& var) {
  return String("Hello");
}

bool connectToWiFiWithSavedSettings() {
  File configs = SD.open("/config.txt");
  if (configs) {
    String configJsonStr = "";
    while (configs.available()) {
      configJsonStr += (char)configs.read();
    }

    configs.close();

    JSONVar configJson = JSON.parse(configJsonStr);

    WIFI_SSID = configJson["ssid"];
    WIFI_PASS = configJson["pass"];

    DEBUG.print(WIFI_SSID);
    DEBUG.print(" ");
    DEBUG.println(WIFI_PASS);

    if (strlen(WIFI_SSID) > 0) return connectToWiFi();
    return false;
  }
}

bool connectToWiFi() {
  //WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  long startConnectTime = millis();
  esp_task_wdt_add(NULL);
  while (WiFi.status() != WL_CONNECTED) {
    esp_task_wdt_reset();
    delay(1000);
    DEBUG.print("Connecting to ");
    DEBUG.println(WIFI_SSID);

    if (millis() > startConnectTime + TIME_FOR_CONNECTING) {
      return false;
    }
  }

  DEBUG.print("Connected with IP: ");
  DEBUG.println(WiFi.localIP());
  esp_task_wdt_delete(NULL);
  return true;
}

void setupServer() {
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SD, "/index.html", String(), false, processor);
    });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SD, "/style.css", "text/css");
    });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SD, "/script.js", "text/javascript");
    });

  // Route to set GPIO to HIGH
  server.on("/wifiCred",
    HTTP_POST,
    [](AsyncWebServerRequest* request) {},
    NULL,
    [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {

      String jsonStr = "";
      for (size_t i = 0; i < len; i++) {
        jsonStr += (char)data[i];
      }

      DEBUG.println(jsonStr);

      JSONVar wifiSettingsJson = JSON.parse(jsonStr);

      WIFI_SSID = wifiSettingsJson["ssid"];
      WIFI_PASS = wifiSettingsJson["pass"];

      DEBUG.print(WIFI_SSID);
      DEBUG.print(" : ");
      DEBUG.println(WIFI_PASS);

      File config = SD.open("/config.txt", "w");
      if (config.print(jsonStr)) {
        request->send(200, "text/plain", "Network credentials saved");
      } else {
        request->send(200, "text/plain", "Something went wrong");
      }

      config.close();
      connectToWiFi();
    });

  server.on("/searchNetworks", HTTP_POST, [](AsyncWebServerRequest* request) {
    String networks = searchNetworks();
    request->send(200, "text/plain", networks);
    });

  server.begin();
}

void createAccessPoint() {
  DEBUG.print("Setting AP (Access Point)…");
  WiFi.softAP(ESP_SSID, ESP_SSID_PASS);

  IPAddress IP = WiFi.softAPIP();
  DEBUG.print("AP IP address: ");
  DEBUG.println(IP);
}

String searchNetworks() {
  String networks = "";

  int n = WiFi.scanNetworks();
  if (n == 0) {
    networks = "{\"error\":\"No networks found\"}";
    return networks;
  }
  networks = "{\"networks\":[";
  for (int i = 0; i < n; ++i) {
    networks += "{\"ssid\":\"" + (String)WiFi.SSID(i) + "\",";
    networks += "\"rssi\":\"" + (String)WiFi.RSSI(i) + "\",";
    String temp = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "false" : "true";
    networks += "\"password\":\"" + temp + "\"}";
    if (i == n - 1) {
      networks += "]}";
    } else {
      networks += ",";
    }
  }

  DEBUG.println(networks);

  return networks;
}
