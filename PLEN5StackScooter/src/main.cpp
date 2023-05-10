#include <M5Unified.h>

void setup() {
  M5.begin(); // Initialize M5StackAtomS3
  M5.Lcd.clear(); // Clear the display
  M5.Lcd.setTextSize(2); // Set the text size to 2
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK); // Set the text color to white on black
  M5.Lcd.setCursor(10, 20); // Set the cursor position
  M5.Lcd.print("HelloWorld"); // Print "HelloWorld" on the display
}

void loop() {
  // Empty loop
}
