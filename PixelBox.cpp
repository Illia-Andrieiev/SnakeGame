#include "PixelBox.h"
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
	image.display(display);
}