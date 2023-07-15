#include <M5Unified.h> // M5Unifiedライブラリをプログラムで使用可能にします。
#include "UNIT_MiniJoyC.h"

// グローバル変数（プログラム全体で使用する変数の定義をします。）
uint32_t count;

UNIT_JOYC sensor;

#define POS_X 0
#define POS_Y 1

// setup関数は起動時に一度だけ実行されます。
// 主に初期化処理を記述します。
void setup() {

  auto cfg = M5.config();       // M5Stack初期設定用の構造体を代入
  // configを設定する場合はここで設定
  // 例
  // cfg.external_spk = true;

  M5.begin(cfg);                           // M5デバイスの初期化

  M5.Display.setTextSize(3);               // テキストサイズを変更
//  M5.Display.print("Hello World!!");       // 画面にHello World!!と1行表示
//  Serial.println("Hello World!!");         // シリアルモニターにHello World!!と1行表示
  count = 0;                               // countを初期化

    while (!(sensor.begin(&Wire, JoyC_ADDR, 0, 26, 100000UL))) {
        delay(100);
        Serial.println("I2C Error!\r\n");
    }




}

// loop関数は起動している間ずっと繰り返し実行されます。
// センサーから値を取得したり、画面を書き換える動作等をおこないます。
void loop() {

  M5.Display.setCursor(0, 20);              // 文字の描画座標（カーソル位置）を設定
  M5.Display.printf("COUNT: %d\n", count);  // countを画面に表示
  Serial.printf("COUNT: %u\n", count);      // countをシリアルに表示
  count++;                                  // countを1増やす

  M5.Display.printf("%d\n", sensor.getADCValue(POS_X));


  delay(1000);                              // 1秒待つ

}
