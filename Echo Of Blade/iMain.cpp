#include "music.hpp"
#include "display.hpp"
#include "loadingbar.hpp"
#include "gamestate.hpp"
#include "gameposition.hpp"
#include "obstacle.hpp"
#include "heroPlayer.hpp"
#include "respawn.hpp"
#include "collision.hpp"
#include "miniBoss.hpp"
#include <time.h>
#include <Windows.h>
#include <stdio.h>

// ----- Game States -----
GameState currentState = MAIN_MENU;

// ----- STORY MODE -----
int storyFrames[10];
int storyFrameIndex = 0;
clock_t storyStartTime = 0;
bool showBlinkingText = false;
int playAgain;
bool arena1Completed = false;
bool showPlayAgainWarning = false;

// ----- Buttons -----
int mainMenuButtons[6];
int mainMenuButtonsHover[5];
int hoveredButtonIndex = -1;
int selectedButtonIndex = 0;
double buttonX = 809.2, buttonStartY = 557.6 - 110, buttonWidth = 346.7, buttonHeight = 92, buttonGap = 75.7;

int transitionsFrames[15];
int currenttransitionsFrame = 0;
int transitionsTimer = 0;
bool isTransitioning = false;

// ----- PopUp -----
int popUpImage;
bool popUpStart = false, popUpShown = false;
bool textBlinking = false;
double popupTriggerX = 16300;

// ----- Level Menu -----
int plainBG, levelMenu[2];
Levels arenaLevels[2];
int hoveredArena = -1;
const double hoverScale = 1.15;

// ----- Images -----
int bg, bgMainMenu, stage1, levelMenuBg, about, option, victory;
int arena2BG;
int arena2Lenght = 13500;
double backgroundOfSetX = 0.0; //to use fake parallox
const double stage1Lenght = 16500.0;

int blinkState = 0; //0: text visible and 1: text hiddden
int timeElapsed = 0;

int screenWidth = 1920;
int screenHeight = 1080;
double centerX = screenWidth / 2 - 190;
double centerY = screenHeight / 2 - 480;

// ----- Character -----
int runFrames[8], idleFrames[6], reverseRunFrames[8], reverseIdleFrames[6], deadFrames[6], attackFrames[4], reverseAttackFrames[4];
int normalJump, reverseJump, playerDead;
double realTimePositionX = 400.0;
int runFrameIndex = 0, idleFrameIndex = 0, jumpFrameIndex = 0, reverseRunFrameIndex = 0, reverseIdleFrameIndex = 0, deadFramesIndex = 0, attackFrameIndex = 0;
double runNormalSpeed = 3.6, runFastSpeed = 8.2;
double characterX = 400, characterY = 100;
double jumpVelocityX = 0.0, jumpVelocityY = 0.0;
clock_t lastFrameTime = clock();
const double frameInterval = 100.0;
const int totalJumpFrames = 0;
const double gravity = 0.5, normalJumpPower = 12.0, longJumpPower = 14.0, longJumpSpeed = 4.5, fastJumpSpeed = 6.7;
const double groundY = 100.0;
bool isRunning = false, isIdle = false, isJumping = false, isFalling = false, isDead = false, isAttacking = false, isReverseAttacking = false;
int jumpCount = 0;
const int maxJumpCount = 2;
const int totalAttackFrames = 4;
clock_t lastAttackTime = 0;
const double attackFrameInterval = 150.0;
bool canJumpNow = true;
int deathTimerMs = 0;
const int deadAnimationInterval = 3000;
const int deatAnimationFrames = 6;
const int deadAnimationTotalMs = deadAnimationInterval * deatAnimationFrames;
const int deathOverlayDelay = 12000;
double deadPositionX = 0.0, deadPositionY = 100.0;

bool deathBlinkState = true;

//mini boss
int miniBossIdle[6], miniBossWalk[12], miniBossAttack[15], miniBossTakeHit[5], miniBossDeath[22];
MiniBoss miniBoss[3];

//obstacle
int  spikeFrames[10], bombFrames[20], spearFrames[12], fireBoxFrames[10], doorFrames[14];
double startPosition = 1200;
int fireBoxWidth = 96, fireBoxHeight = 96;
int spearWidth = 48, spearHeight = 128;
int bombWidth = 96, bombHeight = 96;
int spikeWidth = 96, spikeHeight = 96;
int doorWidth = 300, doorHeight = 650;
clock_t lastSpikeDamageTime = 0;
clock_t lastFireDamageTime = 0;
clock_t lastBombDamageTime = 0;

