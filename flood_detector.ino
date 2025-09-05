#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"
#include "storage.h"
#include "output.h"
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

  // init Sensor Pin
  initSensors();
  //init Output Pin
  initOutput();
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
            int statusCode = httpResData["status"] | 0; // default -1 kalau tidak ada
            if(statusCode == 1) {
              int relayValue = httpResData["sirine"] | 0;
              setRelay(relayValue);
            }
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

// #include <WiFi.h>
// #include <HTTPClient.h>
// #include "config.h"
// #include "storage.h"
// #include "output.h"
// #include "webserver_utils.h"
// #include <ArduinoJson.h>

// // timer
// unsigned long lastPostTime = 0;
// unsigned long lastGetTime  = 0;

// // interval (ms)
// const unsigned long POST_INTERVAL = 5UL * 60UL * 1000UL; // 5 menit
// const unsigned long GET_INTERVAL  = 1UL * 60UL * 1000UL; // 1 menit

// // JSON buffer
// StaticJsonDocument<512> httpResData;

// void setup() {
//   delay(500);
//   Serial.begin(115200);
//   WiFi.begin(WIFI_SSID, WIFI_PASS);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());

//   // init sensor dan relay
//   initSensors();
//   initOutput();
//   // webserver
//   setupWebServer();
// }

// void loop() {
//   server.handleClient();
//   unsigned long now = millis();

//   // =====================
//   // POST ke URL A (5 menit)
//   // =====================
//   if (now - lastPostTime >= POST_INTERVAL) {
//     lastPostTime = now;
//     if (WiFi.status() == WL_CONNECTED) {
//       WiFiClient client;
//       HTTPClient http;

//       if (http.begin(client, configData.serverName)) { // URL A
//         http.useHTTP10(true);
//         http.addHeader("Content-Type", "application/json");

//         // buat payload JSON
//         StaticJsonDocument<256> docToSend;
//         docToSend["uuid_device"] = configData.uuidDevice;
//         docToSend["depth"]       = getSensor1();
//         docToSend["temperature"] = getSensor2();
//         docToSend["humidity"]    = getSensor3();

//         String bodyDoc;
//         serializeJson(docToSend, bodyDoc);

//         int httpResCode = http.POST(bodyDoc);
//         if (httpResCode > 0) {
//           Serial.printf("[POST] Code: %d\n", httpResCode);
//           String httpPayload = http.getString();
//           Serial.println(httpPayload);
//         } else {
//           Serial.print("HTTP POST error: ");
//           Serial.println(httpResCode);
//         }
//         http.end();
//       }
//     }
//   }

//   // =====================
//   // GET ke URL B (1 menit)
//   // =====================
//   if (now - lastGetTime >= GET_INTERVAL) {
//     lastGetTime = now;
//     if (WiFi.status() == WL_CONNECTED) {
//       WiFiClient client;
//       HTTPClient http;

//       if (http.begin(client, configData.serverNameB)) { // URL B
//         http.useHTTP10(true);

//         int httpResCode = http.GET();
//         if (httpResCode > 0) {
//           Serial.printf("[GET] Code: %d\n", httpResCode);
//           String httpPayload = http.getString();
//           Serial.println(httpPayload);

//           // parse JSON GET
//           DeserializationError parseError = deserializeJson(httpResData, httpPayload);
//           if (!parseError) {
//             int statusCode = httpResData["status"] | 0;
//             if (statusCode == 1) {
//               int relayValue = httpResData["sirine"] | 0;
//               setRelay(relayValue);
//             }
//             Serial.printf("GET Server status: %d\n", statusCode);
//           } else {
//             Serial.print("Failed to parse GET JSON: ");
//             Serial.println(parseError.c_str());
//           }
//         } else {
//           Serial.print("HTTP GET error: ");
//           Serial.println(httpResCode);
//         }
//         http.end();
//       }
//     }
//   }
// }

