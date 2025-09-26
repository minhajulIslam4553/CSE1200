#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <windows.h>
#include <mmsystem.h>

//music:rain music
void playRainMusic(){
	mciSendString("open \"Sound//rain.mp3\" alias rain", nullptr, 0, nullptr);
	mciSendString("play rain repeat", nullptr, 0, nullptr);
}

void stopRainMusic(){
	mciSendString("stop rain", nullptr, 0, nullptr);
	mciSendString("close rain", nullptr, 0, nullptr);
}

//music: rain
void playStartMusic(){
	mciSendString("open \"Sound//full.mp3\" alias fadeIn", nullptr, 0, nullptr);
	mciSendString("play fadeIn from 3500 repeat", nullptr, 0, nullptr);
}

void stopStartMusic(){
	mciSendString("stop fadeIn", nullptr, 0, nullptr);
	mciSendString("close fadeIn", nullptr, 0, nullptr);
}

void playEnter(){
	mciSendString("open \"Sound//enter.mp3\" alias enter", nullptr, 0, nullptr);
	mciSendString("play enter", nullptr, 0, nullptr);
}


#endif // !MUSIC_HPP

