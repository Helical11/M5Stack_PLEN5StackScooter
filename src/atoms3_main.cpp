#include <M5Unified.h> // M5Unifiedライブラリをプログラムで使用可能にします。

// GPIOピン設定
const int CLK1  = 5;
const int DIR1 = 7;
const int CLK2  = 6;
const int DIR2 = 8;

// ステッピングモータ回転数指示
int16_t Fre1 = 0;
int16_t Fre2 = 0;

// 車体角度変数 加速度から
float rowAngleA = 0.0;
float AngleA = 0.0;

// 車体角度変数 ジャイロから
float rowAngleG = 0.0;
float AngleG = 0.0;

// 車体角度変数
float preAngle = 0.0;
float Angle = 0.0;

float AccelOffset = 0.0;
float GyroOffset = 0.0;

// ラジアン→度変換
const float RADTODEG = 180/3.14;

// LEDC用PWM解像度
const int16_t PWMBIT = 12;
const int16_t DUTY = 2048;

// 割り込み
hw_timer_t * timer = NULL;

// Teleplot用シリアル出力
void TeleplotPrint() {
  Serial.printf(">rowAngleA:%f\n", rowAngleA);
  Serial.printf(">AngleA:%f\n", AngleA);
  Serial.printf(">rowAngleG:%f\n", rowAngleG);
  Serial.printf(">AngleG:%f\n", AngleG);
  Serial.printf(">Angle:%f\n", Angle);
}

void IMUCalibration() {


  for(int i = 0; i < 1000; i++ ) {
    auto imu_update = M5.Imu.update();
    auto data = M5.Imu.getImuData();
    AccelOffset += atan2(data.accel.z, data.accel.y) * RADTODEG;
    GyroOffset += data.gyro.x;
    printf(">GyroOffset:%f\n", GyroOffset);
    delay(5);
  }

  AccelOffset /= 1000;
  GyroOffset /= 1000;

}

// 割り込み時の実行関数
void IRAM_ATTR onTimer() {

  TeleplotPrint();

}

// セッティング
void setup() {
// GPIO設定
  pinMode(CLK1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(CLK2, OUTPUT);
  pinMode(DIR2, OUTPUT);

// PWM設定
  ledcSetup(1,Fre1,PWMBIT);
  ledcAttachPin(CLK1,1);
  ledcSetup(2,Fre2,PWMBIT);
  ledcAttachPin(CLK2,2);

// M5Stack初期化
  auto cfg = M5.config();
  cfg.internal_imu = true;
  M5.begin(cfg);

  IMUCalibration();

// 割り込みタイマー有効化
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100000, true);
  timerAlarmEnable(timer);
}


// ループ
void loop() {

//  M5.update();
  M5.Display.startWrite();
  M5.Display.setCursor(0, 0);
  M5.Display.print(millis());
  M5.Display.endWrite();

  auto imu_update = M5.Imu.update();
  if (imu_update)
  {
  auto data = M5.Imu.getImuData();

    preAngle = Angle;

// 加速度センサからの角度算出
    rowAngleA = atan2(data.accel.z, data.accel.y) * RADTODEG;
    AngleA = (rowAngleA - AccelOffset);

// ジャイロからの角度算出
    rowAngleG = - (data.gyro.x - GyroOffset )* 0.1;
    AngleG = preAngle + rowAngleG;

// 相補フィルタ
    Angle = 0.44*AngleG + 0.56*AngleA;

//    data.accel.x;   // 車体左右方向
//    data.accel.y;   // 車体重力方向 直立時1
//    data.accel.z;   // 車体重力方向 直立時0

//    data.gyro.x;    // 車輪軸回転
//    data.gyro.y;    // 旋回軸回転
//    data.gyro.z;    // 進行軸回転

// 回転数指示
//    Fre1 = 100*(int16_t)Angle;
//    Fre2 = Fre1;

// 回転方向指示
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, HIGH);
    if(Fre1 < 0 ) {
      digitalWrite(DIR1, HIGH);
      digitalWrite(DIR2, LOW);
      Fre1 *= -1;
      Fre2 = Fre1;
    }

// PWM設定反映
      ledcSetup(1,Fre1,PWMBIT);
      ledcSetup(2,Fre2,PWMBIT);
      ledcWrite(1,DUTY);
      ledcWrite(2,DUTY);

// Display表示
      M5.Display.startWrite();
      M5.Display.setCursor(0, 20);
      M5.Display.print(Angle);

      M5.Display.setCursor(0, 40);
      M5.Display.printf("%4u\n",Fre1);
      M5.Display.setCursor(0, 60);
      M5.Display.printf("%4u\n",Fre2);
      M5.Display.endWrite();


    }

    delay(100);

}