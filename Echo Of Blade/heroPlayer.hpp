#ifndef HEROPLAYER_HPP
#define HEROPLAYER_HPP

#include "display.hpp"

extern int playerHealth;
extern const int maxHealth;
extern int playerImage;
extern const int playerImageX, playerImageY;
extern const int playerImageWidth, playerImageHeight;
extern const int healthBarX;
extern const int healthBarY;
extern const int healthBarWidth, healthBarHeight;

void drawPlayerUI(){
	iShowImage(playerImageX+25, playerImageY-70, playerImageWidth+40, playerImageHeight+40, playerImage);

	if (playerHealth < 0) playerHealth = 0;
	if (playerHealth > maxHealth) playerHealth = maxHealth;

	double healthPercent = (maxHealth > 0)? (double)playerHealth / (double)maxHealth : 0.0;
	int filledWidth = (int)(healthBarWidth * healthPercent);
	int damage = healthBarWidth - filledWidth;

	if (healthPercent < 0.3) 	iSetColor(255, 0, 0);
	else iSetColor(0, 255, 0);
	iFilledRectangle(healthBarX, healthBarY, filledWidth, healthBarHeight);
	
	if (damage > 0){
		iSetColor(50, 50, 50);
		iFilledRectangle(healthBarX+filledWidth, healthBarY, damage, healthBarHeight);
	}

	iSetColor(255, 255, 255);
	iRectangle(healthBarX, healthBarY, healthBarWidth, healthBarHeight);


	/*iRectangle(healthBarX, healthBarY, healthBarWidth, healthBarHeight);
	iFilledRectangle(healthBarX, healthBarY, filledWidth, healthBarHeight);

	iSetColor(255, 255, 255);*/
}
#endif // !HEROPLAYER_HPP
