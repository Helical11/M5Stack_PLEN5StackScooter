#include <M5Unified.h>

void setup() {
  M5.begin(); // Initialize M5StackAtomS3
  M5.Imu.begin(); // Initialize the IMU sensor
  M5.Lcd.setTextSize(2); // Set the text size to 2
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK); // Set the text color to white on black
}

float ax, ay, az, gx, gy, gz, t;

void loop() {
  M5.Lcd.clear(); // 画面をクリア
  M5.Imu.getAccel(&ax, &ay, &az);
  M5.Imu.getGyro(&gx, &gy, &gz);
  float zAccel = az / 1000.0; // Z軸方向の加速度をmgからgに変換
  M5.Lcd.setCursor(10, 20); // カーソル位置を設定
  M5.Lcd.printf("Z-Accel: %.2fg", az); // Z軸方向の加速度の値を表示
  delay(100); // 100ミリ秒待つ
}
