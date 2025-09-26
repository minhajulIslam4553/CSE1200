#ifndef RESPAWN_HPP
#define RESPAWN_HPP

#include <time.h>
#include "gameposition.hpp"

extern double realTimePositionX, backgroundOfSetX;
extern double characterX, characterY;
extern int playerHealth;
extern const int maxHealth;
extern bool isDead;
extern const double groundY;
extern bool isJumping, isFalling;
extern int jumpCount;
extern bool canJumpNow;
extern double jumpVelocityX, jumpVelocityY;

extern int deathTimerMs, deadFramesIndex;

extern clock_t lastSpikeDamageTime;
extern clock_t lastFireDamageTime;
extern clock_t lastBombDamageTime;
extern clock_t lastFrameTime;

static double currextRespawnX = 400.0;

inline void updateCheckPoints(){
	if (realTimePositionX >= 12000.0) currextRespawnX = 12000.0;
	else if (realTimePositionX >= 9000.0) currextRespawnX = 9000.0;
	else if (realTimePositionX >= 6000.0) currextRespawnX = 6000.0;
	else if (realTimePositionX >= 3000.0) currextRespawnX = 3000.0;
	else currextRespawnX = 400.0;
}

void respawnPlayer(){
	updateCheckPoints();

	realTimePositionX = currextRespawnX;
	backgroundOfSetX = (currextRespawnX > 1500) ? currextRespawnX - 1500.0 : 0;
	if (backgroundOfSetX < 0.0) backgroundOfSetX = 0.0;

	characterX = (currextRespawnX <= 1500) ? currextRespawnX : 1500;
	characterY = groundY;
	playerHealth = maxHealth;
	
	previousState = IDLE;

	isJumping = false;
	isFalling = false;
	jumpVelocityX = 0.0;
	jumpVelocityY = 0.0;
	jumpCount = 0;
	canJumpNow = false;

	deathTimerMs = 0;
	deadFramesIndex = 0;

	lastBombDamageTime = 0;
	lastFireDamageTime = 0;
	lastSpikeDamageTime = 0;

	lastFrameTime = clock();

	isDead = false;
}

#endif // !RESPAWN_HPP

