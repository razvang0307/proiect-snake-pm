#include "common.h"
#include "utils.h"

Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_CLK,PIN_DIN,PIN_DC,PIN_CS,PIN_RST);
STATE state = STATE_MAIN_MENU;

void setup()   {
  Serial.begin(115200);
  display.begin();
  display.setContrast(255);

  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_PRESS, INPUT_PULLUP);
}
void loop(){
  switch (state) {
    case STATE_MAIN_MENU:
      handleMainMenu();
      break;
    case STATE_GAME:
      handleGame();
      break;
    case STATE_PAUSE_MENU:
      handlePauseMenu();
      break;
    default:
      break;
  }
}
