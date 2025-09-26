#ifndef GAMEPOSITION_HPP
#define GAMEPOSITION_HPP

enum movementstate {
	IDLE, 
	RUN, 
	REVERSE_RUN, 
	REVERSE_IDLE, 
	DEAD
};

extern movementstate previousState;

#endif