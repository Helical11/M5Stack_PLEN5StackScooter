#include <M5Unified.h> // M5Unifiedライブラリをプログラムで使用可能にします。
#include <UNIT_MiniJoyC.h>
#include <WiFi.h>
#include <ESPNowW.h>

// グローバル変数（プログラム全体で使用する変数の定義をします。）
UNIT_JOYC sensor;

#define POS_X 0
#define POS_Y 1

uint8_t receiver_mac[] = {0xA4, 0xCF, 0x12, 0x44, 0x22, 0x14};

void setup() {

  auto cfg = M5.config();       // M5Stack初期設定用の構造体を代入
  // configを設定する場合はここで設定
  // 例
  // cfg.external_spk = true;

  M5.begin(cfg);                           // M5デバイスの初期化

  M5.Display.setTextSize(3);               // テキストサイズを変更

//コントローラのシリアル通信立ち上げ
    while (!(sensor.begin(&Wire, JoyC_ADDR, 0, 26, 100000UL))) {
        delay(100);
        Serial.println("I2C Error!\r\n");
    }


  WiFi.mode(WIFI_MODE_STA);
  WiFi.disconnect();
  ESPNow.init();
  ESPNow.add_peer(receiver_mac);
  if (esp_now_init() == ESP_OK) {
//    Serial.println("ESPNow Init Success");
    M5.Display.print("ESPNow Init Success\n");
  } else {
//    Serial.println("ESPNow Init Failed");
    M5.Display.print("ESPNow Init Failed\n");
    ESP.restart();
  }

//  Serial.println(WiFi.macAddress());


}

// loop関数は起動している間ずっと繰り返し実行されます。
// センサーから値を取得したり、画面を書き換える動作等をおこないます。
void loop() {

  M5.Display.setCursor(0, 20);              // 文字の描画座標（カーソル位置）を設定

  M5.Display.printf("%4u\n", sensor.getADCValue(POS_X));
  M5.Display.printf("%4u\n", sensor.getADCValue(POS_Y));
  M5.Display.printf("%4u\n",sensor.getButtonStatus());

  uint8_t value = sensor.getADCValue(POS_X)>>3;
  ESPNow.send_message(receiver_mac,&value,1);

  delay(100);                              // 1秒待つ

}
