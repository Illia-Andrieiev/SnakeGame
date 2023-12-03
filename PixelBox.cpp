#include "PixelBox.h"
#include"libs/boost/ut.hpp"
#include<string>
PixelBox::PixelBox(int size, int x, int y, unsigned char red, unsigned char green, unsigned char blue) {
	this->size = size;
	this->x = x;
	this->y = y;
	this->colour[0] = red;
	this->colour[1] = green;
	this->colour[2] = blue;
}
PixelBox::PixelBox(int size, int x, int y, unsigned char colour[3]) {
	this->size = size;
	this->x = x;
	this->y = y;
	this->colour[0] = colour[0];
	this->colour[1] = colour[1];
	this->colour[2] = colour[2];
}
PixelBox::PixelBox(const int size, const int x, const int y, const unsigned char colour[3]) {
	this->size = size;
	this->x = x;
	this->y = y;
	this->colour[0] = colour[0];
	this->colour[1] = colour[1];
	this->colour[2] = colour[2];
}
int PixelBox::getSize() const {
	return size;
}
int PixelBox::getX() const {
	return x;
}
int PixelBox::getY() const {
	return y;
}
unsigned char* PixelBox::getColour() const {
	unsigned char* colour = new unsigned char[3];
	colour[0] = this->colour[0];
	colour[1] = this->colour[1];
	colour[2] = this->colour[2];
	return colour;
}
unsigned char PixelBox::getRed() const {
	return colour[0];
}
unsigned char PixelBox::getGreen() const {
	return colour[1];
}
unsigned char PixelBox::getBlue() const {
	return colour[2];
}
void PixelBox::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
}
void PixelBox::setX(int x) {
	this->x = x;
}
void PixelBox::setY(int y) {
	this->y = y;
}
void PixelBox::setColour(unsigned char newColour[3]) {
	this->colour[0] = newColour[0];
	this->colour[1] = newColour[1];
	this->colour[2] = newColour[2];
}
void PixelBox::setRed(unsigned char red) {
	colour[0] = red;
}
void PixelBox::setGreen(unsigned char green) {
	colour[1] = green;
}
void PixelBox::setBlue(unsigned char blue) {
	colour[2] = blue;
}
// Draw PixelBox on display image
void PixelBox::draw(cimg_library::CImgDisplay& display, cimg_library::CImg<unsigned char>& image) const {
	for (int i = x; i < x + size ; i++) { //	|| i < image.width()
		for (int j = y; j < y + size ; j++) { // || j < image.height()
			image.draw_point(i, j,colour);
		}
	}
}
void PixelBox::drawTest() {
	using namespace boost::ut;
	"draw_test"_test = [] {
		// Arrange
		cimg_library::CImg<unsigned char> image(100, 100, 1, 3, 0);
		cimg_library::CImgDisplay display(image, "Test");
		PixelBox pixelBox(20, 10, 10, 200, 200, 200 ); // Assuming PixelBox has a constructor that takes x, y, size, and colour

		// Act
		pixelBox.draw(display, image);

		// Assert
		for (int i = 10; i < 30; i++) {
			for (int j = 10; j < 30; j++) {
				expect(	image(i, j, 0, 0) == 200 && image(i, j, 0, 1) == 200 && image(i, j, 0, 2) == 200)<< "Pixel at (" + std::to_string(i) + ", " + std::to_string(j) + ") is not gray";
			}
		}
		};
}
// Is given colour same to PIxelBox colour
bool PixelBox::isColourSame(const unsigned char colour[3]) const {
	for (int i = 0; i < 3; i++) {
		if (colour[i] != this->colour[i])
			return false;
	}
	return true;
}