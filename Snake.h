#pragma once
#include<vector>
#include"libs/CImg-3.3.2/CImg.h"
#include"PixelBox.h"
#include"SnakeGame.h"
class SnakeGame;
class Snake
{
private:
	//std::shared_ptr<SnakeGame> game;
	SnakeGame& game;
	cimg_library::CImg<unsigned char> background;
	std::time_t timeFromPlaceEnlargeApple;
	std::time_t timeFromPlaceShrinkApple;
	std::time_t timeFromRemoveEnlargeApple;
	std::time_t timeFromRemoveShrinkApple;
	int score;
	int currHeadX;
	int currHeadY;
	int prevHeadX;
	int prevHeadY;
	int boxSize; // Size of one game field block
	int snakeSpeed; // Speed of snake
	unsigned char firstBoxColour[3]; // Colour of sneaks head pixels in RGB format
	unsigned char secondBoxColour[3]; // Colour of sneaks second box pixels in RGB format
	unsigned char colourEnlarge[3]; // Colour of apple pixels that enlarge snake in RGB format
	unsigned char colourShrink[3]; // Colour of apple pixels that shrink snake in RGB format
	int snakeLength; // Current snake length
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
	PixelBox placeRandomPixelBox(unsigned char colour[3]);
	void enlargeSnake(int amount);
	bool checkApples();
	bool shrinkSnake(int amount);
	void draw() const;
	bool isContainCoordinates(std::vector<PixelBox> &vec, int x, int y) const;
public:
	Snake(SnakeGame& game/*std::shared_ptr<SnakeGame> game*/, const int snakeSpeed, unsigned char firstBoxColour[3], unsigned char secondBoxColour[3],
		unsigned char colourEnlarge[3], unsigned char colourShrink[3], int boxSize, int snakeLength = 4);
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	bool move(int param);
	int getXHeadPosition() const;
	int getYHeadPosition() const;
	int getScore() const;
};

