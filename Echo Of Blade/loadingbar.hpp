#ifndef LOADINGBAR_HPP
#define LOADINGBAR_HPP

#include "gamestate.hpp"
#include "display.hpp"

extern int x, y;
extern int loadingBarWidth, loadingBarHeight;
extern double currentLoadingBarProgress;
extern const double updateInterval, progressPerUpdate;
extern bool isLoading;
extern clock_t lastUpdateTime;

extern int bg;
extern int screenWidth, screenHeight;
extern GameState currentState;
extern GameState loadingTargetState;

void drawLoadingBar(int x, int y) {
	iSetColor(255, 0, 0); // Red color for the loading bar
	iFilledRectangle(x, y, loadingBarWidth * currentLoadingBarProgress / 100, loadingBarHeight);

	char percentageText[100];
	sprintf_s(percentageText, "%0.2lf%%", currentLoadingBarProgress);
	iSetColor(255, 255, 255);
	iText(x + loadingBarWidth / 2 - 20, y + loadingBarHeight / 2 - 5, percentageText, GLUT_BITMAP_TIMES_ROMAN_24);
}

void displayLoadingScreen() {
	displayImages(bg, 0, 0, screenWidth, screenHeight); // Static background image

	// Draw the loading bar at the bottom right
	drawLoadingBar(screenWidth - loadingBarWidth - 755, screenHeight - loadingBarHeight - 930);
}

// to load gane in a mean of time
void loadGame() {
	clock_t currentTime = clock();
	double elapsedTime = (double)(currentTime - lastUpdateTime) / CLOCKS_PER_SEC * 1000;
	if (currentLoadingBarProgress < 100.0) {
		if (elapsedTime >= updateInterval) {
			currentLoadingBarProgress += progressPerUpdate;

			if (currentLoadingBarProgress > 100.0) {
				currentLoadingBarProgress = 100.0;
			}

			lastUpdateTime = currentTime;
		}
	}
	else {
		isLoading = false;
		currentState = loadingTargetState;
	}
}
#endif // !LOADINGBAR_HPP
