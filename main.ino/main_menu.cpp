#include "common.h"
#include "utils.h"

const int menuItems = 2;
int menuIndex = 0;
STATE options[2] = {STATE_GAME, STATE_SETTINGS};
unsigned long lastMoveTime = 0;
const unsigned long moveDebounce = 200;

void handleMainMenu() {
	uint8_t dir = readJoystick();
	if (millis() - lastMoveTime > moveDebounce) {
		if (dir & UP) {
			menuIndex = (menuIndex - 1 + menuItems) % menuItems;
			lastMoveTime = millis();
			tone(PIN_BUZZER, 440, 200);
		} else if (dir & DOWN) {
			menuIndex = (menuIndex + 1) % menuItems;
			lastMoveTime = millis();
			tone(PIN_BUZZER, 440, 200);
		}
	}
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(BLACK);
	display.setCursor(0, 0);
	display.println("Main Menu");

	display.setCursor(10, 15);
	display.print((menuIndex == 0) ? "> " : " ");
	display.println("Start Game");

	display.setCursor(10, 25);
	display.print((menuIndex == 1) ? "> " : " ");
	display.println("Settings");

	display.display();

	if (pressJoystick()) {
		delay(500);
		Serial.println("Button pressed!");
		state = options[menuIndex];
	}

}
