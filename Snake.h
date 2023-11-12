#pragma once
#include<functional>
#include<memory>
#include<vector>
#include"libs/CImg-3.3.2/CImg.h"
#include"PixelBox.h"
#include"SnakeGame.h"
class SnakeGame;
class Snake
{
private:
	//std::shared_ptr<SnakeGame> game;
	SnakeGame* game;
	cimg_library::CImg<unsigned char> background;
	int boxSize; // Size of one game field block
	int snakeSpeed; // Speed of snake
	unsigned char firstBoxColour[3]; // Colour of sneaks head pixels in RGB format
	unsigned char secondBoxColour[3]; // Colour of sneaks second box pixels in RGB format
	unsigned char colourEnlarge[3]; // Colour of apple pixels that enlarge snake in RGB format
	unsigned char colourShrink[3]; // Colour of apple pixels that shrink snake in RGB format
	int snakeLength; // Current snake length
	std::vector<PixelBox> snakeBody; // All snake PixelBoxes
	// Methods
	void initSnakeBody();
public:
	Snake(SnakeGame* game,const int snakeSpeed, unsigned char firstBoxColour[3], unsigned char secondBoxColour[3],
		unsigned char colourEnlarge[3], unsigned char colourShrink[3], int boxSize, int snakeLength = 4);
	void enlargeSnake(int amount);
	void shrinkSnake(int amount);
	void draw() const;
	void drawFirst();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	bool move(int param);
};

