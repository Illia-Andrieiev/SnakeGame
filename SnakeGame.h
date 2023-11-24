#pragma once
#include "libs/CImg-3.3.2/CImg.h"
#include "Snake.h"
class SnakeGame
{
	friend class Snake;
private:
	cimg_library::CImg<unsigned char> background;
	cimg_library::CImgDisplay disp;
	// Parametrs from main 
	int argc; 
	char** argv;
	// Game parametrs
	int moveDirection = 1;
	int enlargeAppleLifeTime;
	int shrinkAppleLifeTime;
	int maximumEnlargeApples; // life time of enlarge apple in ms
	int maximumShrinkApples; // life time of shrink apple in ms
	unsigned int W = 200, H = 200;//400
	unsigned int scoreTableH = 25;
	int boxSize; // Size of one game field block
	unsigned char firstBoxColour[3]; // Colour of sneaks head pixels in RGB format
	unsigned char secondBoxColour[3]; // Colour of sneaks second box pixels in RGB format
	unsigned char colourEnlarge[3]; // Colour of apple pixels that enlarge snake in RGB format
	unsigned char colourShrink[3]; // Colour of apple pixels that shrink snake in RGB format
	/*
			Methots
	*/
	void setSnakeColour(unsigned char colour1[3], unsigned char colour2[3]);
	void setApplesColour(unsigned char colourEnlarge[3],	unsigned char colourShrink[3]);
public:
	SnakeGame(int argc, char** argv, int boxSize = 5);
	void game();
};

 