//hero player
int playerHealth = 200;
const int maxHealth = 200;
int playerImage;
const int playerImageX = 20, playerImageY = 920;
const int playerImageWidth = 128, playerImageHeight = 128;
const int healthBarX = playerImageX + playerImageWidth + 70;
const int healthBarY = playerImageY + playerImageHeight / 2 - 70;
const int healthBarWidth = 300, healthBarHeight = 20;

//movementState
movementstate previousState = IDLE;

//loading bar variable
int x, y;
int loadingBarWidth = 400;
int loadingBarHeight = 40;
double currentLoadingBarProgress = 0.0;
clock_t lastUpdateTime = clock();
const double updateInterval = 400.0;
const double progressPerUpdate = 7.6;
bool isLoading = false;

bool showVictoryScreen = false;
bool allBossesDefeated = false;

// ==========SAVEGAME==========
bool hasSavedGame = false;
GameState savedGameState = MAIN_MENU;
double savedRealTimePositionX = 400.0;
double savedCharacterX = 400.0;
double savedCharacterY = 100.0;
double savedBackgroundOfSetX = 0.0;
int savedPlayerHealth = 200;
movementstate savedPreviousState = IDLE;
bool savedArena1Completed = false;
bool savedPopUpShown = false;

// Animation frame states
int savedRunFrameIndex = 0;
int savedIdleFrameIndex = 0;
int savedReverseRunFrameIndex = 0;
int savedReverseIdleFrameIndex = 0;
int savedAttackFrameIndex = 0;
int savedDeadFramesIndex = 0;

// Jump states
double savedJumpVelocityX = 0.0;
double savedJumpVelocityY = 0.0;
int savedJumpCount = 0;
bool savedIsJumping = false;
bool savedIsFalling = false;
bool savedIsAttacking = false;
bool savedIsReverseAttacking = false;
bool savedCanJumpNow = true;

// Death states
int savedDeathTimerMs = 0;
double savedDeadPositionX = 0.0;
double savedDeadPositionY = 100.0;

// MiniBoss states - we'll save their health and states
int savedMiniBossHealth[3];
int savedMiniBossState[3];
double savedMiniBossX[3];
double savedMiniBossY[3];

// ==========SAVEGAME FUNCTIONS==========

void saveCurrentGameState() {
	hasSavedGame = true;
	savedGameState = currentState;
	savedRealTimePositionX = realTimePositionX;
	savedCharacterX = characterX;
	savedCharacterY = characterY;
	savedBackgroundOfSetX = backgroundOfSetX;
	savedPlayerHealth = playerHealth;
	savedPreviousState = previousState;
	savedArena1Completed = arena1Completed;
	savedPopUpShown = popUpShown;

	// Save animation frame indices
	savedRunFrameIndex = runFrameIndex;
	savedIdleFrameIndex = idleFrameIndex;
	savedReverseRunFrameIndex = reverseRunFrameIndex;
	savedReverseIdleFrameIndex = reverseIdleFrameIndex;
	savedAttackFrameIndex = attackFrameIndex;
	savedDeadFramesIndex = deadFramesIndex;

	// Save jump states
	savedJumpVelocityX = jumpVelocityX;
	savedJumpVelocityY = jumpVelocityY;
	savedJumpCount = jumpCount;
	savedIsJumping = isJumping;
	savedIsFalling = isFalling;
	savedIsAttacking = isAttacking;
	savedIsReverseAttacking = isReverseAttacking;
	savedCanJumpNow = canJumpNow;

	// Save death states
	savedDeathTimerMs = deathTimerMs;
	savedDeadPositionX = deadPositionX;
	savedDeadPositionY = deadPositionY;

	// Save MiniBoss states for ARENA2
	if (currentState == ARENA2) {
		for (int i = 0; i < 3; i++) {
			savedMiniBossHealth[i] = miniBoss[i].health;
			savedMiniBossState[i] = (int)miniBoss[i].state;
			savedMiniBossX[i] = miniBoss[i].x;
			savedMiniBossY[i] = miniBoss[i].y;
		}
	}

	printf("Game saved successfully! State: %d, Position: %.2f, Health: %d\n",
		savedGameState, savedRealTimePositionX, savedPlayerHealth);
}

