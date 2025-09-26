#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "iGraphics.h"
#include "miniBoss.hpp"
//#include "obstacle.hpp"

extern int screenWidth, screenHeight;
extern int bg, bgMainMenu, stage1, playerDead, levelMenuBg, about, option, victory;
extern int mainMenuButtons[6], mainMenuButtonsHover[5], transitionsFrames[15];
extern int hoveredButtonIndex, selectedButtonIndex;
extern double centerX, centerY, buttonX, buttonStartY, buttonWidth, buttonHeight, buttonGap;
extern int blinkState, timeElapsed;
extern int currentFrame;
extern int runFrames[8], idleFrames[6], jumpFrames[9], reverseRunFrames[8], reverseIdleFrames[6], deadFrames[6], attackFrames[4], reverseAttackFrames[4];
extern int normalJump, reverseJump;

//pop up
extern int popUpImage;
extern const double groundY;


//for obstacle
extern int  spikeFrames[10], bombFrames[20], spearFrames[12], fireBoxFrames[10], doorFrames[14];

//LEVEL_MENU
extern int plainBG;
extern int levelMenu[2];
struct Levels {
	int x, y, w, h;
};
extern Levels arenaLevels[2];
extern int hoveredArena;
extern const double hoverScale;

//hero player
extern int playerImage;

//ARENA 2
extern int arena2BG;
extern int arena2Lenght;

// ----- STORY MODE -----
extern int storyFrames[10];
extern int playAgain;

void displayImages(int imageID, double x, double y, double width, double height) {
	iShowImage(x, y, width, height, imageID);
}


void Load()
{
	//Backgrounf for first interface
	bg = iLoadImage("Images/Background/Background1.png");
	stage1 = iLoadImage("Images/Background/stage11.png");
	about = iLoadImage("Images/Background/About.png");
	option = iLoadImage("Images/Background/Option.png");
	victory = iLoadImage("Images/Background/victory.png");

	//transitionsFrames
	char fileName[100];
	for (int i = 0; i < 15; i++){
		sprintf_s(fileName, "Images/Transitions/%d.png", i + 1);
		transitionsFrames[i] = iLoadImage(fileName);
	}

	//pop up image
	popUpImage = iLoadImage("Images/PopUP/Door.png");
	printf("Pop-up image loaded with ID: %d\n", popUpImage);

	//for main menu
	bgMainMenu = iLoadImage("Images/Background/Background1.png");

	for (int i = 0; i < 5; i++){
		sprintf_s(fileName, "Images/Button%d.png", i + 1);
		mainMenuButtons[i] = iLoadImage(fileName);

		sprintf_s(fileName, "Images/HoverButton%d.png", i + 1);
		mainMenuButtonsHover[i] = iLoadImage(fileName);
	}


	for (int i = 0; i < 8; i++){
		sprintf_s(fileName, "Images/Run/sprite_%d.png", i);
		runFrames[i] = iLoadImage(fileName);
	}
	for (int i = 0; i < 6; i++){
		sprintf_s(fileName, "Images/Idle/sprite_%d.png", i);
		idleFrames[i] = iLoadImage(fileName);
	}
	for (int i = 0; i < 8; i++){
		sprintf_s(fileName, "Images/reverseRun/%d.png", i + 1);
		reverseRunFrames[i] = iLoadImage(fileName);
	}
	for (int i = 0; i < 6; i++){
		sprintf_s(fileName, "Images/reverseIdle/%d.png", i + 1);
		reverseIdleFrames[i] = iLoadImage(fileName);
	}
	for (int i = 0; i < 6; i++){
		sprintf_s(fileName, "Images/Dead/%d.png", i + 1);
		deadFrames[i] = iLoadImage(fileName);
	}
	for (int i = 0; i < 4; i++){
		sprintf_s(fileName, "Images/Attack/%d.png", i + 1);
		attackFrames[i] = iLoadImage(fileName);
	}
	for (int i = 0; i < 4; i++){
		sprintf_s(fileName, "Images/reverseAttack/%d.png", i + 1);
		reverseAttackFrames[i] = iLoadImage(fileName);
	}

	normalJump = iLoadImage("Images/Idle/sprite_0.png");
	reverseJump = iLoadImage("Images/reverseIdle/1.png");
	playerDead = iLoadImage("Images/Background/playerDead.png");


	//for obstacle
	for (int i = 0; i < 10; i++){
		sprintf_s(fileName, "Images/FireBox/%d.png", i + 1);
		fireBoxFrames[i] = iLoadImage(fileName);
	}
	for (int i = 0; i < 20; i++){
		sprintf_s(fileName, "Images/Bomb/%d.png", i);
		bombFrames[i] = iLoadImage(fileName);
	}
	for (int i = 0; i < 12; i++){
		sprintf_s(fileName, "Images/Spear/%d.png", i + 1);
		spearFrames[i] = iLoadImage(fileName);
	}
	for (int i = 0; i < 10; i++){
		sprintf_s(fileName, "Images/Spike/sprite_0%d.png", i);
		spikeFrames[i] = iLoadImage(fileName);
	}
	for (int i = 0; i < 14; i++){
		sprintf_s(fileName, "Images/Door/%d.png", i + 1);
		doorFrames[i] = iLoadImage(fileName);
	}

	// ----- STORY MODE -----
	for (int i = 0; i < 10; i++){
		sprintf_s(fileName, "Images/Story/%d.png", i + 1);
		storyFrames[i] = iLoadImage(fileName);
	}

	//LEVEL_MENU
	plainBG = iLoadImage("Images/Background/plainBG.png");
	for (int i = 0; i < 2; i++){
		sprintf_s(fileName, "Images/levelMenu/%d.png", i + 1);
		levelMenu[i] = iLoadImage(fileName);
	}

	arenaLevels[0] = { 182, 28, 671, 1549 };
	arenaLevels[1] = { 754, 28, 671, 1549 };

	//hero player
	playerImage = iLoadImage("Images/Ghost.png");


	//ARENA 2
	arena2BG = iLoadImage("Images/Background/arena2BG.png");
	playAgain = iLoadImage("Images/Background/playAgain.png");

	//MINI BOSS
	LoadMiniBossSprites();

	// Initialize miniBoss positions and stats
	miniBoss[0].x = 1500; miniBoss[0].y = groundY;
	miniBoss[0].health = 100; miniBoss[0].maxHealth = 100;
	miniBoss[0].walkSpeed = 2; miniBoss[0].state = ENEMY_IDLE;

	miniBoss[1].x = 3500; miniBoss[1].y = groundY;
	miniBoss[1].health = 100; miniBoss[1].maxHealth = 100;
	miniBoss[1].walkSpeed = 2; miniBoss[1].state = ENEMY_IDLE;

	miniBoss[2].x = 6000; miniBoss[2].y = groundY;
	miniBoss[2].health = 100; miniBoss[2].maxHealth = 100;
	miniBoss[2].walkSpeed = 2; miniBoss[2].state = ENEMY_IDLE;

	// Copy sprite frames to each miniBoss
	for (int b = 0; b < 3; ++b) {
		for (int i = 0; i < 6; ++i) miniBoss[b].idleFrames[i] = miniBossIdle[i];
		for (int i = 0; i < 12; ++i) miniBoss[b].walkFrames[i] = miniBossWalk[i];
		for (int i = 0; i < 15; ++i) miniBoss[b].attackFrames[i] = miniBossAttack[i];
		for (int i = 0; i < 5; ++i) miniBoss[b].takeHitFrames[i] = miniBossTakeHit[i];
		for (int i = 0; i < 22; ++i) miniBoss[b].deathFrames[i] = miniBossDeath[i];
	}

	// Initialize animation and timing variables for each miniBoss
	for (int b = 0; b < 3; ++b) {
		miniBoss[b].direction = 1;        // face right
		miniBoss[b].frameIndex = 0;       // start on first frame
		miniBoss[b].lastFrameTime = clock();  // set timer baseline
		miniBoss[b].lastDamageTime = 0;   // initialize damage timer
	}
}


