#ifndef POPUP_HPP
#define POPUP_HPP

#include "gamestate.hpp"
#include "display.hpp"
#include <Windows.h>

enum popUpPhase{
	POP_NONE,
	POP_VISIBLE,
	POP_FADING_OUT,
	POP_FADING_IN
};
extern popUpPhase popupPhase;
extern int fadeAlpha;
extern int popUpImage;

extern GameState nextStateOnPopup;

extern bool enterWasDown;
extern int screenWidth, screenHeight;
extern int timeElapsed;

extern bool fadeOut, fadeIn;
extern bool popUpStart, textBlinking;

//initialization
inline void initPopupSystem() {
	popupPhase = POP_NONE;
	fadeAlpha = 0;
	popUpImage = -1;
	enterWasDown = false;
	nextStateOnPopup = LEVEL_MENU;
}

inline void triggerPopup(int imageId, GameState nextState) {
	popUpImage = imageId;
	nextStateOnPopup = nextState;
	popupPhase = POP_VISIBLE;
	fadeAlpha = 0;
}


void updatePopup(GameState &currentState) {
	// Fade-in effect
	if (popUpStart && !textBlinking && fadeAlpha < 255) {
		fadeAlpha += 5; // increase opacity
		if (fadeAlpha >= 255) fadeAlpha = 255;
	}

	// If popup is fully shown and Enter pressed → start fade-out
	if (popUpStart && isKeyPressed(13)) {
		popUpStart = false;     // stop showing popup
		textBlinking = true;    // enable blinking text
	}

	// Handle blinking text phase
	if (textBlinking && isKeyPressed(13)) {
		fadeOut = true;
		textBlinking = false;
	}

	// Fade-out effect
	if (fadeOut) {
		fadeAlpha -= 5; // decrease opacity
		if (fadeAlpha <= 0) {
			fadeOut = false;
			fadeIn = true;
		}
	}

	// Fade-in to LEVEL_MENU
	if (fadeIn) {
		fadeAlpha += 5;
		if (fadeAlpha >= 255) {
			fadeIn = false;
			currentState = LEVEL_MENU; // switch game state
		}
	}
}


inline void drawPopup() {
	if (popupPhase == POP_VISIBLE && popUpImage > 0) {

		// Popup image
		iShowImage(0, 0, screenWidth, screenHeight, popUpImage);

		// Blinking text under popup
		bool blinkOn = ((timeElapsed / 200) % 2) == 0;
		if (blinkOn) {
			iSetColor(255, 255, 255);
			iText(screenWidth / 2 - 180, screenHeight / 2 - 220, "Press ENTER to change between realms", GLUT_BITMAP_HELVETICA_18);
		}
	}

	// Draw fade overlay
	if (fadeAlpha > 0) {
		int c = 255 - fadeAlpha;
		iSetColor(c, c, c);
		iFilledRectangle(0, 0, screenWidth, screenHeight);
	}
}

#endif // POPUP_HPP