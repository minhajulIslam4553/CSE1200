#ifndef MINIBOSS_HPP
#define MINIBOSS_HPP
#include "display.hpp"
#include <time.h>
#include <stdio.h>
#include <math.h>

extern int miniBossIdle[6], miniBossWalk[12], miniBossAttack[15], miniBossTakeHit[5], miniBossDeath[22];
extern double backgroundOfSetX;
extern int screenWidth;

enum EnemyState {
	ENEMY_IDLE,
	ENEMY_WALK,
	ENEMY_ATTACK,
	ENEMY_TAKEHIT,
	ENEMY_DEAD
};

struct MiniBoss {
	double x, y;
	double walkSpeed;
	int health;
	int maxHealth;
	EnemyState state;
	int direction; // 1 = right, -1 = left

	int idleFrames[6];
	int walkFrames[12];
	int attackFrames[15];
	int takeHitFrames[5];
	int deathFrames[22];

	int frameIndex;
	clock_t lastFrameTime;
	clock_t lastDamageTime; // Individual damage timer for each boss
	clock_t attackStartTime; // When the attack animation started
	bool hasDealtDamage; // Flag to track if damage was dealt in current attack cycle

	// New fields for proper hit handling
	clock_t lastHitTime; // When the boss was last hit by player
	bool pendingHit; // Flag to indicate if boss should react to hit after current action
	int pendingDamage; // Amount of pending damage
	bool isInAttackCooldown; // Prevents spam attacks
};

extern MiniBoss miniBoss[3];

void LoadMiniBossSprites() {
	char fileName[100];

	for (int i = 0; i < 6; i++) { // idle
		sprintf_s(fileName, "Images/MiniBossIdle/%d.png", i + 1);
		miniBossIdle[i] = iLoadImage(fileName);
	}

	for (int i = 0; i < 12; i++) { // walk
		sprintf_s(fileName, "Images/MiniBossWalk/%d.png", i + 1);
		miniBossWalk[i] = iLoadImage(fileName);
	}

	for (int i = 0; i < 15; i++) { // attack
		sprintf_s(fileName, "Images/MiniBossAttack/%d.png", i + 1);
		miniBossAttack[i] = iLoadImage(fileName);
	}

	for (int i = 0; i < 5; i++) { // take hit
		sprintf_s(fileName, "Images/MiniBossTakeHit/%d.png", i + 1);
		miniBossTakeHit[i] = iLoadImage(fileName);
	}

	for (int i = 0; i < 22; i++) { // death
		sprintf_s(fileName, "Images/MiniBossDeath/%d.png", i + 1);
		miniBossDeath[i] = iLoadImage(fileName);
	}
}

inline void drawMiniBoss(MiniBoss &boss) {
	int frameID = -1;
	switch (boss.state) {
	case ENEMY_IDLE:    frameID = boss.idleFrames[boss.frameIndex]; break;
	case ENEMY_WALK:    frameID = boss.walkFrames[boss.frameIndex]; break;
	case ENEMY_ATTACK:  frameID = boss.attackFrames[boss.frameIndex]; break;
	case ENEMY_TAKEHIT: frameID = boss.takeHitFrames[boss.frameIndex]; break;
	case ENEMY_DEAD:    frameID = boss.deathFrames[boss.frameIndex]; break;
	}

	// convert world -> screen
	double drawX = boss.x - backgroundOfSetX;
	double drawY = boss.y;

	// cull if off-screen
	if (drawX > screenWidth || drawX + 200 < 0) return;

	if (frameID != -1) iShowImage(drawX, drawY, 450, 450, frameID);

	// Health bar - only draw for living bosses
	if (boss.state != ENEMY_DEAD) {
		double healthWidth = 300.0 * boss.health / boss.maxHealth;
		iSetColor(0, 0, 0);
		iFilledRectangle(drawX + 80, drawY + 300, 300, 15);
		iSetColor(255, 0, 0);
		iFilledRectangle(drawX + 80, drawY + 300, healthWidth, 15);
	}
}