void blinkText(){
	if (timeElapsed % 350 == 0){
		blinkState = !blinkState;
	}
	timeElapsed++;

	displayImages(bg, 0, 0, screenWidth, screenHeight);

	if (blinkState == 0) {
		iText(centerX, centerY, "Press Space or Enter to Continue", GLUT_BITMAP_TIMES_ROMAN_24);
	}
}


void displayMainMenu(){
	displayImages(bgMainMenu, 0, 0, screenWidth, screenHeight); //background of main menu

	for (int i = 0; i < 5; i++){
		double y = buttonStartY - (i*buttonGap);
		if (i == hoveredButtonIndex || (hoveredButtonIndex == -1 && i == selectedButtonIndex)) {
			displayImages(mainMenuButtonsHover[i], buttonX - 12, y - 12, buttonWidth*1.15, buttonHeight*1.15);
		}
		else {
			displayImages(mainMenuButtons[i], buttonX, y, buttonWidth, buttonHeight);
		}

		if (blinkState == 0) {
			iSetColor(255, 255, 255);
			iText(700, 60, "Use Mouse or Arrow Keys to Navigate Main Menu", GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}
}

void drawLevelMenu()
{
	// Background
	iShowImage(0, 0, screenWidth, screenHeight, plainBG);

	const int menuW = 375;
	const int menuH = 900;
	int posX[2] = { 391.2, 1139.8 };
	int posY[2] = { 28, 28 };

	// Draw the three arena cards (middle one gets hover scale if hovered)
	for (int i = 0; i < 2; ++i) {
		const bool isHovered = (i == hoveredArena);
		const double s = isHovered ? hoverScale : 1.0;

		// keep growth centered around the original rect
		int drawX = posX[i];
		int drawY = 28;

		if (isHovered) {
			iShowImage(drawX, drawY + 70, menuW + 75, menuH + 75, levelMenu[i]); // Use hover texture
		}
		else {
			iShowImage(drawX, drawY + 70, menuW, menuH, levelMenu[i]); // Use normal texture
		}
	}

	// Instruction text (bottom centered)
	iSetColor(255, 215, 0); // warm gold fits your UI
	iText(screenWidth / 2 - 180, 40, "Use Mouse to Hover ARENAS", GLUT_BITMAP_HELVETICA_18);
}



#endif