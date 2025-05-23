#include "utils.h"

#define CELL_SIZE 4
#define WIDTH 21	// 84 / 4
#define HEIGHT 12	// 48 / 12
#define MAX_LENGTH (WIDTH * HEIGHT)	// 48 / 12

// Notes

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


int8_t offsetX = 1;
int8_t offsetY = 0;
int8_t nextoffsetX = 1;
int8_t nextoffsetY = 0;

uint8_t snakeX[MAX_LENGTH];
uint8_t snakeY[MAX_LENGTH];
uint16_t snakeLength = 1;

unsigned long lastGameMoveTime = 0;
const unsigned long moveInterval = 300;

uint8_t foodX, foodY;
bool foodEaten = false;

bool gameInitialized = false;

const uint16_t melody[] = {

  // Pacman
  // Score available at https://musescore.com/user/85429/scores/107109
  NOTE_B4, NOTE_B5,  NOTE_FS5,  NOTE_DS5, //1
  NOTE_B5, NOTE_FS5,  NOTE_DS5, NOTE_C5,
  NOTE_C6, NOTE_G6,  NOTE_E6,   NOTE_C6, NOTE_G6, NOTE_E6,

  NOTE_B4,  NOTE_B5,   NOTE_FS5,   NOTE_DS5,  NOTE_B5,  //2
  NOTE_FS5, NOTE_DS5, NOTE_DS5, NOTE_E5,  NOTE_F5,
  NOTE_F5,  NOTE_FS5,  NOTE_G5,  NOTE_G5, NOTE_GS5,  NOTE_A5, NOTE_B5
};

const int16_t dividers[] = {
	16, 16, 16, 16,
	32, -16, 8, 16,
	16, 16, 32, -16, 8,

	16, 16, 16, 16, 32,
	-16, 8, 32, 32, 32,
	32, 32, 32, 32, 16, 8
};

const size_t melodyLength = sizeof(melody) / sizeof(melody[0]);

size_t currentNote = 0;

void playNextNote() {
	uint16_t note = melody[currentNote];
	int divider = dividers[currentNote];
	uint16_t noteDuration = moveInterval * 8;
	if (divider > 0)
		noteDuration /= divider;
	else {
		noteDuration /= (abs(divider));
		noteDuration *= 1.5;
	}
	if (note > 0) {
		tone(PIN_PIEZO, note, 0.9 * noteDuration);
	} else {
		noTone(PIN_PIEZO);
	}

	currentNote = (currentNote + 1) % melodyLength;
}

void spawnFood() {
	bool valid = false;
	while (!valid) {
		foodX = random(WIDTH);
		foodY = random(HEIGHT);
		valid = true;
		for (int i = 0; i < snakeLength; i++) {
			if (snakeX[i] == foodX && snakeY[i] == foodY) {
				valid = false;
				break;
			}
		}
	}
	foodEaten = false;
}


void resetGame() {
	snakeLength = 1;
	snakeX[0] = WIDTH / 2;
	snakeY[0] = HEIGHT / 2;
	offsetX = 1;
	offsetY = 0;
	randomSeed(analogRead(A3));
	spawnFood();
	gameInitialized = true;
}

void collideFood() {
	if (snakeX[0] == foodX && snakeY[0] == foodY) {
		if (snakeLength < MAX_LENGTH) {
			snakeX[snakeLength] = snakeX[snakeLength - 1];
			snakeY[snakeLength] = snakeY[snakeLength - 1];
			snakeLength++;
		}
		foodEaten = true;
	}
}


void handleGame() {
	if (!gameInitialized) {
		resetGame();
	}

	uint8_t dir = readJoystick();

	if (dir & UP && offsetY == 0) {
		nextoffsetX = 0;
		nextoffsetY = -1;
	} else if (dir & DOWN && offsetY == 0) {
		nextoffsetX = 0;
		nextoffsetY = 1;
	} else if (dir & LEFT && offsetX == 0) {
		nextoffsetX = -1;
		nextoffsetY = 0;
	} else if (dir & RIGHT && offsetX == 0) {
		nextoffsetX = 1;
		nextoffsetY = 0;
	}

	if (millis() - lastGameMoveTime >= moveInterval) {
		lastGameMoveTime = millis();
		offsetY = nextoffsetY;
		offsetX = nextoffsetX;

		for (int i = snakeLength - 1; i > 0; i--) {
			snakeX[i] = snakeX[i - 1];
			snakeY[i] = snakeY[i - 1];
		}

		snakeX[0] = (snakeX[0] + offsetX + WIDTH) % WIDTH;
		snakeY[0] = (snakeY[0] + offsetY + HEIGHT) % HEIGHT;
		/*

		tone(PIN_PIEZO, song[current_song_index++], moveInterval * 0.8);
		if (current_song_index >= song_length) {
			current_song_index = 0;
		}
		*/
		for (int i = 1; i < snakeLength; i++) {
			if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
				state = STATE_GAME_OVER;
				gameInitialized = false;
				return;
			}
		}
		if (music)
			playNextNote();
	}

	display.clearDisplay();
	for (int i = 0; i < snakeLength; i++) {
		display.fillRect(snakeX[i] * CELL_SIZE, snakeY[i] * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
	}
	collideFood();
	if (!foodEaten)
		display.fillRect(foodX * CELL_SIZE, foodY * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
	else
		spawnFood();


	display.display();

	if (pressJoystick()) {
		tone(PIN_BUZZER, 440, 200);
		delay(500);
		state = STATE_PAUSE_MENU;
	}
}
