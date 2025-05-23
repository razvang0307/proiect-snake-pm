#include "common.h"
#include "utils.h"

const int settingsItems = 2;
int settingsIndex = 0;
unsigned long settingsLastMoveTime = 0;
const unsigned long settingsMoveDebounce = 200;

bool music = true;

void handleSettings() {
	uint8_t dir = readJoystick();
	if (millis() - settingsLastMoveTime > settingsMoveDebounce) {
		if (dir & UP) {
			settingsIndex = (settingsIndex - 1 + settingsItems) % settingsItems;
			settingsLastMoveTime = millis();
			tone(PIN_BUZZER, 440, 200);
		} else if (dir & DOWN) {
			settingsIndex = (settingsIndex + 1) % settingsItems;
			settingsLastMoveTime = millis();
			tone(PIN_BUZZER, 440, 200);
		}
	}

	display.clearDisplay();
	display.setCursor(0, 0);
	display.setTextSize(1);
	display.println("Settings");

	display.setCursor(10, 15);
	display.print((settingsIndex == 0) ? "> " : " ");
	display.print("Music   ");
	display.println(music ? "X" : "");

	display.setCursor(10, 25);
	display.print((settingsIndex == 1) ? "> " : " ");
	display.println("Back");
	display.display();

	if (pressJoystick()) {
		delay(150);
		tone(PIN_BUZZER, 220, 200);
		if (settingsIndex == 0)
			music = !music;
		else
			state = STATE_MAIN_MENU;
	}

}
