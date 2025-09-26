#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "display.hpp"
#include "heroPlayer.hpp"
#include "gameposition.hpp"
#include "obstacle.hpp"
#include "respawn.hpp"

extern double characterX, characterY;     // player position on screen
extern double realTimePositionX, backgroundOfSetX;          // real world X position
extern int playerHealth;
extern int screenWidth, screenHeight;
void beingDead();

extern int spikeWidth, spikeHeight;
extern int bombWidth, bombHeight;
extern int spearWidth, spearHeight;
extern int fireBoxWidth, fireBoxHeight;

extern clock_t lastSpikeDamageTime;
extern clock_t lastFireDamageTime;
extern clock_t lastBombDamageTime;


extern bool arena1Completed;
extern GameState currentState;

const int SPIKE_DAMAGE = 50;
const int BOMB_DAMAGE = 25;
const int FIREBOX_DAMAGE = 3;

const int SPIKE_INTERVAL_MS = 250;
const int FIREBOX_INTERVAL_MS = 150;
const int BOMB_INTERVAL_MS = 350;
const int BOMB_RADIUS_X = 35.0;

//pop-up
extern bool popUpStart, popUpShown;

int msSince(clock_t then){
	return int((clock() - then) * 1000 / CLOCKS_PER_SEC);
}

void handleCollision() {
	clock_t now = clock();

	// Player dimensions (adjust if needed)
	int playerW = 300;
	int playerH = 300;
	const double playerLeft = realTimePositionX + 15.0;
	const double playerFeetY = characterY + 10.0;

	for (int i = 0; i < obstacleCount; i++) {
		if (!obstacles[i].active) continue;

		double obsX = obstacles[i].x;
		double obsY = obstacles[i].y;
		int obsW = obstacles[i].width;
		int obsH = obstacles[i].height;

		const bool insideBandByLeft = (playerLeft >= obsX - 42) && (playerLeft <= obsX + obsW);
		const bool feetLow = (playerFeetY <= obsY + obsH);

		switch (obstacles[i].type) {
		case SPIKE:
			if (insideBandByLeft && feetLow){
				if (msSince(lastSpikeDamageTime) >= SPIKE_INTERVAL_MS) {
					playerHealth -= SPIKE_DAMAGE;
					lastSpikeDamageTime = clock();
				}
			}
			break;

		case BOMB: {

					   const double bcx = obsX + obsW * 0.5;
					   const double pcx = playerLeft + 150.0 * 0.5;

					   const double dx = fabs(pcx - bcx);
					   if (dx <= BOMB_RADIUS_X && feetLow){
						   if (msSince(lastBombDamageTime) >= BOMB_INTERVAL_MS) {
							   playerHealth -= BOMB_DAMAGE;
							   lastBombDamageTime = clock();
						   }
					   }
					   break;
		}

		case SPEAR:
			if (insideBandByLeft && feetLow){
				playerHealth = 0;
			}
			break;

		case FIRE_BOX:
			if (insideBandByLeft && feetLow){
				if (msSince(lastFireDamageTime) >= FIREBOX_INTERVAL_MS) {
					playerHealth -= FIREBOX_DAMAGE;
					lastFireDamageTime = clock();
				}
			}
			break;
		case DOOR:
			if (insideBandByLeft && feetLow && !popUpShown){
				if (!popUpShown){
					popUpStart = true;
					popUpShown = true;

					if (currentState == PLAY_GAME) {
						arena1Completed = true;
					}
				}
			}
			break;
		}

		// If dead → start animation
		if (playerHealth <= 0) {
			playerHealth = 0;
			beingDead();// trigger dead animation
			return;
		}
	}
	}

#endif