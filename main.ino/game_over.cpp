#include "common.h"
#include "utils.h"

const int gameOverMenuItems = 2;
int gameOverIndex = 0;
STATE gameOverOptions[2] = {STATE_GAME, STATE_MAIN_MENU};
unsigned long gameOverLastMoveTime = 0;
const unsigned long gameOverMoveDebounce = 200;

extern bool gameInitialized;

void handleGameOver() {
	uint8_t dir = readJoystick();
	if (millis() - gameOverLastMoveTime > gameOverMoveDebounce) {
		if (dir & UP) {
			gameOverIndex = (gameOverIndex - 1 + gameOverMenuItems) % gameOverMenuItems;
			gameOverLastMoveTime = millis();
			tone(PIN_BUZZER, 440, 200);
		} else if (dir & DOWN) {
			gameOverIndex = (gameOverIndex + 1) % gameOverMenuItems;
			gameOverLastMoveTime = millis();
			tone(PIN_BUZZER, 440, 200);
		}
	}
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(BLACK);
	display.setCursor(0, 0);
	display.println("Game Over!");

	display.setCursor(10, 15);
	display.print((gameOverIndex == 0) ? "> " : " ");
	display.println("Restart");

	display.setCursor(10, 25);
	display.print((gameOverIndex == 1) ? "> " : " ");
	display.println("Main Menu");

	display.display();

	if (pressJoystick()) {
		delay(150);
		tone(PIN_BUZZER, 220, 200);
		state = gameOverOptions[gameOverIndex];
	}
}
