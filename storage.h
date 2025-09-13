#pragma once
#include <Preferences.h>
#include "config.h"

Preferences preferences;

// configuration storage structure
struct ESPConfig {
  String serverName;
  unsigned long timerDelay;
  int uuidDevice;
};
// Non-Volatile Storage
// Load NVS config 
ESPConfig loadConfig() {
  ESPConfig cfg;
  preferences.begin("config", true);
  cfg.serverName = preferences.getString("server", serverNameDefault);
  cfg.timerDelay = preferences.getULong("delay", timerDelayDefault);
  cfg.uuidDevice = preferences.getInt("uuid", uuidDeviceDefault);
  preferences.end();
  return cfg;
}

// Save config ke NVS
// void saveConfig(String server, unsigned long delay, int uuid) {
//   preferences.begin("config", false);
//   preferences.putString("server", server);
//   preferences.putULong("delay", delay);
//   preferences.putInt("uuid", uuid);
//   preferences.end();
// }
