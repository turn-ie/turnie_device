#include <WiFi.h>
#include <ArduinoOTA.h>
#include "OTA_Handler.h"

const char* ssid = "IA4-411";
const char* password = "gEdCx5Rdm9J9WNAJ7xN7";

static bool otaReady = false;

void setupOTA() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // 最大1秒間だけ接続を試みる
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(50);
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    ArduinoOTA.setPassword("0000");
    ArduinoOTA.setHostname("WifiOTA_NWstudio");
    ArduinoOTA.begin();
    otaReady = true;
    Serial.println("✅ OTA Ready");
  } else {
    Serial.println("⚠️ WiFi failed, continuing without OTA");
    otaReady = false;
  }
}

void handleOTA() {
  if (otaReady) {
    ArduinoOTA.handle();
  }
}
