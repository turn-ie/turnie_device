#include "Json_Handler.h"
#include "DisplayManager.h"

String displayFlag;
String displayText;
std::vector<uint8_t> rgbData;

static JsonObject parseJsonFile(const char* path) {
    if (!LittleFS.begin(false)) LittleFS.begin(true);
    File file = LittleFS.open(path, "r");
    if (!file) return JsonObject();
    
    static StaticJsonDocument<2048> doc;
    deserializeJson(doc, file);
    file.close();
    return doc.as<JsonObject>();
}

void loadDisplayDataFromJson() {
    JsonObject obj = parseJsonFile("/data.json");
    if (!obj) return;
    
    displayFlag = obj["flag"] | "";
    
    if (displayFlag == "text") {
        displayText = obj["text"] | "";
        rgbData.clear();
    } else if (displayFlag == "image") {
        displayText.clear();
        rgbData.clear();
        JsonArray arr = obj["rgb"];
        for (auto v : arr) rgbData.push_back((uint8_t)v.as<int>());
    } else {
        displayText.clear();
        rgbData.clear();
    }
}

void saveIncomingJson(const uint8_t* data, size_t len) {
    if (!data || len == 0) return;
    if (!LittleFS.begin(false)) LittleFS.begin(true);
    File file = LittleFS.open("/data.json", "w");
    if (file) {
        file.write(data, len);
        file.close();
    }
}

String loadJsonFromPath(const char* path, size_t maxBytes) {
    if (!LittleFS.begin(false)) LittleFS.begin(true);
    if (!LittleFS.exists(path)) return String();
    File f = LittleFS.open(path, "r");
    if (!f || f.isDirectory()) return String();
    String s;
    s.reserve(min((size_t)f.size(), maxBytes));
    while (f.available() && s.length() < (int)maxBytes) s += (char)f.read();
    f.close();
    return s;
}

bool performDisplay() {
    String flag = displayFlag;
    if (flag.isEmpty()) return false;
    flag.toLowerCase();
    
    if (flag == "text") {
        if (displayText.isEmpty()) return false;
        DisplayManager::SetTextBrightness(TEXT_BRIGHTNESS);
        unsigned long dur = DisplayManager::TextEstimateDurationMs(displayText.c_str(), TEXT_FRAME_DELAY_MS);
        if (dur) DisplayManager::BlockFor(dur);
        DisplayManager::TextPlayOnce(displayText.c_str(), TEXT_FRAME_DELAY_MS);
        return true;
    }
    
    if (flag == "image" || flag == "photo") {
        if (rgbData.empty()) return false;
        return DisplayManager::ShowRGBRotCCW(rgbData.data(), rgbData.size(), 3000);
    }
    
    return false;
}

void saveIncomingJsonString(const String& json) {
    if (!json.isEmpty()) saveIncomingJson((const uint8_t*)json.c_str(), json.length());
}

const char* getDisplayFlag() {
    return displayFlag.c_str();
}

const char* getDisplayText() {
    return displayText.c_str();
}

const std::vector<uint8_t>& getRgbData() {
    return rgbData;
}
