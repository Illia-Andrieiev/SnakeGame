#include "Snake.h"
Snake::Snake(SnakeGame* game, const int snakeSpeed, unsigned char firstBoxColour[3], unsigned char secondBoxColour[3],
    unsigned char colourEnlarge[3], unsigned char colourShrink[3], int boxSize, int snakeLength) {
    const unsigned char green[3] = { 50, 205, 50 };
    const unsigned char white[3] = { 255, 255, 255 };
    for (int i = 0; i < 3; i++) {
        this->colourEnlarge[i] = colourEnlarge[i];
        this->colourShrink[i] = colourShrink[i];
        this->firstBoxColour[i] = firstBoxColour[i];
        this->secondBoxColour[i] = secondBoxColour[i];
    }
    this->snakeLength = snakeLength;
    this->snakeSpeed = snakeSpeed;
    if (boxSize < 5)
        this->boxSize = 5;
    else if (boxSize > 50)
        this->boxSize = 50;
    else
        this->boxSize = (boxSize / 5) * 5;
    initSnakeBody();
    this->game = game;
    background.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, game->W - 1, game->H - 1, green, 1.0f)
        .resize(boxSize, boxSize).noise(25).blur(2).normalize(0, 128);
}
void Snake::initSnakeBody() {
    unsigned char green[3] = { 50, 205, 50 };
    for (int i = 0; i < snakeLength; i++) {
        if (i % 2 == 0) 
            snakeBody.push_back(PixelBox(boxSize,  -boxSize * i - boxSize, 0, firstBoxColour));
        else 
            snakeBody.push_back(PixelBox(boxSize, -boxSize * i - boxSize, 0, secondBoxColour));
    }
    //snakeBody.push_back(PixelBox(boxSize, -boxSize * snakeLength - boxSize, 0, green));
}
void Snake::enlargeSnake(int amount) {

}
void Snake::shrinkSnake(int amount) {

}
// Draw the snake on display image
void Snake::draw() const {
    const unsigned char green[3] = { 50, 205, 50 };
    const unsigned char white[3] = { 255, 255, 255 };
  //  game->background = this->background;
    //game->disp.display(game->background);
    for (auto box : snakeBody)
        box.draw(game->disp, game->background);
    game->disp.wait(snakeSpeed);
}
void Snake::moveLeft() {
    if (snakeBody[0].getX() == snakeBody[1].getX() + boxSize) {
        return;
    }
    unsigned char green[3] = { 50, 205, 50 };
    cimg_library::CImg<unsigned char> im;
    im.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, game->W - 1, game->H - 1, green, 1.0f)
        .resize(boxSize, boxSize).noise(35).blur(2).normalize(0, 128);
    background = im;
    game->background.draw_image(snakeBody[snakeBody.size() - 1].getX() + boxSize, snakeBody[snakeBody.size() - 1].getY(), 0, 0, background);
    for (std::size_t i = snakeBody.size() - 1; i >0; i--)
        snakeBody[i].setPosition(snakeBody[i - 1].getX(), snakeBody[i - 1].getY());
    snakeBody[0].setX(snakeBody[0].getX() - boxSize);
    if (snakeBody[0].getX() < 0)
        snakeBody[0].setX(static_cast<int>(game->background.width()) - boxSize);
    draw();
}
void Snake::moveRight() {
    if (snakeBody[0].getX() == snakeBody[1].getX() - boxSize) {
        return;
    }
    unsigned char green[3] = { 50, 205, 50 };
    cimg_library::CImg<unsigned char> im;
    im.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, game->W - 1, game->H - 1, green, 1.0f)
        .resize(boxSize, boxSize).noise(35).blur(2).normalize(0, 128);
    background = im;
    game->background.draw_image(snakeBody[snakeBody.size() - 1].getX() - boxSize, snakeBody[snakeBody.size() - 1].getY(), 0, 0, background);
    for (std::size_t i = snakeBody.size() - 1; i > 0; i--)
        snakeBody[i].setPosition(snakeBody[i - 1].getX(), snakeBody[i - 1].getY());
    snakeBody[0].setX(snakeBody[0].getX() + boxSize);
    if (snakeBody[0].getX() > static_cast<int>(game->background.width()) - boxSize)
        snakeBody[0].setX(0);
    draw();
}
void Snake::moveUp() {
    if (snakeBody[0].getY() == snakeBody[1].getY() + boxSize) {
        return;
    }
    unsigned char green[3] = { 50, 205, 50 };
    cimg_library::CImg<unsigned char> im;
    im.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, game->W - 1, game->H - 1, green, 1.0f)
        .resize(boxSize, boxSize).noise(35).blur(2).normalize(0, 128);
    background = im;
    game->background.draw_image(snakeBody[snakeBody.size() - 1].getX() , snakeBody[ snakeBody.size() - 1].getY() + boxSize, 0, 0, background);
    for (std::size_t i = snakeBody.size() - 1; i > 0; i--)
        snakeBody[i].setPosition(snakeBody[i - 1].getX(), snakeBody[i - 1].getY());
    snakeBody[0].setY(snakeBody[0].getY() - boxSize);
    if (snakeBody[0].getY() < 0)
        snakeBody[0].setY(static_cast<int>(game->background.height()) - boxSize);
    draw();
}
void Snake::moveDown() {
    if (snakeBody[0].getY() == snakeBody[1].getY() - boxSize) {
        return;
    }
    unsigned char green[3] = { 50, 205, 50 };
    cimg_library::CImg<unsigned char> im;
    im.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, game->W - 1, game->H - 1, green, 1.0f)
        .resize(boxSize, boxSize).noise(35).blur(2).normalize(0, 128);
    background = im;
    game->background.draw_image(snakeBody[snakeBody.size() - 1].getX(), snakeBody[snakeBody.size() - 1].getY() - boxSize, 0, 0, background);
    for (std::size_t i = snakeBody.size() - 1; i > 0; i--)
        snakeBody[i].setPosition(snakeBody[i - 1].getX(), snakeBody[i - 1].getY());
    snakeBody[0].setY(snakeBody[0].getY() + boxSize);
    if (snakeBody[0].getY() > static_cast<int>(game->background.height()) - boxSize)
        snakeBody[0].setY(0);
    draw();
}
bool Snake::move(int param) {
    switch (param)
    { 
    case 0:
        moveLeft();
        break;
    case 1:
        moveRight();
        break;
    case 2:
        moveUp();
        break;
    case 3:
        moveDown();
        break;
    default:
        break;
    }
    return true;
}