void loadSavedGameState() {
	if (!hasSavedGame) {
		printf("No saved game found!\n");
		return;
	}

	currentState = savedGameState;
	realTimePositionX = savedRealTimePositionX;
	characterX = savedCharacterX;
	characterY = savedCharacterY;
	backgroundOfSetX = savedBackgroundOfSetX;
	playerHealth = savedPlayerHealth;
	previousState = savedPreviousState;
	arena1Completed = savedArena1Completed;
	popUpShown = savedPopUpShown;

	// Restore animation frame indices
	runFrameIndex = savedRunFrameIndex;
	idleFrameIndex = savedIdleFrameIndex;
	reverseRunFrameIndex = savedReverseRunFrameIndex;
	reverseIdleFrameIndex = savedReverseIdleFrameIndex;
	attackFrameIndex = savedAttackFrameIndex;
	deadFramesIndex = savedDeadFramesIndex;

	// Restore jump states
	jumpVelocityX = savedJumpVelocityX;
	jumpVelocityY = savedJumpVelocityY;
	jumpCount = savedJumpCount;
	isJumping = savedIsJumping;
	isFalling = savedIsFalling;
	isAttacking = savedIsAttacking;
	isReverseAttacking = savedIsReverseAttacking;
	canJumpNow = savedCanJumpNow;

	// Restore death states
	deathTimerMs = savedDeathTimerMs;
	deadPositionX = savedDeadPositionX;
	deadPositionY = savedDeadPositionY;

	// Restore MiniBoss states for ARENA2
	if (savedGameState == ARENA2) {
		for (int i = 0; i < 3; i++) {
			miniBoss[i].health = savedMiniBossHealth[i];
			miniBoss[i].state = (EnemyState)savedMiniBossState[i];
			miniBoss[i].x = savedMiniBossX[i];
			miniBoss[i].y = savedMiniBossY[i];
		}
	}

	// Start appropriate music based on saved state
	if (savedGameState == PLAY_GAME || savedGameState == ARENA2) {
		stopStartMusic();
		playRainMusic();
	}

	printf("Game loaded successfully! State: %d, Position: %.2f, Health: %d\n",
		currentState, realTimePositionX, playerHealth);
}

void clearSavedGame() {
	hasSavedGame = false;
	savedGameState = MAIN_MENU;
	savedRealTimePositionX = 400.0;
	savedCharacterX = 400.0;
	savedCharacterY = 100.0;
	savedBackgroundOfSetX = 0.0;
	savedPlayerHealth = 200;
	savedPreviousState = IDLE;
	savedArena1Completed = false;
	savedPopUpShown = false;

	printf("Saved game cleared!\n");
}

bool checkAllBossesDefeated() {
	for (int i = 0; i < 3; i++) {
		if (miniBoss[i].state != ENEMY_DEAD) {
			return false;
		}
	}
	return true;
}

void displayHeroCharacter(double x, double y) {
	if (isAttacking) {
		if (isReverseAttacking) {
			iShowImage(x, y, 300, 300, reverseAttackFrames[attackFrameIndex]);
		}
		else {
			iShowImage(x, y, 300, 300, attackFrames[attackFrameIndex]);
		}

		// Update attack frame timing
		if ((clock() - lastAttackTime) * 1000 / CLOCKS_PER_SEC >= attackFrameInterval) {
			attackFrameIndex++;
			lastAttackTime = clock();
			if (attackFrameIndex >= 3) {  // 3 frames per attack
				isAttacking = false;
				attackFrameIndex = 0;
			}
		}
		return; // skip other animations while attacking
	}

	if (isJumping || isFalling){
		if (previousState == REVERSE_RUN || previousState == REVERSE_IDLE) iShowImage(x, y, 300, 300, reverseJump);
		else iShowImage(x, y, 300, 300, normalJump);
	}
	else{
		switch (previousState){
		case RUN:
			iShowImage(x, y, 300, 300, runFrames[runFrameIndex]);
			break;
		case REVERSE_RUN:
			iShowImage(x, y, 300, 300, reverseRunFrames[reverseRunFrameIndex]);
			break;
		case REVERSE_IDLE:
			iShowImage(x, y, 300, 300, reverseIdleFrames[reverseIdleFrameIndex]);
			break;
		case IDLE:
			iShowImage(x, y, 300, 300, idleFrames[idleFrameIndex]);
			break;

		case DEAD:
			double drawX = deadPositionX - backgroundOfSetX;
			double drawY = groundY;

			iShowImage(drawX, drawY, 300, 300, deadFrames[deadFramesIndex]);

			if (deathTimerMs >= deadAnimationTotalMs + deathOverlayDelay){
				iShowImage(0, 0, screenWidth, screenHeight, playerDead);
				bool blinkOn = ((timeElapsed / 200) % 2) == 0;

				if (blinkOn){
					iSetColor(255, 255, 255);
					iText(screenWidth / 2 - 180, screenHeight / 2 - 220, "Press Enter or Space to Continue", GLUT_BITMAP_HELVETICA_18);
				}
			}
			break;
		}
	}
}

