#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"
#include "storage.h"
#include "webserver_utils.h"
#include <ArduinoJson.h>

unsigned long lastTime = 0;

// gunakan kapasitas yang jelas
StaticJsonDocument<512> httpResData;

void setup() {
  delay(500);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());

  initSensors();
  // Web server
  setupWebServer();
}

void loop() {
  server.handleClient();

  // POST setiap interval
  if ((millis() - lastTime) >= configData.timerDelay) {
    lastTime = millis();

    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;   // bikin client baru setiap request
      HTTPClient http;

      if (http.begin(client, configData.serverName)) {
        http.useHTTP10(true); // disable keep-alive, aman untuk ESP32
        http.addHeader("Content-Type", "application/json");

        // JSON yang dikirim
        StaticJsonDocument<256> docToSend;
        docToSend["uuid_device"] = configData.uuidDevice;
        docToSend["depth"]       = getSensor1();
        docToSend["temperature"] = getSensor2();
        docToSend["humidity"]    = getSensor3();

        String bodyDoc;
        serializeJson(docToSend, bodyDoc);

        int httpResCode = http.POST(bodyDoc);
        if (httpResCode > 0) {
          Serial.printf("[POST] Code: %d\n", httpResCode);
          String httpPayload = http.getString();
          Serial.println(httpPayload);

          // parse JSON response
          DeserializationError parseError = deserializeJson(httpResData, httpPayload);
          if (parseError) {
            Serial.print("Failed to parse JSON: ");
            Serial.println(parseError.c_str());
          } else {
            int statusCode = httpResData["status"] | -1; // default -1 kalau tidak ada
            Serial.printf("Server status: %d\n", statusCode);
          }
        } else {
          Serial.print("HTTP POST error: ");
          Serial.println(httpResCode);
        }

        http.end(); // WAJIB: selalu dipanggil
      }
    }
  }
}

