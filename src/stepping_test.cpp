#include <M5Unified.h> // M5Unifiedライブラリをプログラムで使用可能にします。

const int CLK1  = 5;
const int DIR1 = 7;
//const int RST1 = 39;
const int CLK2  = 6;
const int DIR2 = 8;
//const int RST2 = 7;

double Fre1 = 100;
double Fre2 = 100;

void setup() {
  pinMode(CLK1, OUTPUT);
  pinMode(DIR1, OUTPUT);
//  pinMode(RST1, OUTPUT);
  pinMode(CLK2, OUTPUT);
  pinMode(DIR2, OUTPUT);
//  pinMode(RST2, OUTPUT);

//  digitalWrite(RST1, LOW);
  digitalWrite(DIR1, LOW);
//  digitalWrite(RST2, HIGH);
  digitalWrite(DIR2, HIGH);

  auto cfg = M5.config();
  cfg.internal_imu = true;
  M5.begin(cfg);

  ledcSetup(1,Fre1,8);
  ledcAttachPin(CLK1,1);
  ledcSetup(2,Fre2,8);
  ledcAttachPin(CLK2,1);

}

void loop() {

  M5.update();
  M5.Display.startWrite();
  M5.Display.setCursor(0, 0);
  M5.Display.print(millis());
  M5.Display.endWrite();

  auto imu_update = M5.Imu.update();
  if (imu_update)
  {
    auto data = M5.Imu.getImuData();
    data.accel.x;      // accel x-axis value.
    data.accel.y;      // accel y-axis value.
    data.accel.z;

    Fre1 = 1000 * data.accel.z;
    Fre2 = Fre1;

    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, HIGH);
    if(Fre1 < 0 ) {
      digitalWrite(DIR1, HIGH);
      digitalWrite(DIR2, LOW);
      Fre1 *= -1;
      Fre2 = Fre1;
    }

//      Fre1 = Fre1 + 100;
      ledcSetup(1,Fre1,8);
      ledcSetup(2,Fre2,8);
//      digitalWrite(RST1, HIGH);
      ledcWrite(1,128);
      ledcWrite(2,128);
      M5.Display.startWrite();
      M5.Display.setCursor(0, 20);
      M5.Display.print(data.accel.x);
      M5.Display.setCursor(0, 40);
      M5.Display.print(data.accel.y);
      M5.Display.setCursor(0, 60);
      M5.Display.print(data.accel.z);
      M5.Display.setCursor(0, 80);
      M5.Display.print(Fre1);
      M5.Display.endWrite();

    }

  delay(100);

}