void displayStoryMode() {
	clock_t currentTime = clock();
	double elapsedTime = (double)(currentTime - storyStartTime) / CLOCKS_PER_SEC;

	// Show the story images one by one, every 3.5 seconds
	if (elapsedTime >= 3.5 * storyFrameIndex && storyFrameIndex < 10) {
		storyFrameIndex++;
	}

	// Display current story image (if we have one to show)
	if (storyFrameIndex > 0 && storyFrameIndex <= 10) {
		int imageIndex = storyFrameIndex - 1; // Convert to 0-based index
		if (imageIndex < 10) {
			iShowImage(0, 0, screenWidth, screenHeight, storyFrames[imageIndex]);
		}
	}

	// After displaying all 10 images, show "Press Enter to Start"
	if (storyFrameIndex >= 10) {
		showBlinkingText = true;

		// Blink the text every 500ms
		if (timeElapsed % 125 == 0) { // Adjusted for smoother blinking
			blinkState = !blinkState;
		}

		if (blinkState == 0) {
			iSetColor(255, 255, 255);  // White color for the text
			iText(screenWidth / 2 - 180, screenWidth / 2 - 50, "Press Enter to Start", GLUT_BITMAP_HELVETICA_18);
		}
	}
}

void iDraw()
{
	iClear();

	if (timeElapsed % 400 == 0) {
		blinkState = !blinkState;
	}
	timeElapsed++;

	//first screen  
	if (currentState == START_SCREEN) {
		displayImages(bg, 0, 0, screenWidth, screenHeight);
		blinkText();
	}
	else if (currentState == MAIN_MENU) {
		displayMainMenu();
	}
	else if (currentState == OPTIONS) {
		iShowImage(0, 0, screenWidth, screenHeight, option);

		bool blinkOn = ((timeElapsed / 250) % 2) == 0;
		if (blinkOn) {
			iSetColor(255, 255, 255);
			iText(screenWidth / 2 - 220, 50, "Press ESC to go back to Main Menu", GLUT_BITMAP_HELVETICA_18);
		}
	}
	else if (currentState == ABOUT) {
		iShowImage(0, 0, screenWidth, screenHeight, about);

		bool blinkOn = ((timeElapsed / 250) % 2) == 0;
		if (blinkOn) {
			iSetColor(255, 255, 255);
			iText(screenWidth / 2 - 220, 50, "Press ESC to go back to Main Menu", GLUT_BITMAP_HELVETICA_18);
		}
	}
	else if (currentState == TRANSITION){
		if (isTransitioning){
			displayImages(transitionsFrames[currenttransitionsFrame], 0, 0, screenWidth, screenHeight);

			if (transitionsTimer % 65 == 0){
				currenttransitionsFrame++;
				if (currenttransitionsFrame >= 15){
					currenttransitionsFrame = 0;
					isTransitioning = false;
					currentState = MAIN_MENU;
				}
			}
			transitionsTimer++;
		}
	}

	else if (currentState == LOADING_SCREEN){
		displayLoadingScreen();
		loadGame();
	}
	else if (currentState == STORY_MODE) {
		displayStoryMode();
	}
	else if (currentState == PLAY_GAME){
		for (int i = 0; i <= stage1Lenght / screenWidth; i++){
			double drawX = i * screenWidth - backgroundOfSetX;

			if (drawX + screenWidth >= 0 && drawX <= screenWidth){
				iShowImage(drawX, 0, screenWidth, screenHeight, stage1);
			}
		}

		drawObstacle();
		drawPlayerUI();
		displayHeroCharacter(characterX, characterY);

		// Show ESC instruction
		iSetColor(255, 255, 255);
		iText(20, screenHeight - 30, "Press ESC to return to Main Menu", GLUT_BITMAP_HELVETICA_12);
	}
	else if (currentState == VICTORY) {
		iShowImage(0, 0, screenWidth, screenHeight, victory); // Display victory image
		bool blinkOn = ((timeElapsed / 150) % 2) == 0;
		if (blinkOn) {
			iSetColor(255, 255, 255);
			iText(screenWidth / 2 - 180, screenHeight / 2 - 220, "Press Enter to Exit", GLUT_BITMAP_HELVETICA_18);
		}
	}

	else if (currentState == ARENA2){
		for (int i = 0; i <= arena2Lenght / screenWidth; i++){
			double drawX = i * screenWidth - backgroundOfSetX;

			if (drawX + screenWidth >= 0 && drawX <= screenWidth){
				iShowImage(drawX, 0, screenWidth, screenHeight, arena2BG);
			}
		}

		// Display the character
		displayHeroCharacter(characterX, characterY);
		drawPlayerUI();
		for (int i = 0; i<3; i++) drawMiniBoss(miniBoss[i]);

		// Show ESC instruction
		iSetColor(255, 255, 255);
		iText(20, screenHeight - 30, "Press ESC to return to Main Menu", GLUT_BITMAP_HELVETICA_12);

		if (realTimePositionX >= 6800) {
			iShowImage(0, 0, screenWidth, screenHeight, victory);
			currentState = VICTORY;
		}
	}
	else if (currentState == LEVEL_MENU){
		drawLevelMenu();
	}

	if (showPlayAgainWarning) {
		iShowImage(0, 0, screenWidth, screenHeight, playAgain);
		bool blinkOn = ((timeElapsed / 150) % 2) == 0;
		if (blinkOn) {
			iSetColor(255, 255, 255);
			iText(screenWidth / 2 - 200, screenHeight / 2 - 100,
				"Press ENTER to play again or ESC to go back", GLUT_BITMAP_HELVETICA_18);
		}
	}

	if (popUpStart){
		iShowImage(0, 0, screenWidth, screenHeight, popUpImage);

		bool blinkOn = ((timeElapsed / 100) % 2) == 0;
		if (blinkOn){
			iSetColor(255, 255, 255);
			iText(screenWidth / 2 - 180, screenHeight / 2 - 220, "Press ENTER to change between realms", GLUT_BITMAP_HELVETICA_18);
		}
	}

}

