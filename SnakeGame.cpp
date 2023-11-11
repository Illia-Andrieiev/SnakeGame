//cimg_library::cimg::dialog
#include "SnakeGame.h"
#include"PixelBox.h"
using namespace cimg_library;
// Constructors
SnakeGame::SnakeGame(int argc, char** argv) {
    this->argc = argc;
    this->argv = argv;
    boxSize = 5;
    unsigned char firstBoxColour[3] = { 0,0,139 };
    unsigned char secondBoxColour[3] = { 173, 216, 230 }; 
    unsigned char colourEnlarge[3] = { 255, 0, 0 };
    unsigned char colourShrink[3] = { 218, 112, 214 };
    setSnakeColour(firstBoxColour,secondBoxColour);
    setApplesColour(colourEnlarge,colourShrink);
}
SnakeGame::SnakeGame(int argc, char** argv, int boxSize) {
    this->argc = argc;
    this->argv = argv;
    this->boxSize = boxSize;
    unsigned char firstBoxColour[3] = { 0,0,139 };
    unsigned char secondBoxColour[3] = { 173, 216, 230 };
    unsigned char colourEnlarge[3] = { 255, 0, 0 };
    unsigned char colourShrink[3] = { 218, 112, 214 };
    setSnakeColour(firstBoxColour, secondBoxColour);
    setApplesColour(colourEnlarge, colourShrink);
}
SnakeGame::SnakeGame(int argc, char** argv, unsigned char firstBoxColour[3],
                     unsigned char secondBoxColour[3], int boxSize = 5) {
    this->argc = argc;
    this->argv = argv;
    this->boxSize = boxSize;
    unsigned char colourEnlarge[3] = { 255, 0, 0 };
    unsigned char colourShrink[3] = { 218, 112, 214 };
    setSnakeColour(firstBoxColour, secondBoxColour);
    setApplesColour(colourEnlarge, colourShrink);
}
SnakeGame::SnakeGame(int argc, char** argv, unsigned char firstBoxColour[3],unsigned char secondBoxColour[3],
                     unsigned char colourEnlarge[3], unsigned char colourShrink[3], int boxSize = 5) {
    this->argc = argc;
    this->argv = argv;
    this->boxSize = boxSize;
    setSnakeColour(firstBoxColour, secondBoxColour);
    setApplesColour(colourEnlarge, colourShrink);
}



void SnakeGame::game() {
    cimg_usage("A simple Snake game, using the CImg Library");

    const char* geom = cimg_option("-g", "300x300", "Size of the game board");
    const int delay = cimg_option("-s", 10, "Game speed (lower value means faster)");
    const bool twoplayers = !cimg_option("-1", true, "One player only");
    const int zoom = cimg_option("-z", 2, "Zoom factor");
    const bool full = cimg_option("-f", false, "Fullscreen mode");
    unsigned int W = 400, H = 400;
    sscanf_s(geom, "%u%*c%u", &W, &H);

    // Define game colors and variables
    //----------------------------------
    const unsigned char blue[] = { 128,200,255 }, red[] = { 255,0,0 }, white[] = { 255,255,255 };

    // Create background image
    //--------------------------
    CImg<unsigned char> background, img;
    background.assign(64, 64, 1, 3, 0).draw_plasma().resize(W, H).blur(2).normalize(0, 128).
        draw_rectangle(0, 0, W - 1, H - 1, white, 1.0f, ~0U);
   
    // Open display window
    //---------------------
    CImgDisplay disp(background, "* CImg-Tron *");
    if (zoom > 1) disp.resize(-100 * zoom, -100 * zoom);
    if (full) disp.toggle_fullscreen().display(background);

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