inline void handleMiniBossAI(MiniBoss &boss, double playerX, double playerY) {
	if (boss.state == ENEMY_DEAD) return;

	// Calculate actual distance between player and boss
	double distance = fabs(playerX - boss.x);
	clock_t currentTime = clock();

	// If player is within detection range (1000px), engage
	if (boss.health > 0 && distance < 1000) {
		// If close enough for attack (150px), start attacking
		if (distance < 150) {
			// Only start attack if not already attacking and not in cooldown
			if (boss.state != ENEMY_ATTACK && !boss.isInAttackCooldown) {
				boss.state = ENEMY_ATTACK;
				boss.frameIndex = 0;
				boss.lastFrameTime = currentTime;
				boss.attackStartTime = currentTime;
				boss.hasDealtDamage = false; // Reset damage flag for new attack
				boss.isInAttackCooldown = true; // Start cooldown
			}
			// Set direction to face player
			boss.direction = (playerX > boss.x) ? 1 : -1;
		}
		else if (boss.state != ENEMY_ATTACK && boss.state != ENEMY_TAKEHIT) {
			// Move toward player only if not attacking or taking hit
			boss.state = ENEMY_WALK;
			boss.direction = (playerX > boss.x) ? 1 : -1;
			boss.x += boss.walkSpeed * boss.direction;
		}
	}
	else if (boss.state != ENEMY_ATTACK && boss.state != ENEMY_TAKEHIT) {
		// Only go idle if not in middle of important animation
		boss.state = ENEMY_IDLE;
	}
}

inline void updateMiniBosses(double playerX, double playerY) {
	for (int i = 0; i < 3; i++) {
		MiniBoss &boss = miniBoss[i];
		clock_t currentTime = clock();

		if (boss.state == ENEMY_DEAD) {
			// Continue updating death animation frames
			if ((currentTime - boss.lastFrameTime) * 1000 / CLOCKS_PER_SEC > 150) {
				boss.frameIndex++;
				boss.lastFrameTime = currentTime;

				// Death animation has 22 frames (0-21)
				if (boss.frameIndex >= 22) {
					boss.frameIndex = 21; // Stay on last death frame
				}
			}
			continue; // Skip AI updates for dead bosses
		}

		// Handle pending hits (damage that was delayed due to attack state)
		if (boss.pendingHit && boss.state != ENEMY_ATTACK) {
			boss.health -= boss.pendingDamage;
			boss.pendingHit = false;
			boss.pendingDamage = 0;

			if (boss.health <= 0) {
				boss.health = 0;
				boss.state = ENEMY_DEAD;
				boss.frameIndex = 0;
				boss.lastFrameTime = currentTime;
				continue;
			}
			else {
				boss.state = ENEMY_TAKEHIT;
				boss.frameIndex = 0;
				boss.lastFrameTime = currentTime;
			}
		}

		// Update AI (only if not taking hit)
		if (boss.state != ENEMY_TAKEHIT) {
			handleMiniBossAI(boss, playerX, playerY);
		}

		// Update animation frames
		if ((currentTime - boss.lastFrameTime) * 1000 / CLOCKS_PER_SEC > 150) {
			boss.frameIndex++;
			boss.lastFrameTime = currentTime;

			int maxFrames = 0;
			switch (boss.state) {
			case ENEMY_IDLE: maxFrames = 6; break;
			case ENEMY_WALK: maxFrames = 12; break;
			case ENEMY_ATTACK: maxFrames = 15; break;
			case ENEMY_TAKEHIT: maxFrames = 5; break;
			case ENEMY_DEAD: maxFrames = 22; break;
			}

			if (boss.frameIndex >= maxFrames) {
				if (boss.state == ENEMY_ATTACK) {
					// Attack animation completed
					boss.isInAttackCooldown = false; // Clear attack cooldown

					// Check if player is still in range
					double distance = fabs(playerX - boss.x);
					if (distance < 150) {
						// Player still in range, but add delay before next attack
						boss.state = ENEMY_IDLE;
						boss.frameIndex = 0;
					}
					else {
						// Player moved away, go to appropriate state
						boss.state = ENEMY_IDLE;
						boss.frameIndex = 0;
					}
				}
				else if (boss.state == ENEMY_TAKEHIT) {
					// Take hit animation completed, return to AI control
					boss.state = ENEMY_IDLE;
					boss.frameIndex = 0;
				}
				else {
					boss.frameIndex = 0; // Loop other animations
				}
			}
		}

		// Reset attack cooldown after some time
		if (boss.isInAttackCooldown &&
			(currentTime - boss.attackStartTime) * 1000 / CLOCKS_PER_SEC > 2000) {
			boss.isInAttackCooldown = false;
		}
	}
}