void iMouseMove(int mx, int my) {
	//main menu
	if (currentState == MAIN_MENU){
		hoveredButtonIndex = -1;
		double buttonY[5] = { 590, 554, 475, 396, 317 };

		if ((mx >= buttonX - 160 && mx <= buttonX + buttonWidth - 240)) {
			if (my >= buttonY[0] && my <= buttonY[0] + buttonHeight) hoveredButtonIndex = 0;
			else if (my >= buttonY[1] && my <= buttonY[1] + buttonHeight) hoveredButtonIndex = 1;
			else if (my >= buttonY[2] && my <= buttonY[2] + buttonHeight) hoveredButtonIndex = 2;
			else if (my >= buttonY[3] && my <= buttonY[3] + buttonHeight) hoveredButtonIndex = 3;
			else if (my >= buttonY[4] && my <= buttonY[4] + buttonHeight) hoveredButtonIndex = 4;
		}
	}

	//level menu
	if (currentState == LEVEL_MENU){
		hoveredArena = -1;

		const int arenaWidth = 260;
		const int arenaHeight = 595;

		int arenaX[2] = { 333, 930 };
		int arenaY[2] = { 373, 373 };


		for (int i = 0; i < 2; i++){

			if ((mx >= arenaX[i] && mx <= arenaX[i] + arenaWidth) &&
				(my >= arenaY[i] && my <= arenaY[i] + arenaHeight)){
				hoveredArena = i;
				break;
			}
		}
	}
}

void iPassiveMouseMove(int mx, int my){
	iMouseMove(mx, my);
}

void resetPlayerPosition() {
	realTimePositionX = 400.0;
	characterX = 400;
	characterY = 100;
	backgroundOfSetX = 0.0;
	jumpVelocityX = 0.0;
	jumpVelocityY = 0.0;
	jumpCount = 0;
	isJumping = false;
	isFalling = false;
	isAttacking = false;
	isReverseAttacking = false;
	previousState = IDLE;
	runFrameIndex = 0;
	idleFrameIndex = 0;
	reverseRunFrameIndex = 0;
	reverseIdleFrameIndex = 0;
	attackFrameIndex = 0;
}

void checkPlayerAttackOnBosses() {
	const double attackRange = 200.0;
	const int playerAttackDamage = 25;

	for (int i = 0; i < 3; i++) {
		MiniBoss &boss = miniBoss[i];
		if (boss.state == ENEMY_DEAD) continue;
		double distance = fabs(realTimePositionX - boss.x);

		if (distance <= attackRange) {
			playerHitMiniBoss(boss, playerAttackDamage);
			printf("Player hit MiniBoss %d! Boss health: %d\n", i, boss.health);

			break;
		}
	}
}

