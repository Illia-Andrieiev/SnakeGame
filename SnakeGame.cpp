//cimg_library::cimg::dialog
#include<string>
#include<iostream>
#include<ctime>
#include "SnakeGame.h"
#include"PixelBox.h"
using namespace cimg_library;
// Constructors

SnakeGame::SnakeGame(int argc, char** argv, int boxSize) {
    const unsigned char green[3] = { 50, 205, 50 };
    const unsigned char grey[3] = { 200, 200, 200 };
    this->argc = argc;
    this->argv = argv;
    this->boxSize = boxSize;
    maximumEnlargeApples = 10;
    maximumShrinkApples = 4;
    shrinkAppleLifeTime = 12000;
    enlargeAppleLifeTime = 9000;
    unsigned char firstBoxColour[3] = { 0,0,139 };
    unsigned char secondBoxColour[3] = { 139, 0, 0 };
    unsigned char colourEnlarge[3] = { 255, 0, 0 };
    unsigned char colourShrink[3] = { 218, 112, 214 };
    setSnakeColour(firstBoxColour,secondBoxColour);
    setApplesColour(colourEnlarge,colourShrink);
    background.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, W - 1, H - 1, green, 1.0f)
        .noise(10).resize(W, H + scoreTableH).blur(2).normalize(0, 128).draw_rectangle(0, 0, W, scoreTableH, grey, 1.0f);
    CImgDisplay displ(background, "* CImg-Snake *");
    this->disp = displ;
}

void SnakeGame::game() {
    cimg_usage("A simple Snake game, using the CImg Library");
   // const char* geom = cimg_option("-g", "500x500", "Size of the game board");
    const int delay = cimg_option("-s", 10, "Game speed (lower value means faster)");
    const bool twoplayers = !cimg_option("-1", true, "One player only");
    const int zoom = cimg_option("-z", 3, "Zoom factor");
    const bool full = cimg_option("-f", false, "Fullscreen mode");
   // sscanf_s(geom, "%u%*c%u", &W, &H);
    // Define game colors and variables
    //----------------------------------
    const unsigned char blue[] = { 128,200,255 }, grey[3] = { 200, 200, 200 }, black[] = { 0,0,0 }, red[] = { 255,0,0 }, white[] = { 255,255,255 };
    int score1 = 0, score2 = 0, round_over = 0, ix1 = -1, iy1 = -1, x1 = 0, y1 = 0, u1 = 0, v1 = 0,
        ix2 = -1, iy2 = -1, x2 = 0, y2 = 0, u2 = 0, v2 = 0;
    bool start_round = true, turbo1 = false, turbo2 = false;

    cimg_library::CImg<unsigned char> score;
    score.assign(64, 64, 1, 3, 0).normalize(0, 128).draw_rectangle(0, 0, W, scoreTableH*3, grey, 1.0f).resize(W, scoreTableH);
    // Open display window
    //---------------------
    if (zoom > 1) disp.resize(-100 * zoom, -100 * zoom);
    if (full) disp.toggle_fullscreen().display(background);
    // Start main game loop
    //----------------------
   // Snake snake(this, delay * 5, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize,8);
   
    Snake snake(*this, delay * 5, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize,12);
    std::string message = "Score: " + std::to_string(snake.getScore());
    
    while (!disp.is_closed() && !disp.is_keyESC()) {
        message = "Score: " + std::to_string(snake.getScore());
        background.draw_image(0, 0, 0, 0, score);
        background.draw_text(W/5, 0,message.data(), black, 0, 1, 30).resize(-100, -100, 1, 3);
        bool isCollide = snake.move(moveDirection);
        if (disp.is_keyA()) { moveDirection = moveDirection == 1? 1:0; }
        else if (disp.is_keyD()) { moveDirection = moveDirection == 0 ? 0 : 1; }
        else if (disp.is_keyW()) { moveDirection = moveDirection == 3 ? 3 : 2; }
        else if (disp.is_keyS()) { moveDirection = moveDirection == 2 ? 2 : 3; }
        //if (disp.is_keyA()) {
        //    moveDirection = moveDirection == 1 ? 1 : 0; snake.move(moveDirection);
        //}
        //else if (disp.is_keyD()) { moveDirection = moveDirection == 0 ? 0 : 1; snake.move(moveDirection);
        //}
        //else if (disp.is_keyW()) { moveDirection = moveDirection == 3 ? 3 : 2; snake.move(moveDirection);
        //}
        //else if (disp.is_keyS()) { moveDirection = moveDirection == 2 ? 2 : 3; snake.move(moveDirection);
        //}

        //// Check if round is over.
        if (isCollide) {
            const int xc = round_over == 1 ? x1 : x2, yc = round_over == 1 ? y1 : y2;
            for (int r = 0; r < 50; r += 3) background.draw_circle(snake.getXHeadPosition(), snake.getYHeadPosition(), r, round_over == 1 ? blue : red, r / 300.0f).display(disp.wait(20));
            for (int rr = 0; rr < 50; rr += 3)
                ((+background) *= (50 - rr) / 50.0f).draw_circle(snake.getXHeadPosition(), snake.getYHeadPosition(), (50 + rr), round_over == 1 ? blue : red, 1 / 6.0f).display(disp.wait(20));
            start_round = true;
            CImg<unsigned char> logo;
            logo.draw_text(0, 0, " Game Over! ", white, 0, 1, 33).resize(-100, -100, 1, 3);
            while (!disp.is_closed() && !disp.is_keyESC()) {

            }
            return;
        }

    }
}

/*
        PRIVATE METHODS
*/

// Set colours of Apples and Snake
void SnakeGame::setSnakeColour(unsigned char colour1[3], unsigned char colour2[3]) {
    for (int i = 0; i < 3; i++) {
        this->firstBoxColour[i] = colour1[i];
        this->secondBoxColour[i] = colour2[i];
    }
}
void SnakeGame::setApplesColour(unsigned char colourEnlarge[3], unsigned char colourShrink[3]) {
    for (int i = 0; i < 3; i++) {
        this->colourEnlarge[i] = colourEnlarge[i];
        this->colourShrink[i] = colourShrink[i];
    }
}