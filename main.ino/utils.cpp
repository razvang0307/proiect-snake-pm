#include "utils.h"

#define THRESHOLD_TOP 500
#define THRESHOLD_BOT 150

uint8_t readJoystick() {
	uint8_t dir = 0;
	int y = analogRead(PIN_STICK_Y);
	int x = analogRead(PIN_STICK_X);
	if (y > THRESHOLD_TOP ) dir |= DOWN;
	else if (y < THRESHOLD_BOT) dir |= UP;
	if (x > THRESHOLD_TOP) dir |= LEFT;
	else if (x < THRESHOLD_BOT) dir |= RIGHT;
	return dir;
}

bool pressJoystick() {
	return digitalRead(PIN_PRESS) == LOW;
}
