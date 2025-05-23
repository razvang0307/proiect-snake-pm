#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Display

#define PIN_CLK 7
#define PIN_DIN 6
#define PIN_DC 	5
#define PIN_CS 	4
#define PIN_RST 3

// Buzzer
#define PIN_BUZZER 2

// Joystick
#define PIN_PRESS 8
#define PIN_STICK_Y A0
#define PIN_STICK_X A1

#define UP 		1
#define DOWN 	2
#define LEFT 	4
#define RIGHT	8

extern Adafruit_PCD8544 display;

enum STATE {
	STATE_MAIN_MENU,
	STATE_SETTINGS,
	STATE_GAME,
	STATE_PAUSE_MENU
};

extern STATE state;

void handleMainMenu();
void handleSettings();
void handleGame();
void handlePauseMenu();