inline void playerHitMiniBoss(MiniBoss &boss, int damage) {
	if (boss.state == ENEMY_DEAD) return;

	clock_t currentTime = clock();

	// Prevent spam hitting (invulnerability frames)
	if ((currentTime - boss.lastHitTime) * 1000 / CLOCKS_PER_SEC < 300) return;

	boss.lastHitTime = currentTime;

	// If boss is currently attacking, delay the hit reaction
	if (boss.state == ENEMY_ATTACK) {
		boss.pendingHit = true;
		boss.pendingDamage = damage;
		printf("Boss hit during attack - damage delayed\n");
		return;
	}

	// Apply damage immediately
	boss.health -= damage;

	if (boss.health <= 0) {
		boss.health = 0;
		boss.state = ENEMY_DEAD;
		boss.frameIndex = 0;
		boss.lastFrameTime = currentTime;
		printf("Boss killed!\n");
	}
	else {
		boss.state = ENEMY_TAKEHIT;
		boss.frameIndex = 0;
		boss.lastFrameTime = currentTime;
		printf("Boss hit! Health: %d\n", boss.health);
	}
}

inline int bossIndexFromPtr(MiniBoss* ptr) {
	for (int i = 0; i < 3; ++i) if (&miniBoss[i] == ptr) return i;
	return 0;
}

inline void miniBossAttackPlayer(MiniBoss &boss, int &playerHealth) {
	if (boss.state == ENEMY_ATTACK && boss.health > 0 && !boss.hasDealtDamage) {
		clock_t currentTime = clock();

		// Deal damage at specific frames of the attack animation (frames 9-11 out of 15)
		// This creates a "hit" moment in the attack animation
		if (boss.frameIndex >= 9 && boss.frameIndex <= 11) {
			// Only deal damage once per attack cycle and respect cooldown
			if ((currentTime - boss.lastDamageTime) * 1000 / CLOCKS_PER_SEC > 1500) { // 1.5 second cooldown
				playerHealth -= 22; // Damage per attack
				if (playerHealth < 0) playerHealth = 0;
				boss.lastDamageTime = currentTime;
				boss.hasDealtDamage = true; // Mark that damage was dealt this cycle
				printf("MiniBoss dealt damage! Player health: %d\n", playerHealth);
			}
		}
	}
}

// Helper function to initialize miniboss (call this when setting up bosses)
inline void initializeMiniBoss(MiniBoss &boss, double x, double y, double walkSpeed, int maxHealth) {
	boss.x = x;
	boss.y = y;
	boss.walkSpeed = walkSpeed;
	boss.health = maxHealth;
	boss.maxHealth = maxHealth;
	boss.state = ENEMY_IDLE;
	boss.direction = 1;
	boss.frameIndex = 0;
	boss.lastFrameTime = clock();
	boss.lastDamageTime = 0;
	boss.attackStartTime = 0;
	boss.hasDealtDamage = false;
	boss.lastHitTime = 0;
	boss.pendingHit = false;
	boss.pendingDamage = 0;
	boss.isInAttackCooldown = false;

	// Copy sprite references
	for (int i = 0; i < 6; i++) boss.idleFrames[i] = miniBossIdle[i];
	for (int i = 0; i < 12; i++) boss.walkFrames[i] = miniBossWalk[i];
	for (int i = 0; i < 15; i++) boss.attackFrames[i] = miniBossAttack[i];
	for (int i = 0; i < 5; i++) boss.takeHitFrames[i] = miniBossTakeHit[i];
	for (int i = 0; i < 22; i++) boss.deathFrames[i] = miniBossDeath[i];
}

#endif