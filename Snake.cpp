#include "Snake.h"
#include <random>
#include<ctime>
#include<iostream>
Snake::Snake(SnakeGame& game/*std::shared_ptr<SnakeGame> game*/, const int snakeSpeed, unsigned char firstBoxColour[3], unsigned char secondBoxColour[3],
    unsigned char colourEnlarge[3], unsigned char colourShrink[3], int boxSize, int snakeLength): game(game) {
    score = 0;
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
    currHeadX = snakeBody[0].getX();
    currHeadY = snakeBody[0].getY();
    background.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, game.W - 1, game.H - 1, green, 1.0f)
        .resize(boxSize, boxSize).noise(35).blur(2).normalize(0, 128);
}
void Snake::initSnakeBody() {
    for (int i = 0; i < snakeLength; i++) {
        if (i % 2 == 0)
            snakeBody.push_back(PixelBox(boxSize, -boxSize*i - boxSize, 0, firstBoxColour));
        else
            snakeBody.push_back(PixelBox(boxSize, -boxSize * i - boxSize, 0, secondBoxColour));
    }
}
void Snake::enlargeSnake(int amount) {
    // chance to get multiplication of enlarge 
    std::random_device rd;
    size_t param = rd();
    if (param % 13 == 0) {
        amount = 3;
    }
    if (param % 30 == 0) {
        amount = 5;
    }
    for (int i = snakeLength; i < snakeLength + amount; i++) {
        if (i % 2 == 0)
            snakeBody.push_back(PixelBox(boxSize, 2*snakeBody[i-1].getX() - snakeBody[i-2].getX(), 
                2*snakeBody[i - 1].getY() -snakeBody[i - 2].getY(), firstBoxColour));
        else
            snakeBody.push_back(PixelBox(boxSize, 2 * snakeBody[i - 1].getX() - snakeBody[i - 2].getX(),
                2 * snakeBody[i - 1].getY() - snakeBody[i - 2].getY(), secondBoxColour));
    }
    score = score + 50 + (amount - 1) * 30;
    snakeLength += amount;
}
bool Snake::shrinkSnake(int amount) {
    score -= amount * 40;
    if (amount >= snakeLength) {
        snakeBody.clear();
        snakeLength = 0;
        return true;
    }
    else { 
        for (int i = 0; i < amount; i++) {
            drawGrass();
            snakeBody.erase(snakeBody.end()-1);
            snakeLength = snakeBody.size();
        }        
        return false;
    }  
}
// Draw the snake on display image
void Snake::draw() const {
    const unsigned char green[3] = { 50, 205, 50 };
    const unsigned char white[3] = { 255, 255, 255 };
    for (auto box : snakeBody)
        box.draw(game.disp, game.background);
    for (auto box : enlargeApples)
        box.draw(game.disp, game.background);
    for (auto box : shrinkApples)
        box.draw(game.disp, game.background);
    game.background.display(game.disp);
    game.disp.wait(snakeSpeed);
}
void Snake::drawGrass() {
    const unsigned char green[3] = { 50, 205, 50 };
    cimg_library::CImg<unsigned char> grass;
    grass.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, game.W - 1, game.H - 1, green, 1.0f)
        .resize(boxSize, boxSize).noise(35).blur(2).normalize(0, 128);
    background = grass;
    game.background.draw_image(snakeBody[snakeLength - 1].getX(), snakeBody[snakeLength - 1].getY(), 0, 0, background);
}
void Snake::moveLeft() {
    drawGrass();
    for (std::size_t i = snakeLength - 1; i >0; i--)
        snakeBody[i].setPosition(snakeBody[i - 1].getX(), snakeBody[i - 1].getY());
    snakeBody[0].setX(snakeBody[0].getX() - boxSize);
    if (snakeBody[0].getX() < 0)
        snakeBody[0].setX(static_cast<int>(game.background.width()) - boxSize);
    draw();
}
void Snake::moveRight() {
    drawGrass();
    for (std::size_t i = snakeLength - 1; i > 0; i--)
        snakeBody[i].setPosition(snakeBody[i - 1].getX(), snakeBody[i - 1].getY());
    snakeBody[0].setX(snakeBody[0].getX() + boxSize);
    if (snakeBody[0].getX() > static_cast<int>(game.background.width()) - boxSize)
        snakeBody[0].setX(0);
    draw();
}
void Snake::moveUp() {
 
    drawGrass();
    for (std::size_t i = snakeLength - 1; i > 0; i--)
        snakeBody[i].setPosition(snakeBody[i - 1].getX(), snakeBody[i - 1].getY());
    snakeBody[0].setY(snakeBody[0].getY() - boxSize);
    if (snakeBody[0].getY() < 0)
        snakeBody[0].setY(static_cast<int>(game.background.height()) - boxSize);
    draw();
}
void Snake::moveDown() {
    drawGrass();
    for (std::size_t i = snakeLength - 1; i > 0; i--)
        snakeBody[i].setPosition(snakeBody[i - 1].getX(), snakeBody[i - 1].getY());
    snakeBody[0].setY(snakeBody[0].getY() + boxSize);
    if (snakeBody[0].getY() > static_cast<int>(game.background.height()) - boxSize)
        snakeBody[0].setY(0);
    draw();
}
bool Snake::move(int param) {
    placeRandomEnlargeApple();
    placeRandomShrinkApple();
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
    currHeadX = snakeBody[0].getX();
    currHeadY = snakeBody[0].getY();
    return isCollide() + checkApples();
}
// Is checking pixelBox have same coordinates as some pixelBox in vector
bool Snake::isCollide() const{
    for (int i = 1; i < snakeLength; i++) {
        if(snakeBody[i].getX() == getXHeadPosition() && snakeBody[i].getY() == getYHeadPosition())
            return true;
    }
    return false;
}
bool Snake::isAteApple(std::vector<PixelBox>& apples) const{
    int len = apples.size();
    for (int i = 0; i < len; i++) {
        if (apples[i].getX() == getXHeadPosition() && apples[i].getY() == getYHeadPosition())
            return true;
    }
    return false;
}
int Snake::getXHeadPosition() const {
    return currHeadX;
}
int Snake::getYHeadPosition() const {
    return currHeadY;
}
int Snake::getScore() const {
    return score;
}
void Snake::placeRandomEnlargeApple() {
    if (enlargeApples.size() >= game.maximumEnlargeApples)
        return;
    if (snakeLength >= (game.W - boxSize) * (game.H - boxSize) - 1)
        return;
    PixelBox apple = placeRandomPixelBox(colourEnlarge);
    enlargeApples.push_back(apple);
}
void Snake::placeRandomShrinkApple() {
    if (shrinkApples.size() >= game.maximumShrinkApples)
        return;
    if (snakeLength >= (game.W - boxSize) * (game.H - boxSize) - 1)
        return;
    PixelBox apple = placeRandomPixelBox(colourShrink);
    shrinkApples.push_back(apple);
}
bool Snake::isContainCoordinates(std::vector<PixelBox> &vec, int x, int y) const{
    for (auto box : vec) {
        if (x == box.getX() && y == box.getY())
            return true;
    }
    return false;
}
bool Snake::checkApples() {
    if (isAteApple(enlargeApples)) {
        deleteApple(enlargeApples);
        enlargeSnake(1);
    }
    if (isAteApple(shrinkApples)) {
        deleteApple(shrinkApples);
        return shrinkSnake(4);
    }
    return false;
}
void Snake::deleteApple(std::vector<PixelBox>& apples) {
    for (int i = 0; i < apples.size(); i++) {
        if (apples[i].getX() == snakeBody[0].getX() && apples[i].getY() == snakeBody[0].getY()) {
            apples.erase(apples.begin() + i);
        }
    }
}
PixelBox Snake::placeRandomPixelBox(unsigned char colour[3]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrX(0, game.W - boxSize);
    std::uniform_int_distribution<> distrY(0, game.H - boxSize);
    while (true) {
        int x = distrX(gen);
        x = x / boxSize * boxSize;
        int y = distrY(gen);
        y = y / boxSize * boxSize;
        if (isContainCoordinates(snakeBody, x, y) || isContainCoordinates(enlargeApples, x, y)
            || isContainCoordinates(shrinkApples, x, y))
            continue;
        PixelBox apple(boxSize, x, y, colour);
        return apple;
    }
}