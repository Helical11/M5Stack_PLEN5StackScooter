#include <M5Unified.h> // M5Unifiedライブラリをプログラムで使用可能にします。

const int CLK1  = 5;
const int DIR1 = 6;
const int RST1 = 7;
double Fre1 = 100;

void setup() {
  pinMode(CLK1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(RST1, OUTPUT);
  digitalWrite(RST1, HIGH);
  digitalWrite(DIR1, LOW);

  auto cfg = M5.config();
  cfg.internal_imu = true;
  M5.begin(cfg);

  ledcSetup(1,Fre1,8);
  ledcAttachPin(CLK1,1);

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

    Fre1 = 5000 * data.accel.y;

    digitalWrite(DIR1, LOW);
    if(Fre1 < 0 ) {
      digitalWrite(DIR1, HIGH);
      Fre1 *= -1;
    }

//      Fre1 = Fre1 + 100;
      ledcSetup(1,Fre1,8);
//      digitalWrite(RST1, HIGH);
      ledcWrite(1,128);
      M5.Display.startWrite();
      M5.Display.setCursor(0, 20);
      M5.Display.print(data.accel.x);
      M5.Display.setCursor(0, 40);
      M5.Display.print(Fre1);
      M5.Display.endWrite();

    }

  delay(100);

}