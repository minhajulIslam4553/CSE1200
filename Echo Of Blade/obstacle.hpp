#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "display.hpp"


enum ObstacleType { FIRE_BOX, SPEAR, BOMB, SPIKE, DOOR};


struct Obstacle {
	ObstacleType type;
	double x, y;
	int currentFrame;
	int totalFrames;
	int* frames; 
	int width, height;
	int animationDelay; 
	int animationCounter; 
	bool active;
};

const int maxObstacles = 150;
int obstacleCount = 0;
Obstacle obstacles[maxObstacles];

extern int  spikeFrames[10], bombFrames[20], spearFrames[12], fireBoxFrames[10], doorFrames[14];
extern double backgroundOfSetX;
extern int screenWidth, screenHeight;
extern double startPosition;
extern int fireBoxWidth, fireBoxHeight;
extern int spearWidth, spearHeight;
extern int bombWidth, bombHeight;
extern int spikeWidth, spikeHeight;
extern int doorWidth, doorHeight;

void addNewObstacle(ObstacleType type, double x, double y, int spriteFrames[], int totalFrames, int width, int height, int animationDelay){
	if (obstacleCount >= maxObstacles) return;

	obstacles[obstacleCount].type = type;
    obstacles[obstacleCount].x = x;
    obstacles[obstacleCount].y = y;
    obstacles[obstacleCount].frames = spriteFrames;
    obstacles[obstacleCount].totalFrames = totalFrames;
    obstacles[obstacleCount].width = width;
    obstacles[obstacleCount].height = height;
    obstacles[obstacleCount].currentFrame = 0;
    obstacles[obstacleCount].animationDelay = animationDelay;
    obstacles[obstacleCount].animationCounter = 0;
    obstacles[obstacleCount].active = true;

    obstacleCount++;


}

void generateObstacle(){
	obstacleCount = 0;
	startPosition = 1200;

	for (int i = 0; i < 3; i++){
		addNewObstacle(SPIKE, startPosition + i*(spikeWidth + 5), 100, spikeFrames, 10, spikeWidth, spikeHeight, 10);
	}
	startPosition = 2300;

	for (int i = 0; i < 3; i++){
		addNewObstacle(BOMB, startPosition + i*(bombWidth - 10), 100, bombFrames, 20, bombWidth, bombHeight, 5);
	}
	startPosition = 3800;

	for (int i = 0; i < 5; i++){
		addNewObstacle(SPEAR, startPosition + i*(spearWidth + 5), 100, spearFrames, 12, spearWidth, spearHeight, 5 + i * 3);
	}
	startPosition = 5100;

	for (int i = 0; i < 3; i++){
		addNewObstacle(FIRE_BOX, startPosition + i*(fireBoxWidth + 5), 100, fireBoxFrames, 10, fireBoxWidth, fireBoxHeight, 5 + i * 2);
	}
	startPosition = 6600;

	for (int i = 0; i < 2; i++){
		addNewObstacle(BOMB, startPosition + i*(bombWidth - 10), 100, bombFrames, 20, bombWidth, bombHeight, 5);
	}
	startPosition = 7700;

	for (int i = 0; i < 6; i++){
		addNewObstacle(SPEAR, startPosition + i*(spearWidth + 5), 100, spearFrames, 12, spearWidth, spearHeight, 5 + i * 3);
	}
	startPosition = 9400;

	for (int i = 0; i < 2; i++){
		addNewObstacle(SPIKE, startPosition + i*(spikeWidth + 5), 100, spikeFrames, 10, spikeWidth, spikeHeight, 10);
	}
	startPosition = 10800;

	for (int i = 0; i < 5; i++){
		addNewObstacle(FIRE_BOX, startPosition + i*(fireBoxWidth + 5), 100, fireBoxFrames, 10, fireBoxWidth, fireBoxHeight, 5 + i * 2);
	}
	startPosition = 12400;

	for (int i = 0; i < 4; i++){
		addNewObstacle(SPIKE, startPosition + i*(spikeWidth + 5), 100, spikeFrames, 10, spikeWidth, spikeHeight, 10);
	}

	startPosition = 13900;

	for (int i = 0; i < 5; i++){
		addNewObstacle(SPEAR, startPosition + i*(spearWidth + 5), 100, spearFrames, 12, spearWidth, spearHeight, 5 + i * 3);
	}
	startPosition = 14700;

	for (int i = 0; i < 4; i++){
		addNewObstacle(FIRE_BOX, startPosition + i*(fireBoxWidth + 5), 100, fireBoxFrames, 10, fireBoxWidth, fireBoxHeight, 5 + i * 2);
	}

	startPosition = 16300;
	addNewObstacle(DOOR, startPosition, 100, doorFrames, 14, doorWidth, doorHeight, 4);
}

	void updateObstacle(){
		for (int i = 0; i < obstacleCount; i++){
			if (!obstacles[i].active) continue;
			obstacles[i].animationCounter++;

			if (obstacles[i].animationCounter >= obstacles[i].animationDelay){
				obstacles[i].currentFrame = (obstacles[i].currentFrame + 1) % obstacles[i].totalFrames;
				obstacles[i].animationCounter = 0;
			}
		}
	}

	void drawObstacle(){
		for (int i = 0; i < obstacleCount; i++){
			if (!obstacles[i].active) continue;

			double drawX = obstacles[i].x - backgroundOfSetX;
			if (drawX + obstacles[i].width < 0 || drawX > screenWidth) continue;

			iShowImage(drawX, obstacles[i].y, obstacles[i].width, obstacles[i].height, obstacles[i].frames[obstacles[i].currentFrame]);
		}
	}

#endif // !OBSTACLE_HPP
