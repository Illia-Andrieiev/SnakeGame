#pragma once
#include<vector>
#include"libs/CImg-3.3.2/CImg.h"
#include"PixelBox.h"
#include"SnakeGame.h"
class SnakeGame;
class Snake
{
private:
	SnakeGame& game; // Game, in which snake created
	std::time_t timeFromPlaceEnlargeApple; // Time from last placing enlarge apple 
	std::time_t timeFromPlaceShrinkApple; // Time from last placing shrink apple 
	std::time_t timeFromRemoveEnlargeApple; // Time from last removing enlarge apple 
	std::time_t timeFromRemoveShrinkApple; // Time from last removing shrink apple 
	int score; // Current score
	int currHeadX; // Current snake`s head X position 
	int currHeadY; // Current snake`s head Y position 
	int prevHeadX; // Previous snake`s head X position 
	int prevHeadY; // Previous snake`s head Y position 
	int boxSize; // Size of one game field block
	int snakeSpeed; // Speed of snake
	unsigned char firstBoxColour[3]; // Colour of sneaks head pixels in RGB format
	unsigned char secondBoxColour[3]; // Colour of sneaks second box pixels in RGB format
	unsigned char colourEnlarge[3]; // Colour of apple pixels that enlarge snake in RGB format
	unsigned char colourShrink[3]; // Colour of apple pixels that shrink snake in RGB format
	size_t snakeLength; // Current snake length
	std::vector<PixelBox>enlargeApples; // Positions of all enlarge apples. Stored as x1,y1 coordinates pairs 
	std::vector<PixelBox>shrinkApples; // Positions of all shrink apples. Stored as x1,y1 coordinates pairs
	std::vector<PixelBox> snakeBody; // All snake PixelBoxes
	// Methods
	void deleteCollidedApple(std::vector<PixelBox>& apples);
	void deleteRandomApple(std::vector<PixelBox>& apples);
	void initSnakeBody();
	void drawGrass(int x, int y);
	bool isAteApple(std::vector<PixelBox>& apples) const;
	bool isCollide() const;
	void placeRandomEnlargeApple();
	void placeRandomShrinkApple();
	void placeRandomApple(std::vector<PixelBox>& apples, unsigned char colour[3], std::time_t& timeFromPlace, time_t appleLifetime, int maxAmount);
	PixelBox placeRandomPixelBox(unsigned char colour[3]);
	void enlargeSnake(int amount);
	bool checkApples();
	bool shrinkSnake(int amount);
	void draw() const;
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	bool isContainCoordinates(std::vector<PixelBox> &vec, int x, int y) const;
	void changeSpeed(char operation, int amount);
	// Tests
	void clearTest();
	void initSnakeBodyTest();
	void deleteRandomAppleTest();
	void shrinkSnakeTest();
	void enlargeSnakeTest();
	void placeRandomPixelBoxTest();
	void placeRandomAppleTest();
	void moveDownTest();
	void moveUpTest();
	void moveLeftTest();
	void moveRightTest();
public:
	Snake(SnakeGame& game, const int snakeSpeed, unsigned char firstBoxColour[3], unsigned char secondBoxColour[3],
		unsigned char colourEnlarge[3], unsigned char colourShrink[3], int boxSize, int snakeLength = 4);
	bool move(int param);
	int getXHeadPosition() const;
	int getYHeadPosition() const;
	int getScore() const;
	void clear(const int snakeSpeed, unsigned char firstBoxColour[3], unsigned char secondBoxColour[3],
		unsigned char colourEnlarge[3], unsigned char colourShrink[3], int boxSize, int snakeLength = 4);
	void snakeTest();
};

