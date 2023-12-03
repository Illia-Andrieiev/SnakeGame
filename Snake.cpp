#include "Snake.h"
#include"libs/boost/ut.hpp"
#include <random>
#include<ctime>
#include<iostream>

// Constructor
Snake::Snake(SnakeGame& game, const int snakeSpeed, unsigned char firstBoxColour[3], unsigned char secondBoxColour[3],
    unsigned char colourEnlarge[3], unsigned char colourShrink[3], int boxSize, int snakeLength) : game(game) {
    score = 0; // Start score = 0
    // Initialization colours
    const unsigned char green[3] = { 50, 205, 50 };
    const unsigned char white[3] = { 255, 255, 255 };
    for (int i = 0; i < 3; i++) {
        this->colourEnlarge[i] = colourEnlarge[i];
        this->colourShrink[i] = colourShrink[i];
        this->firstBoxColour[i] = firstBoxColour[i];
        this->secondBoxColour[i] = secondBoxColour[i];
    }
    // Initialization snake parametrs
    timeFromPlaceEnlargeApple = (time_t)(game.enlargeAppleLifeTime / 1.5);
    timeFromPlaceShrinkApple = 0;
    timeFromRemoveEnlargeApple = 0;
    timeFromRemoveShrinkApple = 0;
    this->snakeLength = snakeLength;
    this->snakeSpeed = snakeSpeed;
    if (boxSize < 10) // min boxSize == 5
        this->boxSize = 5;
    else // max boxSize == 10
        this->boxSize = 10;
    // Initializate snake body
    initSnakeBody();
    currHeadX = snakeBody[0].getX();
    currHeadY = snakeBody[0].getY();
}
// Assign snake new start parametrs
void Snake::clear(const int snakeSpeed, unsigned char firstBoxColour[3], unsigned char secondBoxColour[3],
    unsigned char colourEnlarge[3], unsigned char colourShrink[3], int boxSize, int snakeLength){
    score = 0; // Start score = 0
    // Initialization colours
    const unsigned char green[3] = { 50, 205, 50 };
    const unsigned char white[3] = { 255, 255, 255 };
    for (int i = 0; i < 3; i++) {
        this->colourEnlarge[i] = colourEnlarge[i];
        this->colourShrink[i] = colourShrink[i];
        this->firstBoxColour[i] = firstBoxColour[i];
        this->secondBoxColour[i] = secondBoxColour[i];
    }
    // Initialization snake parametrs
    timeFromPlaceEnlargeApple = (time_t)(game.enlargeAppleLifeTime / 1.5);
    timeFromPlaceShrinkApple = 0;
    timeFromRemoveEnlargeApple = 0;
    timeFromRemoveShrinkApple = 0;
    this->snakeLength = snakeLength;
    this->snakeSpeed = snakeSpeed;
    if (boxSize < 10) // min boxSize == 5
        this->boxSize = 5;
    else // max boxSize == 10
        this->boxSize = 10;
    snakeBody.clear(); // Clear snake body
    enlargeApples.clear(); // Clear apples
    shrinkApples.clear(); // Clear apples
    // Initializate snake body from scratch
    initSnakeBody();
    currHeadX = snakeBody[0].getX();
    currHeadY = snakeBody[0].getY();
}
void Snake::clearTest() {
    enlargeApples.push_back(PixelBox(boxSize, 0, 0, colourEnlarge));
    shrinkApples.push_back(PixelBox(boxSize, 0, 0, colourShrink));
    timeFromPlaceShrinkApple = 1;
    timeFromRemoveEnlargeApple = 1;
    timeFromRemoveShrinkApple = 1;
    snakeLength = 12;
    clear(65, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 9); // Clear snake
    size_t len = snakeLength;
    time_t t2 = timeFromPlaceShrinkApple;
    time_t t3 = timeFromRemoveEnlargeApple;
    time_t t1 = timeFromRemoveShrinkApple;
    std::vector<PixelBox>& appleEn = enlargeApples;
    std::vector<PixelBox>& appleSh = shrinkApples;
    using namespace boost::ut;
    "Clear"_test = [len,t1,t2,t3,appleEn,appleSh] {
        expect(appleEn.size() == 0 && appleSh.size() == 0 >> fatal) << "Apples amount != 0";
        expect(t1 ==0 && t2 ==0&& t3 ==0) << "Time do not start from 0";
        expect(len == 9) << "Snake Length do not change";
    };
}
// Initializate snake body
void Snake::initSnakeBody() {
    snakeBody.clear();
    // Add sections to snake. if i%2 == 0 use firstBoxColour for section, else secondBoxColour
    for (int i = 0; i < snakeLength; i++) {
        if (i % 2 == 0)
            snakeBody.push_back(PixelBox(boxSize, -boxSize*i - boxSize, game.scoreTableH, firstBoxColour));
        else
            snakeBody.push_back(PixelBox(boxSize, -boxSize * i - boxSize, game.scoreTableH, secondBoxColour));
    }
}
void Snake::initSnakeBodyTest() {
    initSnakeBody();
    std::size_t len = snakeLength;
    std::vector<PixelBox> body = snakeBody;
    const unsigned char c1[3] = { firstBoxColour[0], firstBoxColour[1], firstBoxColour[2]};
    const unsigned char c2[3] = { secondBoxColour[0], secondBoxColour[1], secondBoxColour[2] };
    int boxSize = this->boxSize;
    int y = game.scoreTableH;
    using namespace boost::ut;
    "InitSnakeBody"_test = [body,len,c1,c2, boxSize,y] {
        expect(body.size() == len >> fatal) << "Expected snake length do not match to snake body real length";
        for (int i = 0; i < len; i++) {
            if(i % 2 ==0)
                expect(body[i].isColourSame(c1) && body[i].getY() == y && body[i].getX() == -boxSize * i - boxSize) 
                << "Initializated in not expected way";
            else
                expect(body[i].isColourSame(c2) && body[i].getY() == y&& body[i].getX() == -boxSize * i - boxSize)
                << "Initializated in not expected way";
        }
    };
}
// Enlarge snake length at least at amount points with chance to multiply. Min amount == 1
void Snake::enlargeSnake(int amount) {
    if (amount < 1)
        amount = 1;
    // chance to get multiplication of enlarge 
    std::random_device rd;
    size_t param = rd();
    if (param % 13 == 0) {
        amount += 2;
    }
    if (param % 30 == 0) {
        amount += 4;
    }
    // Add second section if necessary
    if (snakeLength == 1) {
        snakeBody.push_back(PixelBox(boxSize, prevHeadX, prevHeadY, secondBoxColour));
        --amount;
        ++snakeLength;
    } 
    // Add sections to snake. if i%2 == 0 use firstBoxColour for section, else secondBoxColour
    for (std::size_t i = snakeLength; i < snakeLength + amount; i++) {
        if (i % 2 == 0)
            snakeBody.push_back(PixelBox(boxSize, 2*snakeBody[i-1].getX() - snakeBody[i-2].getX(), 
                2*snakeBody[i - 1].getY() -snakeBody[i - 2].getY(), firstBoxColour));
        else
            snakeBody.push_back(PixelBox(boxSize, 2 * snakeBody[i - 1].getX() - snakeBody[i - 2].getX(),
                2 * snakeBody[i - 1].getY() - snakeBody[i - 2].getY(), secondBoxColour));
    }
    score = score + 50 + (amount - 1) * 30; // Increase score
    snakeLength += amount; // Increase snakeLength
}
void Snake::enlargeSnakeTest() {
    using namespace boost::ut;
    clear(65, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 5); // Clear snake
    enlargeSnake(0);
    size_t len = snakeLength;
    size_t expectedLen = snakeBody.size();
    "enlargeSnake_1"_test = [len, expectedLen] {
        // Test when amount is less than 1   
        expect(len == 6 || len == 8 || len ==10)<<"Method do not update snakeLength"; 
        expect(expectedLen == len) << "Method update snakeLength, but do not enlarge snake";
    };
    enlargeSnake(5);
    expectedLen = snakeBody.size();
    "enlargeSnake_2"_test = [expectedLen] {
        // Test when amount is greater than 1
        expect(expectedLen > 10 && expectedLen <21 ) << "Method update snakeLength, but do not enlarge snake";
    };
    clear(65, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 1); // Clear snake
    enlargeSnake(1);
    len = snakeLength;
    expectedLen = snakeBody.size();
    "enlargeSnake_3"_test = [len, expectedLen] {
        // Test when amount is greater than 1
        expect(expectedLen == len) << "Method update snakeLength, but do not enlarge snake";
     };
}
// Shrink snake length at amount points. Min amount == 1.
// Return false if snakeLength == 0, else return false.
bool Snake::shrinkSnake(int amount) {
    if (amount < 1)
        amount = 1;
    score -= amount * 40; // Decrease score
    if (amount >= snakeLength) {
        snakeBody.clear();
        snakeLength = 0;
        return true;
    }
    else { 
        for (int i = 0; i < amount; i++) {
            drawGrass(snakeBody[snakeLength - 1].getX(), snakeBody[snakeLength - 1].getY());
            snakeBody.erase(snakeBody.end()-1);
            snakeLength = snakeBody.size();
        }        
        return false;
    }  
}
void Snake::shrinkSnakeTest() {
    clear(65, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 9); // Clear snake
    bool result = shrinkSnake(3);
    size_t len = snakeLength;
    using namespace boost::ut;
    "shrinkSnake_1"_test = [result, len] {
        expect(!result) << "Method return wrong value";
        expect(len == 6) << "Method delete wrong amount of elements or don`t update snakeLength";
        };
    // Test when amount is greater than or equal to snake length
    result = shrinkSnake(7);
    len = snakeLength;
    "shrinkSnake_2"_test = [result, len] {
        expect(result) << "Method return wrong value";
        expect(len == 0) << "Method delete wrong amount of elements or don`t update snakeLength";
        };
    // Test when amount is less than 1
    clear(65, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 9); // Clear snake
    result = shrinkSnake(0);
    len = snakeLength;
    "shrinkSnake_3"_test = [result, len] {
        expect(!result) << "Method return wrong value";
        expect(len == 8) << "Method delete wrong amount of elements or don`t update snakeLength";
        };
}
// Draw the snake and apples on display background
void Snake::draw() const {
    const unsigned char green[3] = { 50, 205, 50 };
    const unsigned char white[3] = { 255, 255, 255 };
    // Draw snake
    for (auto box : snakeBody)
        box.draw(game.disp, game.background);
    // Draw apples
    for (auto box : enlargeApples)
        box.draw(game.disp, game.background);
    for (auto box : shrinkApples)
        box.draw(game.disp, game.background);
    game.background.display(game.disp); // Display background
    game.disp.wait(snakeSpeed); // Wait current snakeSpeed ms.  
}
// Draw square randomly generated grass img bixSize size on (x,y) position
void Snake::drawGrass(int x,int y) {
    const unsigned char green[3] = { 50, 205, 50 };
    // Generate random grass img
    cimg_library::CImg<unsigned char> grass;
    grass.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, game.W - 1, game.H - 1, green, 1.0f)
        .resize(boxSize, boxSize).noise(35).blur(2).normalize(0, 128);
    // Draw grass on background image
    game.background.draw_image(x, y, 0, 0, grass);
}
// Move snake head on one PixelBox left, snake`s ¹n section on snake`s ¹(n-1) section position.
// If X head position < 0, X = game.background.width()
void Snake::moveLeft() {
    drawGrass(snakeBody[snakeLength - 1].getX(), snakeBody[snakeLength - 1].getY()); // Draw grass on snake`s tail position 
    for (std::size_t i = snakeLength - 1; i >0; i--) // Set new snakeBody positions
        snakeBody[i].setPosition(snakeBody[i - 1].getX(), snakeBody[i - 1].getY());
    snakeBody[0].setX(snakeBody[0].getX() - boxSize); // Set new snake head position
    if (snakeBody[0].getX() < 0) // move on else side of background
        snakeBody[0].setX(static_cast<int>(game.background.width()) - boxSize);
    draw();
}
void Snake::moveLeftTest() {
    using namespace boost::ut;
    "moveLeft"_test = [this] {
        clear(65, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 3); // Clear snake
        snakeBody[0].setPosition(50, 60);
        snakeBody[1].setPosition(50 + boxSize, 60);
        snakeBody[2].setPosition(50 + boxSize*2, 60-boxSize);
        // Test when snake's head is not at the bottom of the game background
        moveLeft();
        expect(snakeBody[0].getX() == 50 - boxSize);
        expect(snakeBody[1].getX() == 50);
        expect(snakeBody[2].getX() == 50 + boxSize && snakeBody[2].getY() == 60) << "Snake move incorrectly";
        snakeBody[0].setPosition(0, 60);
        snakeBody[1].setPosition(boxSize, 60);
        snakeBody[2].setPosition(boxSize * 2, 60);
        // Test when snake's head is at the bottom of the game background
        moveLeft();
        expect(snakeBody[0].getX() == game.background.width() - boxSize) << "Snake head do not jump on another game field side";
        expect(snakeBody[1].getX() == 0);
        expect(snakeBody[2].getX() == boxSize);
        };
}
// Move snake head on one PixelBox right, snake`s ¹n section on snake`s ¹(n-1) section position.
// If X head position > game.background.width(), X = 0
void Snake::moveRight() {
    drawGrass(snakeBody[snakeLength - 1].getX(), snakeBody[snakeLength - 1].getY()); // Draw grass on snake`s tail position 
    for (std::size_t i = snakeLength - 1; i > 0; i--) // Set new snakeBody positions
        snakeBody[i].setPosition(snakeBody[i - 1].getX(), snakeBody[i - 1].getY());
    snakeBody[0].setX(snakeBody[0].getX() + boxSize); // Set new snake head position
    if (snakeBody[0].getX() > static_cast<int>(game.background.width()) - boxSize) // move on else side of background
        snakeBody[0].setX(0);
    draw();
}
void Snake::moveRightTest() {
    using namespace boost::ut;
    "moveRight"_test = [this] {
        clear(65, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 3); // Clear snake
        snakeBody[0].setPosition(50 + boxSize * 2, 60);
        snakeBody[1].setPosition(50 + boxSize, 60);
        snakeBody[2].setPosition(50, 60);
        // Test when snake's head is not at the bottom of the game background
        moveRight();
        expect(snakeBody[0].getX() == 50 + boxSize * 3);
        expect(snakeBody[1].getX() == 50 + boxSize * 2);
        expect(snakeBody[2].getX() == 50 + boxSize );
        snakeBody[0].setPosition(game.background.width()-boxSize, 60);
        snakeBody[1].setPosition(game.background.width() - boxSize*2, 60);
        snakeBody[2].setPosition(game.background.width() - boxSize*3, 60);
        // Test when snake's head is at the bottom of the game background
        moveRight();
        expect(snakeBody[0].getX() == 0) << "Snake head do not jump on another game field side";
        expect(snakeBody[1].getX() == game.background.width() - boxSize);
        expect(snakeBody[2].getX() == game.background.width() - boxSize * 2);
        };
}
// Move snake head on one PixelBox up, snake`s ¹n section on snake`s ¹(n-1) section position.
// If Y head position < game.scoreTableH, Y = game.background.height()
void Snake::moveUp() {
    drawGrass(snakeBody[snakeLength - 1].getX(), snakeBody[snakeLength - 1].getY()); // Draw grass on snake`s tail position
    for (std::size_t i = snakeLength - 1; i > 0; i--) // Set new snakeBody positions
        snakeBody[i].setPosition(snakeBody[i - 1].getX(), snakeBody[i - 1].getY());
    snakeBody[0].setY(snakeBody[0].getY() - boxSize); // Set new snake head position
    if ((unsigned int)snakeBody[0].getY() < game.scoreTableH) // move on else side of background
        snakeBody[0].setY(static_cast<int>(game.background.height()) - boxSize);
    draw();
}
void Snake::moveUpTest() {
    using namespace boost::ut;
    "moveUp"_test = [this] {
        clear(65, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 3); // Clear snake
        snakeBody[0].setPosition(10, 60);
        snakeBody[1].setPosition(10, 60 + boxSize);
        snakeBody[2].setPosition(10, 60 + boxSize * 2);
        // Test when snake's head is not at the bottom of the game background
        moveUp();
        expect(snakeBody[0].getY() == 60 - boxSize);
        expect(snakeBody[1].getY() == 60);
        expect(snakeBody[2].getY() == 60 + boxSize);
        snakeBody[0].setPosition(10, game.scoreTableH);
        snakeBody[1].setPosition(10, game.scoreTableH + boxSize);
        snakeBody[2].setPosition(10, game.scoreTableH + boxSize * 2);
        // Test when snake's head is at the bottom of the game background
        moveUp();
        expect(snakeBody[0].getY() == game.background.height() - boxSize) << "Snake head do not jump on another game field side";
        expect(snakeBody[1].getY() == game.scoreTableH);
        expect(snakeBody[2].getY() == game.scoreTableH + boxSize);
    };
}
// Move snake head on one PixelBox down, snake`s ¹n section on snake`s ¹(n-1) section position.
// If Y head position > game.background.height(), Y = game.scoreTableH
void Snake::moveDown() {
    drawGrass(snakeBody[snakeLength - 1].getX(), snakeBody[snakeLength - 1].getY());  // Draw grass on snake`s tail position
    for (std::size_t i = snakeLength - 1; i > 0; i--) // Set new snakeBody positions
        snakeBody[i].setPosition(snakeBody[i - 1].getX(), snakeBody[i - 1].getY());
    snakeBody[0].setY(snakeBody[0].getY() + boxSize); // Set new snake head position
    if (snakeBody[0].getY() > static_cast<int>(game.background.height()) - boxSize) // move on else side of background
        snakeBody[0].setY(game.scoreTableH);
    draw();
}
void Snake::moveDownTest() {
    using namespace boost::ut;
    "moveDown"_test = [this] {
        clear(65, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 3); // Clear snake
        snakeBody[0].setPosition(10, 20);
        snakeBody[1].setPosition(10, 20- boxSize);
        snakeBody[2].setPosition(10, 20 - boxSize*2);
        // Test when snake's head is not at the bottom of the game background
        moveDown();
        expect(snakeBody[0].getY() == 20 + boxSize);
        expect(snakeBody[1].getY() == 20_i);
        expect(snakeBody[2].getY() == 20 - boxSize);
        snakeBody[0].setPosition(10, game.background.height() - boxSize);
        snakeBody[1].setPosition(10, game.background.height() - boxSize*2);
        snakeBody[2].setPosition(10, game.background.height() - boxSize * 3);
        // Test when snake's head is at the bottom of the game background
        moveDown();
        expect(snakeBody[0].getY() == game.scoreTableH)<< "Snake head do not jump on another game field side";
        expect(snakeBody[1].getY() == game.background.height() - boxSize);
        expect(snakeBody[2].getY() == game.background.height() - boxSize * 2);
    };
}
// Move snake on game field. param: 0 - left, 1 - right, 2 - up, 3 - down. Place apples, remove apples and check for game over.
// Return true, if game over. Else return false
bool Snake::move(int param) {
    cimg_library::cimg::tic(); // Start countig time
    // Update previous head position
    prevHeadX = snakeBody[0].getX();
    prevHeadY = snakeBody[0].getY();
    // Place apples
    placeRandomEnlargeApple();
    placeRandomShrinkApple();
    // Move snake
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
    unsigned __int64 time = cimg_library::cimg::toc(); // Stop counting time
    // Update current head position
    currHeadX = snakeBody[0].getX();
    currHeadY = snakeBody[0].getY();
    // Update time from last change apples state
    timeFromPlaceEnlargeApple += time;
    timeFromPlaceShrinkApple += time;
    timeFromRemoveEnlargeApple += time;
    timeFromRemoveShrinkApple += time;
    // Check for game over and apples state
    return isCollide() + checkApples();
}
// Is Snake`s head have same coordinates(collide) with some else section in snakeBody
bool Snake::isCollide() const{
    for (int i = 1; i < snakeLength; i++) {
        if(snakeBody[i].getX() == currHeadX && snakeBody[i].getY() == currHeadY)
            return true;
    }
    return false;
}
// Is Snake`s head have same coordinates(collide) with some apple
bool Snake::isAteApple(std::vector<PixelBox>& apples) const{
    size_t len = apples.size();
    for (int i = 0; i < len; i++) {
        if (apples[i].getX() == currHeadX && apples[i].getY() == currHeadY)
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
// Place apple with specified characteristics
void Snake::placeRandomApple(std::vector<PixelBox>& apples, unsigned char colour[3], std::time_t& timeFromPlace,
    time_t appleLifetime, int maxAmount) {
    if (apples.size() >= maxAmount)
        return;
    if ((unsigned int)snakeLength + shrinkApples.size() + enlargeApples.size() >= (game.W / boxSize) * (game.H / boxSize)) // If snake too big, can`t place apple
        return;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, int(appleLifetime / 1.5));
    if (timeFromPlace > distr(gen)) { // If time from place more than random generated number by uniform_distribution, place apple
        PixelBox apple = placeRandomPixelBox(colour);
        apples.push_back(apple);
        timeFromPlace = 0;
    }
}
void Snake::placeRandomAppleTest() {
    using namespace boost::ut;
    clear(65, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 90); // Clear snake
    "placeRandomApple"_test = [this] {
        unsigned char colour[3] = { 255, 0, 0 }; // Red colour
        std::vector<PixelBox> apples;
        std::time_t timeFromPlace = 10;
        time_t appleLifetime = 15;
        int maxAmount = 20;
        int applePlacesAmount = 0;

        // Test when apples size is less than maxAmount
        placeRandomApple(apples, colour, timeFromPlace, appleLifetime, maxAmount);
        expect(apples.size() == 1 >> fatal)<<"Method do not place apple";
        expect(timeFromPlace == 0) << "Method do not change time from place";
        while(apples.size()<20) {
            timeFromPlace = 5;
            placeRandomApple(apples, colour, timeFromPlace, appleLifetime, maxAmount);
            ++applePlacesAmount;
        }
        expect(applePlacesAmount < 60 && applePlacesAmount>=30) << "Probably place apples not randomly in time. Amount of attempt: "<<applePlacesAmount;
        // Test when apples size is equal to maxAmount
        placeRandomApple(apples, colour, timeFromPlace, appleLifetime, maxAmount);
        expect(that % apples.size() == maxAmount)<< "Place apples, when it more then maxAmount";

        // Test when snake length is too big
        enlargeSnake((game.W / boxSize) * (game.H / boxSize)); // Assume growSnake increases snakeLength
        placeRandomApple(apples, colour, timeFromPlace, appleLifetime, maxAmount);
        expect(that % apples.size() == maxAmount);
    };
}
// Place random enlarge apple in random period of time 
void Snake::placeRandomEnlargeApple() {
    placeRandomApple(enlargeApples, colourEnlarge, timeFromPlaceEnlargeApple, 
        game.enlargeAppleLifeTime, game.maximumEnlargeApples);
}
// Place random shrink apple in random period of time 
void Snake::placeRandomShrinkApple() {
    placeRandomApple(shrinkApples, colourShrink, timeFromPlaceShrinkApple,
        game.shrinkAppleLifeTime, game.maximumShrinkApples);
}
// Is vector contain PixelBox with given coordinates
bool Snake::isContainCoordinates(std::vector<PixelBox> &vec, int x, int y) const{
    for (auto box : vec) {
        if (x == box.getX() && y == box.getY())
            return true;
    }
    return false;
}
// Processing apples on game field
bool Snake::checkApples() {
    if (isAteApple(enlargeApples)) { // If snake ate enlarge apple
        deleteCollidedApple(enlargeApples); // delete this apple
        enlargeSnake(1); // enlarge snake
        // Change snake`s speed
        if (snakeLength < 50 && snakeLength % 3 == 0) 
            changeSpeed('+', 1);
        else if (snakeLength < 100 && snakeLength % 6 == 0)
            changeSpeed('+', 1);
        else if (snakeLength % 13 == 0)
            changeSpeed('+', 1);
        return false;
    }
    else if (isAteApple(shrinkApples)) { // If snake ate shrink apple
        deleteCollidedApple(shrinkApples); // delete this apple
        // Change snake`s speed
        if(snakeLength>100)
            changeSpeed('-', 1);
        else
            changeSpeed('-', 2);
        return shrinkSnake(4); // shrink snake
    }
    // Delete random apples if it`s time for it
    if (timeFromRemoveEnlargeApple >= game.enlargeAppleLifeTime) {
        deleteRandomApple(enlargeApples);
        timeFromRemoveEnlargeApple = 0;
    }
    if (timeFromRemoveShrinkApple >= game.shrinkAppleLifeTime) {
        deleteRandomApple(shrinkApples);
        timeFromRemoveShrinkApple = 0;
    }
    return false;
}
// Check for apple with coordinates same to snake`s head and delete it 
void Snake::deleteCollidedApple(std::vector<PixelBox>& apples) {
    for (int i = 0; i < apples.size(); i++) {
        if (apples[i].getX() == snakeBody[0].getX() && apples[i].getY() == snakeBody[0].getY()) {
            apples.erase(apples.begin() + i);
            return;
        }
    }
}
// Delete random apple 
void Snake::deleteRandomApple(std::vector<PixelBox>& apples) {
    if (apples.size() == 0)
        return;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, static_cast<int>(apples.size()-1));
    int pos = distr(gen);
    drawGrass(apples[pos].getX(), apples[pos].getY());
    apples.erase(apples.begin() + pos);
     
}
void Snake::deleteRandomAppleTest() {
    using namespace boost::ut;
    std::vector<PixelBox> apples;
    int indikators[4] = { 0,0,0,0 };
    // Check if function delete element 
    for (int i = 0; i < 3; i++) {
        apples.push_back(PixelBox(5, i, i, colourEnlarge));
    }
    deleteRandomApple(apples);
    size_t size1 = apples.size();
    bool isRandom = false;
    // Check if function delete random element
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 4; j++) {
            apples.push_back(PixelBox(5, j ,j, colourEnlarge));
        }
        deleteRandomApple(apples);
        for (int i = 0; i < 4; i++) {
            bool  flag = false;
            for (auto box : apples) {
                if (box.getX() == i) {
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                ++indikators[i];
                break;
            }
        }
    }
    if (indikators[0] < 50 && indikators[1] < 50 && indikators[2] < 50 && indikators[3] < 50)
        isRandom = true;
    "deleteRandomApple"_test = [size1,isRandom] {
        expect(size1 == 2_i) << "Function not delete elements";
        expect(isRandom) << "Probably, function delete elements not randomly";
    };
}
// Place PsxelBox with given colour randomly on background, if it possible. Used uniform_int_distribution
PixelBox Snake::placeRandomPixelBox(unsigned char colour[3]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrX(0, game.W - boxSize);
    std::uniform_int_distribution<> distrY(game.scoreTableH, game.scoreTableH + game.H - boxSize);
    while (true) {
        // Try get x and y
        int x = distrX(gen);
        x = x / boxSize * boxSize ;
        int y = distrY(gen);
        y = y / boxSize * boxSize + game.scoreTableH % boxSize;
        // If contains coordsnates try again
        if (isContainCoordinates(snakeBody, x, y) || isContainCoordinates(enlargeApples, x, y)
            || isContainCoordinates(shrinkApples, x, y))
            continue;
        PixelBox apple(boxSize, x, y, colour);
        return apple;
    }
}
void Snake::placeRandomPixelBoxTest() {
    using namespace boost::ut; 
    clear(65, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 90); // Clear snake
    for (int i = 0; i < 3; i++) {
        shrinkApples.push_back(PixelBox(5, i*5, i*5, colourEnlarge));
    }
    for (int i = 3; i < 12; i++) {
        enlargeApples.push_back(PixelBox(5, i*5, i*5, colourEnlarge));
    }
    "placeRandomPixelBox"_test = [this] {
        unsigned char colour[3] = { 255, 0, 0 }; // Red colour
        // Test that the placed PixelBox does not overlap with any existing PixelBoxes
        int prevX = -1, prevY = -1, colisionCounter = 0;
        for (int i = 0; i < 50; ++i) {
            PixelBox pixelBox = placeRandomPixelBox(colour);
            expect(that % !isContainCoordinates(snakeBody, pixelBox.getX(), pixelBox.getY())) << "Collision with snake";
            expect(that % !isContainCoordinates(enlargeApples, pixelBox.getX(), pixelBox.getY())) << "Collision with enlarge apple";
            expect(that % !isContainCoordinates(shrinkApples, pixelBox.getX(), pixelBox.getY())) << "Collision with shrink apple";
            if (pixelBox.getX() == prevX && pixelBox.getY() == prevY) {
                ++colisionCounter;
            }
            prevX = pixelBox.getX();
            prevY = pixelBox.getY();
        }
        expect(colisionCounter < 10) << "Probably, method return not random PixelBox";
    };
}
// Change snake speeed in ms. '+' - increase, '-' - decrease
void Snake::changeSpeed(char operation, int amountMS) {
    if (operation == '-' && snakeSpeed < 75)
        snakeSpeed += amountMS;
    else if (operation == '+' && snakeSpeed > 15)
        snakeSpeed -= amountMS;
}
void Snake::snakeTest() {
    initSnakeBodyTest();
    clearTest();
    deleteRandomAppleTest();
    shrinkSnakeTest();
    enlargeSnakeTest();
    placeRandomPixelBoxTest();
    placeRandomAppleTest();
    moveDownTest();
    moveUpTest();
    moveLeftTest();
    moveRightTest();
    PixelBox::drawTest();
}