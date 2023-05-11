#include <M5Unified.h> // M5Unifiedライブラリをプログラムで使用可能にします。
#include <WiFi.h>
#include <ESPNowW.h>

esp_now_peer_info_t slave;

// 送信コールバック
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//  Serial.print("Last Packet Sent to: ");
//  Serial.println(macStr);
//  Serial.print("Last Packet Send Status: ");
//  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  // 画面にも描画
  M5.Display.fillScreen(BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.print("Last Packet Sent to: \n  ");
  M5.Display.println(macStr);
  M5.Display.print("Last Packet Send Status: \n  ");
  M5.Display.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
// 受信コールバック
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.printf("Last Packet Recv from: %s\n", macStr);
  Serial.printf("Last Packet Recv Data(%d): ", data_len);
  for ( int i = 0 ; i < data_len ; i++ ) {
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.println("");
  // 画面にも描画
  M5.Display.fillScreen(BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.print("Last Packet Recv from: \n  ");
  M5.Display.println(macStr);
  M5.Display.printf("Last Packet Recv Data(%d): \n  ", data_len);
  for ( int i = 0 ; i < data_len ; i++ ) {
    M5.Display.print(data[i]);
    M5.Display.print(" ");
  }
}
// setup関数は起動時に一度だけ実行されます。
// 主に初期化処理を記述します。
void setup() {

  auto cfg = M5.config();       // M5Stack初期設定用の構造体を代入
  // configを設定する場合はここで設定
  // 例
  // cfg.external_spk = true;

  M5.begin(cfg);                           // M5デバイスの初期化

  M5.Display.setTextSize(1);               // テキストサイズを変更
  M5.Display.print("Hello World!!");       // 画面にHello World!!と1行表示
//  Serial.println("Hello World!!");         // シリアルモニターにHello World!!と1行表示
  count = 0;                               // countを初期化

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
//    Serial.println("ESPNow Init Success");
    M5.Display.print("ESPNow Init Success\n");
  } else {
//    Serial.println("ESPNow Init Failed");
    M5.Display.print("ESPNow Init Failed\n");
    ESP.restart();
  }
  // マルチキャスト用Slave登録
  memset(&slave, 0, sizeof(slave));
  for (int i = 0; i < 6; ++i) {
    slave.peer_addr[i] = (uint8_t)0xff;
  }
  esp_err_t addStatus = esp_now_add_peer(&slave);
  if (addStatus == ESP_OK) {
    // Pair success
//    Serial.println("Pair success");
  }
  // ESP-NOWコールバック登録
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);


}

// loop関数は起動している間ずっと繰り返し実行されます。
// センサーから値を取得したり、画面を書き換える動作等をおこないます。
void loop() {

//  M5.Display.setCursor(0, 20);              // 文字の描画座標（カーソル位置）を設定
//  M5.Display.printf("COUNT: %d\n", count);  // countを画面に表示
//  Serial.printf("COUNT: %d\n", count);      // countをシリアルに表示

  M5.update();
  // ボタンを押したら送信
  if ( M5.BtnA.wasPressed() ) {
    uint8_t data[2] = {123, 234};
    esp_err_t result = esp_now_send(slave.peer_addr, data, sizeof(data));
//    Serial.print("Send Status: ");
    if (result == ESP_OK) {
//      Serial.println("Success");
    } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
//      Serial.println("ESPNOW not Init.");
    } else if (result == ESP_ERR_ESPNOW_ARG) {
//      Serial.println("Invalid Argument");
    } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
//      Serial.println("Internal Error");
    } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
//      Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
//      Serial.println("Peer not found.");
    } else {
//      Serial.println("Not sure what happened");
    }
  }

  delay(1);

}