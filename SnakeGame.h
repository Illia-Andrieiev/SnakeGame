#pragma once
#include "libs/CImg-3.3.2/CImg.h"
class SnakeGame
{
private:
	// Parametrs from main 
	int argc; 
	char** argv;
	// Game parametrs
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
	SnakeGame(int argc, char** argv);
	SnakeGame(int argc, char** argv, int boxSize);
	SnakeGame(int argc, char** argv, unsigned char firstBoxColour[3],
		unsigned char secondBoxColour[3], int boxSize);
	SnakeGame(int argc, char** argv, unsigned char firstBoxColour[3], 
		unsigned char secondBoxColour[3], unsigned char colourEnlarge[3], unsigned char colourShrink[3], int boxSize);
	void game();
};

 