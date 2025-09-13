#pragma once
#include <WebServer.h>
#include "webpage.h"
#include "sensors.h"

WebServer server(80);

void handleRoot() {
  String html = buildHtmlPage(
    serverNameA,
    serverNameB,
    timerDelay,
    uuidDevice,
    getSensor1(),
    getSensor2(),
    getSensor3()
  );
  server.send(200, "text/html", html);
}

// void handleSave() {
//   if (!server.hasArg("server") || !server.hasArg("delay") || !server.hasArg("uuid")) {
//     server.send(400, "text/plain", "Missing fields");
//     return;
//   }

//   String serverUrl = server.arg("server");
//   unsigned long delayMin = server.arg("delay").toInt();
//   unsigned long delayMs = (delayMin > 0) ? delayMin * 60000UL : configData.timerDelay;
//   int uuid = server.arg("uuid").toInt();

//   saveConfig(serverUrl, delayMs, uuid);

//   configData.serverName = serverUrl;
//   configData.timerDelay = delayMs;
//   configData.uuidDevice = uuid;

//   server.send(200, "text/plain", "Saved! Restart or refresh to apply.");
// }

void setupWebServer() {
  server.on("/", handleRoot);
  // server.on("/save", HTTP_POST, handleSave);
  server.begin();
}
