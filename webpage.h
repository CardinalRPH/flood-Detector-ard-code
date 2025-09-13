#pragma once
#include <WiFi.h>

// Buat HTML page
String buildHtmlPage(String serverNameA, String serverNameB, unsigned long timerDelay, int uuid_device, int s1, int s2, int s3) {
  unsigned long minutes = timerDelay / 60000UL;
  if (minutes == 0) minutes = 0;

  String html = R"====(
<!DOCTYPE html><html><head>
<meta charset="UTF-8">
<title>ESP32 Config</title>
<style>
body { font-family: Arial; background: #0f172a; color: #e5e7eb; }
.card { background: #111827; padding: 20px; margin: 20px auto; border-radius: 12px; max-width: 700px; }
.btn { background: #22d3ee; border: none; padding: 10px 16px; border-radius: 6px; cursor: pointer; font-weight: bold; }
.sensor { margin-top: 14px; background: #1e293b; padding: 12px; border-radius: 8px; }
</style>
</head><body>
<div class='card'>
  <h1>ESP32 Config</h1>
 <div> 
 <label>Server URL</label>
    <h3>
    URL A: )====";
  html += serverNameA;
  html += R"====(
    </h3>
    <br>

    <h3>
    URL B:  )====";
  html += serverNameB;
  html += R"====(
    </h3>
    <br>

    <h3>
    Interval (menit) : )====";
  html += String(minutes);
  html += R"====(
    </h3>
    <br>

    <h3>
    UUID : )====";
  html += String(uuid_device);
  html += R"====(
    </h3>
    <br>
 </div>

  <div class='sensor'>
    <h3>Sensor Data</h3>
    <p>Sensor 1: )====" + String(s1) + R"====(</p>
    <p>Sensor 2: )====" + String(s2) + R"====(</p>
    <p>Sensor 3: )====" + String(s3) + R"====(</p>
  </div>
</div></body></html>
)====";
  return html;
}