void iMouse(int button, int state, int mx, int my){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){

		if (currentState == LEVEL_MENU) {
			if (hoveredArena == 0) {
				if (arena1Completed) {
					showPlayAgainWarning = true;
				}
				else {
					resetPlayerPosition();
					playerHealth = maxHealth;
					loadingTargetState = PLAY_GAME;
					currentLoadingBarProgress = 0.0;
					lastUpdateTime = clock();
					isLoading = true;
					currentState = LOADING_SCREEN;
				}
			}
			else if (hoveredArena == 1) {
				resetPlayerPosition();
				loadingTargetState = ARENA2;
				currentLoadingBarProgress = 0.0;
				lastUpdateTime = clock();
				isLoading = true;
				currentState = LOADING_SCREEN;
			}
		}

		if (currentState == ARENA2) {
			if (!isAttacking) {  // Prevent starting attack again while current attack is running
				isAttacking = true;

				if (previousState == REVERSE_RUN || previousState == REVERSE_IDLE) {
					isReverseAttacking = true;
				}
				else {
					isReverseAttacking = false;
				}

				attackFrameIndex = 0;
				lastAttackTime = clock();

				checkPlayerAttackOnBosses();
			}
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){

	}

}

clock_t lastKeyPressTime = 0;
const double keyPressDelay = 150.0;
double highestY = 0.0;

void beingDead(){
	if (previousState == DEAD) return;

	previousState = DEAD;
	deadFramesIndex = 0;
	deathTimerMs = 0;

	deadPositionX = realTimePositionX;
	deadPositionY = characterY;

	isJumping = false;
	isFalling = false;
}

void movement(){
	clock_t currentTime = clock();
	double elapsedTime = (double)(currentTime - lastFrameTime) / CLOCKS_PER_SEC * 1000;

	movementstate currentAnimState = IDLE;
	if (playerHealth <= 0 && previousState != DEAD){
		beingDead();
	}
	if (previousState == DEAD){
		deathTimerMs += (int)elapsedTime;

		int idx = deathTimerMs / deadAnimationInterval;
		if (idx < 0) idx = 0;
		if (idx >= deatAnimationFrames) idx = deatAnimationFrames - 1;
		deadFramesIndex = idx;


		if (deathTimerMs >= deadAnimationTotalMs + deathOverlayDelay){
			if (GetAsyncKeyState(VK_SPACE) & 0x8000 || GetAsyncKeyState(13) & 0x8000){
				respawnPlayer();
				previousState = IDLE;
				return;
			}
		}
		return;
	}

	double arenaLength = (currentState == ARENA2) ? arena2Lenght : stage1Lenght;
	if (!isAttacking) {
		if ((GetAsyncKeyState('W') & 0x8000) && (GetAsyncKeyState(VK_SHIFT) & 0x8000)) {
			currentAnimState = RUN;
			realTimePositionX += runFastSpeed;

			if (realTimePositionX > arenaLength) {
				realTimePositionX = arenaLength;
			}

			if (characterX < 1300) characterX += runFastSpeed;
			else if (backgroundOfSetX < arenaLength) backgroundOfSetX += runFastSpeed;

			if (elapsedTime >= frameInterval - 40.0){
				runFrameIndex = (runFrameIndex + 1) % 8;
				lastFrameTime = currentTime;
			}
		}

		else if (GetAsyncKeyState('W') & 0x8000) {

			currentAnimState = RUN;
			realTimePositionX += runNormalSpeed;

			if (realTimePositionX > arenaLength) {
				realTimePositionX = arenaLength;
			}

			if (characterX < 1300) characterX += runNormalSpeed;
			else if (backgroundOfSetX < arenaLength) backgroundOfSetX += runNormalSpeed;

			if (elapsedTime >= frameInterval - 20.0) {
				runFrameIndex = (runFrameIndex + 1) % 8;
				lastFrameTime = currentTime;
			}
		}

		else if (GetAsyncKeyState('A') & 0x8000){
			currentAnimState = REVERSE_RUN;

			double speed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) ? runFastSpeed : runNormalSpeed;

			if (characterX > 200)characterX -= speed;
			else if (backgroundOfSetX > 0) backgroundOfSetX -= speed;
			realTimePositionX -= speed;

			if (realTimePositionX < 0) {
				realTimePositionX = 0;
			}

			if (elapsedTime >= frameInterval - 20.0) {
				reverseRunFrameIndex = (reverseRunFrameIndex + 1) % 8;
				lastFrameTime = currentTime;
			}
		}

		else if (previousState == REVERSE_RUN || previousState == REVERSE_IDLE){
			currentAnimState = REVERSE_IDLE;
			if (elapsedTime >= frameInterval + 200.0) {
				reverseIdleFrameIndex = (reverseIdleFrameIndex + 1) % 6;
				lastFrameTime = currentTime;
			}
		}

		else{

			currentAnimState = IDLE;
			if (elapsedTime >= frameInterval + 200.0) {
				idleFrameIndex = (idleFrameIndex + 1) % 6;
				lastFrameTime = currentTime;
			}
		}
	}


	if (currentAnimState != previousState){

		if (currentAnimState == IDLE) idleFrameIndex = 0;
		else if (currentAnimState == RUN) runFrameIndex = 0;
		else if (currentAnimState == REVERSE_RUN) reverseRunFrameIndex = 0;
		else if (currentAnimState == REVERSE_IDLE) reverseIdleFrameIndex = 0;


		previousState = currentAnimState;
	}
}


