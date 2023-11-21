#pragma once
#include "libs/CImg-3.3.2/CImg.h"
class PixelBox
{
private:
	unsigned char colour[3]; // Colour of pixels in box in RGB format
	int x, y; // Position of upper left box corner pixel
	int size; // size of PixelBox
public:
	PixelBox(int size,int x, int y, unsigned char red, unsigned char green, unsigned char blue);
	PixelBox(int size,int x, int y, unsigned char colour[3]);
	PixelBox(const int size, const int x, const int y, const unsigned char colour[3]);
	int getSize() const;
	int getX() const;
	int getY() const;
	unsigned char* getColour() const;
	unsigned char getRed() const;
	unsigned char getGreen() const;
	unsigned char getBlue() const;
	void setPosition(int x, int y);
	void setX(int x);
	void setY(int y);
	void setColour(unsigned char newColour[3]);
	void setRed(unsigned char red);
	void setGreen(unsigned char green);
	void setBlue(unsigned char blue);
	void draw(cimg_library::CImgDisplay &display, cimg_library::CImg<unsigned char>&image) const;
};

