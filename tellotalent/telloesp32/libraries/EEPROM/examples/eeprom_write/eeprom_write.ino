#include <DFRobot_SSD1306_I2C.h>

DFRobot_SSD1306_I2C oled12864(0x3c);

void setup() {
  oled12864.begin();
  oled12864.setTextColor(1);
  oled12864.setCursorLine(1);
  oled12864.printLine("hello");
}

void loop() {

}
