#pragma once

#include <Arduino.h>

namespace Comm {

// 受信完了時にアプリへ通知するコールバック型（JSONバイト列そのまま）
using OnMessageCallback = void (*)(const uint8_t* data, size_t len);

// ESP-NOW + チャンク化送受信の初期化
// - wifiChannel: 使用するWiFiチャンネル（例: 6）
// - broadcastMac: ブロードキャスト（FF:FF:FF:FF:FF:FF）を登録するためのアドレス
void Init(int wifiChannel, const uint8_t broadcastMac[6]);

// 受信完了時のコールバックを登録
void SetOnMessage(OnMessageCallback cb);

// JSON文字列をブロードキャスト送信（必要に応じてチャンク分割）
void SendJsonBroadcast(const String& json);

// 自身のMACアドレスを取得（文字列）
String GetSelfMacString();

} // namespace Comm
