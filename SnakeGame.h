#pragma once
#include <vector>
#include "libs/CImg-3.3.2/CImg.h"
#include "Snake.h"
#include <string>
class SnakeGame
{
	friend class Snake;
private:
	cimg_library::CImg<unsigned char> background; // Background img
	cimg_library::CImgDisplay disp; // Game display
	// Parametrs from main 
	int argc; 
	char** argv;
	// Game parametrs
	std::vector<int> records; // Contain records
	int maximumRecords = 10; // Maximum amount of records
	int moveDirection = 1; // Start move direction for snake
	time_t enlargeAppleLifeTime; // life time of enlarge apple in ms
	time_t shrinkAppleLifeTime; // life time of shrink apple in ms
	int maximumEnlargeApples; // Maximum amount of enlarge apples 
	int maximumShrinkApples; // Maximum amount of shrink apples 
	const unsigned int W = 200, H = 200; // Game field width and height in pixels
	const unsigned int scoreTableH = 25; // Height of score table in pixels
	int boxSize; // Size of one game field block
	unsigned char firstBoxColour[3]; // Colour of sneaks head pixels in RGB format
	unsigned char secondBoxColour[3]; // Colour of sneaks second box pixels in RGB format
	unsigned char colourEnlarge[3]; // Colour of apple pixels that enlarge snake in RGB format
	unsigned char colourShrink[3]; // Colour of apple pixels that shrink snake in RGB format
	/*
			Methots
	*/
	void setNewRoundParametrs(Snake& snake, int delay,int snakeLength);
	void readRecordsFromFile(std::string fileName);
	void printRecordsInFile(std::string fileName);
	void addRecord(int rec);
	void displayRecords();
	void displayMenu();
	void setSnakeColour(unsigned char colour1[3], unsigned char colour2[3]);
	void setApplesColour(unsigned char colourEnlarge[3],	unsigned char colourShrink[3]);
	// Tests
	void addRecordTest();
	void printRecordsInFileTest();
	void readRecordsFromFileTest();
public:
	SnakeGame(int argc, char** argv, int boxSize = 5);
	void game();
	void gameTest();
};

 