#include "utils.h"

#define CELL_SIZE 4
#define WIDTH 21	// 84 / 4
#define HEIGHT 12	// 48 / 12
#define MAX_LENGTH (WIDTH * HEIGHT)	// 48 / 12

int8_t offsetX = 1;
int8_t offsetY = 0;

uint8_t snakeX[MAX_LENGTH];
uint8_t snakeY[MAX_LENGTH];
uint16_t snakeLength = 1;

unsigned long lastGameMoveTime = 0;
const unsigned long moveInterval = 200;

bool gameInitialized = false;

void resetGame() {
	snakeLength = 1;
	snakeX[0] = WIDTH / 2;
	snakeY[0] = HEIGHT / 2;
	offsetX = 1;
	offsetY = 0;
	gameInitialized = true;
}


void handleGame() {
	if (!gameInitialized) {
		resetGame();
	}

	uint8_t dir = readJoystick();

	if (dir & UP && offsetY == 0) {
		offsetX = 0;
		offsetY = -1;
	} else if (dir & DOWN && offsetY == 0) {
		offsetX = 0;
		offsetY = 1;
	} else if (dir & LEFT && offsetX == 0) {
		offsetX = -1;
		offsetY = 0;
	} else if (dir & RIGHT && offsetX == 0) {
		offsetX = 1;
		offsetY = 0;
	}

	if (millis() - lastGameMoveTime >= moveInterval) {
		lastGameMoveTime = millis();

		for (int i = snakeLength - 1; i > 0; i--) {
			snakeX[i] = snakeX[i - 1];
			snakeY[i] = snakeY[i - 1];
		}

		snakeX[0] = (snakeX[0] + offsetX + WIDTH) % WIDTH;
		snakeY[0] = (snakeY[0] + offsetY + HEIGHT) % HEIGHT;
	}

	display.clearDisplay();
	for (int i = 0; i < snakeLength; i++) {
		display.fillRect(snakeX[i] * CELL_SIZE, snakeY[i] * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
	}

	display.display();

	if (pressJoystick()) {
		delay(500);
		state = STATE_PAUSE_MENU;
	}
}
