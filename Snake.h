#pragma once
#include<vector>
#include"PixelBox.h"
class Snake
{
private:
	unsigned char firstBoxColour[3]; // Colour of sneaks head pixels in RGB format
	unsigned char secondBoxColour[3]; // Colour of sneaks second box pixels in RGB format
	unsigned char colourEnlarge[3]; // Colour of apple pixels that enlarge snake in RGB format
	unsigned char colourShrink[3]; // Colour of apple pixels that shrink snake in RGB format
	int snakeLength; // Current snake length
	std::vector<PixelBox> snakeBody;
public:
	Snake();
	void enlargeSnake(int amount);
	void shrinkSnake(int amount);
};

