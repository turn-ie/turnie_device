#include <WiFi.h>
#include <ArduinoOTA.h>
#include "OTA_Handler.h"
#include "Display_Manager.h" // ★追加: LED制御のため

const char* ssid = "IA4-411";
const char* password = "gEdCx5Rdm9J9WNAJ7xN7";

static bool otaReady = false;

void setupOTA() {
  // ★変更: 接続試行中は赤色点灯
  DisplayManager::AllOnRed(20);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("[OTA] Connecting to WiFi...");

  // ★変更: 接続待ち時間を延長 (1秒 -> 10秒)
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 200) { // 50ms * 200 = 10秒
    delay(50);
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    ArduinoOTA.setPassword("0000");
    ArduinoOTA.setHostname("WifiOTA_NWstudio");
    ArduinoOTA.begin();
    otaReady = true;
    Serial.println("✅ OTA Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // ★変更: 接続成功したら白色点灯
    DisplayManager::AllOnGreen(20);
  } else {
    Serial.println("⚠️ WiFi failed, OTA aborted.");
    otaReady = false;
    
    // ★追加: 失敗したら消灯
    DisplayManager::Clear();
  }
}

void handleOTA() {
  if (otaReady) {
    ArduinoOTA.handle();
  }
}
