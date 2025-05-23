#include "common.h"
#include "utils.h"

const int pauseMenuItems = 3;
int pauseMenuIndex = 0;
STATE pause_options[3] = {STATE_GAME, STATE_GAME, STATE_MAIN_MENU};
unsigned long pauseLastMoveTime = 0;
const unsigned long pauseMoveDebounce = 200;

extern bool gameInitialized;

void handlePauseMenu() {
	uint8_t dir = readJoystick();
	if (millis() - pauseLastMoveTime > pauseMoveDebounce) {
		if (dir & UP) {
			pauseMenuIndex = (pauseMenuIndex - 1 + pauseMenuItems) % pauseMenuItems;
			pauseLastMoveTime = millis();
			tone(PIN_BUZZER, 440, 200);
		} else if (dir & DOWN) {
			pauseMenuIndex = (pauseMenuIndex + 1) % pauseMenuItems;
			pauseLastMoveTime = millis();
			tone(PIN_BUZZER, 440, 200);
		}
	}
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(BLACK);
	display.setCursor(0, 0);
	display.println("Paused");

	display.setCursor(10, 15);
	display.print((pauseMenuIndex == 0) ? "> " : " ");
	display.println("Resume");

	display.setCursor(10, 25);
	display.print((pauseMenuIndex == 1) ? "> " : " ");
	display.println("Restart");

	display.setCursor(10, 35);
	display.print((pauseMenuIndex == 2) ? "> " : " ");
	display.println("Main Menu");

	display.display();

	if (pressJoystick()) {
		delay(150);
		tone(PIN_BUZZER, 220, 200);
		if (pauseMenuIndex >= 1) {
			gameInitialized = false;
		}
		state = pause_options[pauseMenuIndex];
	}
}
