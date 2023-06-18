#include <M5Unified.h> // M5Unifiedライブラリをプログラムで使用可能にします。

const int CLK1  = 5;
const int DIR1 = 6;
const int RST1 = 7;
uint32_t Fre1 = 1000;

void setup() {
  pinMode(CLK1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(RST1, OUTPUT);
  digitalWrite(RST1, LOW);
  digitalWrite(DIR1, LOW);

  auto cfg = M5.config();
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


    if (M5.BtnA.wasClicked() )
    {
      Fre1 = Fre1 + 1000;
      ledcSetup(1,Fre1,8);
      digitalWrite(RST1, HIGH);
      ledcWrite(1,128);
      M5.Display.startWrite();
      M5.Display.setCursor(0, 20);
      M5.Display.print("PWM_ON");
      M5.Display.endWrite();


    }

  delay(100);

}