GameState loadingTargetState = PLAY_GAME;
void fixedUpdate() {
	clock_t currentTime = clock();
	double elapsedTime = (double)(currentTime - lastFrameTime) / CLOCKS_PER_SEC * 1000;

	if (currentState == START_SCREEN) {
		if (isKeyPressed(' ') || isKeyPressed(13)) {
			currentState = TRANSITION;
			isTransitioning = true;
			currenttransitionsFrame = 0;
			transitionsTimer = 0;

			playEnter();
			playStartMusic();
		}
	}

	else if (currentState == MAIN_MENU) {
		if (isKeyPressed(13)) {
			playEnter();

			switch (selectedButtonIndex) {
			case 0: // Play Game
				// Clear any previous save and start fresh
				clearSavedGame();
				loadingTargetState = STORY_MODE;
				currentLoadingBarProgress = 0.0;
				lastUpdateTime = clock();
				isLoading = true;
				currentState = LOADING_SCREEN;
				stopStartMusic();
				playRainMusic();
				break;

			case 1: 
				if (hasSavedGame) {
					loadingTargetState = savedGameState;
					currentLoadingBarProgress = 0.0;
					lastUpdateTime = clock();
					isLoading = true;
					currentState = LOADING_SCREEN;
					stopStartMusic();
					playRainMusic();
				}
				else {
					
					clearSavedGame();
					loadingTargetState = STORY_MODE;
					currentLoadingBarProgress = 0.0;
					lastUpdateTime = clock();
					isLoading = true;
					currentState = LOADING_SCREEN;
					stopStartMusic();
					playRainMusic();
				}
				break;

			case 2:
				currentState = OPTIONS;
				break;

			case 3:
				currentState = ABOUT;
				break;

			case 4:
				exit(0); 
				break;
			}
		}
	}

	else if (currentState == OPTIONS) {
		if (isKeyPressed(27)) { // ESC key
			currentState = MAIN_MENU;
			playEnter();
		}
	}

	// Handle ABOUT state
	else if (currentState == ABOUT) {
		if (isKeyPressed(27)) { // ESC key
			currentState = MAIN_MENU;
			playEnter();
		}
	}

	else if (currentState == STORY_MODE) {
		// Initialize story mode if not already started
		if (storyStartTime == 0) {
			storyStartTime = clock();
			storyFrameIndex = 0;
			showBlinkingText = false;
		}

		// Check if Enter key is pressed after all images are shown
		if (showBlinkingText && isKeyPressed(13)) {
			// Reset story mode variables for next time
			storyStartTime = 0;
			storyFrameIndex = 0;
			showBlinkingText = false;

			currentState = PLAY_GAME;
			stopStartMusic();  // Stop any background music for the story
			playRainMusic();   // Start the background music for the game
		}
	}

	// Handle ESC key during gameplay to save and return to main menu
	else if (currentState == PLAY_GAME || currentState == ARENA2) {
		if (isKeyPressed(27)) { // ESC key
			saveCurrentGameState();
			stopRainMusic();
			playStartMusic();
			currentState = MAIN_MENU;
			selectedButtonIndex = 1; // Highlight Continue button
			hoveredButtonIndex = 1;
			printf("Returned to main menu. Game saved.\n");
		}
	}

	if (showPlayAgainWarning) {
		if (isKeyPressed(13)) {  // Enter key - play again
			showPlayAgainWarning = false;
			resetPlayerPosition();
			playerHealth = maxHealth;  // Full health restore
			// Reset story mode variables to skip story
			storyStartTime = 0;
			storyFrameIndex = 0;
			showBlinkingText = false;

			loadingTargetState = PLAY_GAME;
			currentLoadingBarProgress = 0.0;
			lastUpdateTime = clock();
			isLoading = true;
			currentState = LOADING_SCREEN;
		}
		else if (isKeyPressed(27)) {  // ESC key - go back to level menu
			showPlayAgainWarning = false;
			// Stay in LEVEL_MENU state
		}
		return;  // Don't process other game logic while warning is shown
	}

	//Menu naviagtion  
	if (currentState == MAIN_MENU) {
		clock_t currentTime = clock();
		double elapsedTime = (double)(currentTime - lastKeyPressTime) / CLOCKS_PER_SEC * 1000;

		if (isSpecialKeyPressed(GLUT_KEY_DOWN) && elapsedTime >= keyPressDelay) {
			selectedButtonIndex = (selectedButtonIndex + 1) % 5;
			hoveredButtonIndex = selectedButtonIndex;

			lastKeyPressTime = currentTime;
		}
		else if (isSpecialKeyPressed(GLUT_KEY_UP) && elapsedTime >= keyPressDelay) {
			selectedButtonIndex = (selectedButtonIndex - 1 + 5) % 5;
			hoveredButtonIndex = selectedButtonIndex;

			lastKeyPressTime = currentTime;
		}
	}

	//handling pop-up
	if (popUpStart && isKeyPressed(13)){
		popUpStart = false;
		popUpShown = true;
		loadingTargetState = LEVEL_MENU;
		currentLoadingBarProgress = 0.0;
		lastUpdateTime = clock();
		isLoading = true;

		currentState = LOADING_SCREEN;
		return;
	}


	//main game movement  
	else if (currentState == PLAY_GAME) {
		movement();

		handleCollision();
		updateObstacle();
	}
	else if (currentState == ARENA2){
		movement();
		updateMiniBosses(realTimePositionX, characterY);  // <<< use world X
		for (int i = 0; i < 3; ++i) miniBossAttackPlayer(miniBoss[i], playerHealth);
	}

	// Handle loading screen completion
	else if (currentState == LOADING_SCREEN) {
		// When loading completes, check if we need to restore saved state
		if (!isLoading) {
			if (loadingTargetState == savedGameState && hasSavedGame) {
				loadSavedGameState();
			}
		}
	}

	//jumpimg
	if (jumpCount == 0 && (GetAsyncKeyState(VK_SPACE) && 0x8000) && canJumpNow){
		isJumping = true;

		highestY = characterY;
		printf("Jump started at X: %.2f\n", realTimePositionX);

		if ((GetAsyncKeyState('w') & 0x8000) && (GetAsyncKeyState(VK_SPACE) & 0x8000)){
			jumpVelocityY = 11.5;
			jumpVelocityX = fastJumpSpeed;
		}
		else if (GetAsyncKeyState('w') & 0x8000){
			jumpVelocityY = 11.5;
			jumpVelocityX = longJumpSpeed;
		}
		else{
			jumpVelocityY = 11.5;
			jumpVelocityX = 0;
		}

		jumpCount = 1;
		canJumpNow = false;
	}

	if (jumpCount == 1 && (GetAsyncKeyState(VK_SPACE) && 0x8000) && canJumpNow && isFalling){
		isJumping = true;
		jumpVelocityY = 10.5;
		if (GetAsyncKeyState('w') & 0x8000){
			jumpVelocityX = longJumpSpeed;
		}
		else{
			jumpVelocityX = 0.0;
		}

		jumpCount = 2;
		canJumpNow = false;
	}

	if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)){
		canJumpNow = true;
	}

	//handiling jump movement
	if (isJumping || isFalling){
		characterY += jumpVelocityY;
		double arenaLength = (currentState == ARENA2) ? arena2Lenght : stage1Lenght;
		if (realTimePositionX + jumpVelocityX <= arenaLength) realTimePositionX += jumpVelocityX;
		else jumpVelocityX = 0;

		if (characterY > highestY) {
			highestY = characterY;
		}

		if (backgroundOfSetX < arenaLength) backgroundOfSetX += jumpVelocityX;

		jumpVelocityY -= gravity;

		//landing
		if (characterY <= groundY){
			characterY = groundY;
			isJumping = false;
			isFalling = false;
			jumpVelocityY = 0.0;
			jumpVelocityX = 0.0;
			jumpCount = 0;
			printf("realX: %lf", realTimePositionX);
			printf("Highest Y reached during jump: %.2f\n", highestY);
		}
		else if (jumpVelocityY < 0){
			isJumping = false;
			isFalling = true;
		}


	}

	double arenaLength = (currentState == ARENA2) ? arena2Lenght : stage1Lenght;

	if (realTimePositionX > arenaLength) realTimePositionX = arenaLength;
	if (backgroundOfSetX > arenaLength) backgroundOfSetX = arenaLength;

	if (realTimePositionX < 0) realTimePositionX = 0;
	if (backgroundOfSetX < 0) backgroundOfSetX = 0;

	if (previousState != DEAD){
		updateCheckPoints();
	}

}

int main(){
	iInitialize(screenWidth, screenHeight, "BLADE: THE LONE WARRIOR");
	Load();
	generateObstacle();
	iStart();
	return 0;
}