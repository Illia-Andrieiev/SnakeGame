//cimg_library::cimg::dialog
#include<functional>
#include "SnakeGame.h"
#include"PixelBox.h"
using namespace cimg_library;
// Constructors

SnakeGame::SnakeGame(int argc, char** argv) {
    const unsigned char green[3] = { 50, 205, 50 };
    const unsigned char white[3] = { 255, 255, 255 };
    this->argc = argc;
    this->argv = argv;
    boxSize = 5;
    unsigned char firstBoxColour[3] = { 0,0,139 };
    unsigned char secondBoxColour[3] = { 139, 0, 0 };
    unsigned char colourEnlarge[3] = { 255, 0, 0 };
    unsigned char colourShrink[3] = { 218, 112, 214 };
    setSnakeColour(firstBoxColour,secondBoxColour);
    setApplesColour(colourEnlarge,colourShrink);
    background.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, W - 1, H - 1, green, 1.0f)
        .noise(10).resize(W, H).blur(2).normalize(0, 128);
    CImgDisplay displ(background, "* CImg-Snake *");
    this->disp = displ;
}
SnakeGame::SnakeGame(int argc, char** argv, int boxSize) {
    const unsigned char green[3] = { 50, 205, 50 };
    const unsigned char white[3] = { 255, 255, 255 };
    this->argc = argc;
    this->argv = argv;
    this->boxSize = boxSize;
    unsigned char firstBoxColour[3] = { 0,0,139 };
    unsigned char secondBoxColour[3] = { 139, 0, 0 };
    unsigned char colourEnlarge[3] = { 255, 0, 0 };
    unsigned char colourShrink[3] = { 218, 112, 214 };
    setSnakeColour(firstBoxColour, secondBoxColour);
    setApplesColour(colourEnlarge, colourShrink);
    background.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, W - 1, H - 1, green, 1.0f).noise(10).resize(W, H).blur(2).normalize(0, 128).
        draw_rectangle(0, 0, W - 1, H - 1, white, 1.0f, ~0U);
    CImgDisplay displ(background, "* CImg-Snake *");
    this->disp = displ;
}
SnakeGame::SnakeGame(int argc, char** argv, unsigned char firstBoxColour[3],
                     unsigned char secondBoxColour[3], int boxSize = 5) {
    const unsigned char green[3] = { 50, 205, 50 };
    const unsigned char white[3] = { 255, 255, 255 };
    this->argc = argc;
    this->argv = argv;
    this->boxSize = boxSize;
    unsigned char colourEnlarge[3] = { 255, 0, 0 };
    unsigned char colourShrink[3] = { 218, 112, 214 };
    setSnakeColour(firstBoxColour, secondBoxColour);
    setApplesColour(colourEnlarge, colourShrink);
    background.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, W - 1, H - 1, green, 1.0f).resize(W, H).blur(2).normalize(0, 128).
        draw_rectangle(0, 0, W - 1, H - 1, white, 1.0f, ~0U);

    CImgDisplay displ(background, "* CImg-Snake *");
    this->disp = displ;
}
SnakeGame::SnakeGame(int argc, char** argv, unsigned char firstBoxColour[3],unsigned char secondBoxColour[3],
                     unsigned char colourEnlarge[3], unsigned char colourShrink[3], int boxSize = 5) {
    const unsigned char green[3] = { 50, 205, 50 };
    const unsigned char white[3] = { 255, 255, 255 };
    this->argc = argc;
    this->argv = argv;
    this->boxSize = boxSize;
    setSnakeColour(firstBoxColour, secondBoxColour);
    setApplesColour(colourEnlarge, colourShrink);
    background.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, W - 1, H - 1, green, 1.0f)
        .noise(10).resize(W, H).blur(2).normalize(0, 128);
    CImgDisplay displ(background, "* CImg-Snake *");
    this->disp = displ;
}


void SnakeGame::game() {
    cimg_usage("A simple Snake game, using the CImg Library");

    const char* geom = cimg_option("-g", "300x300", "Size of the game board");
    const int delay = cimg_option("-s", 10, "Game speed (lower value means faster)");
    const bool twoplayers = !cimg_option("-1", true, "One player only");
    const int zoom = cimg_option("-z", 3, "Zoom factor");
    const bool full = cimg_option("-f", false, "Fullscreen mode");
    unsigned int W = 400, H = 400;
    sscanf_s(geom, "%u%*c%u", &W, &H);

    // Define game colors and variables
    //----------------------------------
    const unsigned char blue[] = { 128,200,255 }, red[] = { 255,0,0 }, white[] = { 255,255,255 };
    int score1 = 0, score2 = 0, round_over = 0, ix1 = -1, iy1 = -1, x1 = 0, y1 = 0, u1 = 0, v1 = 0,
        ix2 = -1, iy2 = -1, x2 = 0, y2 = 0, u2 = 0, v2 = 0;
    bool start_round = true, turbo1 = false, turbo2 = false;

   
    // Open display window
    //---------------------
    if (zoom > 1) disp.resize(-100 * zoom, -100 * zoom);
    if (full) disp.toggle_fullscreen().display(background);
    // Start main game loop
    //----------------------
    Snake snake(this, delay * 5, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize);
    int moveDirection = 1;
    while (!disp.is_closed() && !disp.is_keyESC()) {
        bool is_Collide = snake.move(moveDirection);
        //// Init new game round if necessary
        ////----------------------------------
        //if (start_round) {
        //    // Init game variables
        //    round_over = 0;
        //    ix1 = -1; iy1 = -1; x1 = 10; y1 = 10; u1 = 1; v1 = 0; turbo1 = false;
        //    ix2 = -1; iy2 = -1; x2 = W - 11; y2 = H - 11; u2 = -1; v2 = 0; turbo2 = false;
        //    img = background;
        //    start_round = false;

        //    // Display a simple pre-round page
        //    CImg<unsigned char> logo, pressakey;
        //    logo.draw_text(0, 0, " CImg-Tron ", white, 0, 1, 33).resize(-100, -100, 1, 3);
        //    CImg<unsigned char> tmp = (+background).draw_image((W - logo.width()) / 2, (H - logo.height()) / 2 - 20,
        //        logo, logo.get_channel(0).dilate(6).normalize(0, 1)).
        //        draw_text(W / 2 - 60, H / 2 + 10, "Blue ( %u )", blue, 0, 1, 13, score1).
        //        draw_text(W / 2 + 10, H / 2 + 10, "Red ( %u )", red, 0, 1, 13, score2);
        //    pressakey.draw_text(0, 0, "* Press a key to start round *", white);
        //    for (float i = 0; i < 1; i += 0.05f) ((+tmp) *= i).display(disp.wait(20));
        //    disp.flush();
        //    for (unsigned long t = 0; !disp.key() && !disp.is_closed(); ++t) {
        //        if (!(t % 10)) {
        //            if (t % 20) disp.display(tmp);
        //            else disp.display((+tmp).draw_image(W / 2 - 70, H / 2 + 50, pressakey, pressakey, 1, 255));
        //        }
        //        if (disp.wait(20).is_resized()) disp.resize(disp);
        //    }
        //    if (disp.is_keyESC()) disp.flush();
        //}

        //// Test collision between players and borders
        //if (x1 < 0 || x1 >= img.width() || y1 < 0 || y1 >= img.height() ||
        //    img(x1, y1, 0) != background(x1, y1, 0) ||
        //    img(x1, y1, 1) != background(x1, y1, 1) ||
        //    img(x1, y1, 2) != background(x1, y1, 2) ||
        //    ((ix1 >= 0 || iy1 >= 0) && (img(ix1, iy1, 0) != background(ix1, iy1, 0) ||  // Collision test for turbo mode
        //        img(ix1, iy1, 1) != background(ix1, iy1, 1) ||
        //        img(ix1, iy1, 2) != background(ix1, iy1, 2)))) {
        //    round_over = 1; score2++;
        //}
        //if (twoplayers) {
        //    if (x2 < 0 || x2 >= img.width() || y2 < 0 || y2 >= img.height() ||
        //        img(x2, y2, 0) != background(x2, y2, 0) ||
        //        img(x2, y2, 1) != background(x2, y2, 1) ||
        //        img(x2, y2, 2) != background(x2, y2, 2) ||
        //        ((ix2 >= 0 || iy2 >= 0) && (img(ix2, iy2, 0) != background(ix2, iy2, 0) ||  // Collision test for turbo mode
        //            img(ix2, iy2, 1) != background(ix2, iy2, 1) ||
        //            img(ix2, iy2, 2) != background(ix2, iy2, 2)))) {
        //        round_over = 2; score1++;
        //    }
        //}

        //// Draw new players positions
        //img.draw_point(x1, y1, blue);
        //if (ix1 >= 0 && iy1 >= 0) img.draw_point(ix1, iy1, blue);
        //if (twoplayers) {
        //    img.draw_point(x2, y2, red);
        //    if (ix2 >= 0 && iy2 >= 0) img.draw_point(ix2, iy2, red);
        //}
        //if (disp.is_resized()) disp.resize(disp);
        //img.display(disp);

        //// Update players positions
        //x1 += u1; y1 += v1;
        //if (turbo1) { ix1 = x1; iy1 = y1; x1 += u1; y1 += v1; }
        //else { ix1 = iy1 = -1; }
        //if (twoplayers) {
        //    x2 += u2; y2 += v2;
        //    if (turbo2) { ix2 = x2; iy2 = y2; x2 += u2; y2 += v2; }
        //    else { ix2 = iy2 = -1; }
        //}

        //// Test keyboard events
        //int nu1 = u1, nv1 = v1, nu2 = u2, nv2 = v2;
        //if (disp.is_keyARROWLEFT()) { nu1 = -1; nv1 = 0; }
        //if (disp.is_keyARROWRIGHT()) { nu1 = 1; nv1 = 0; }
        //if (disp.is_keyARROWUP()) { nu1 = 0; nv1 = -1; }
        //if (disp.is_keyARROWDOWN()) { nu1 = 0; nv1 = 1; }
        //turbo1 = disp.is_keyCTRLRIGHT();
        //if (twoplayers) {
        if (disp.is_keyA()) { moveDirection = moveDirection == 1? 1:0; }
        if (disp.is_keyD()) { moveDirection = moveDirection == 0 ? 0 : 1; }
        if (disp.is_keyW()) { moveDirection = moveDirection == 3 ? 3 : 2; }
        if (disp.is_keyS()) { moveDirection = moveDirection == 2 ? 2 : 3; }
        //    turbo2 = disp.is_keyTAB();
        //}
        //if (nu1 != -u1 && nv1 != -v1) { u1 = nu1; v1 = nv1; }
        //if (nu2 != -u2 && nv2 != -v2) { u2 = nu2; v2 = nv2; }

        //// Check if round is over.
        //if (round_over) {
        //    const int xc = round_over == 1 ? x1 : x2, yc = round_over == 1 ? y1 : y2;
        //    for (int r = 0; r < 50; r += 3) img.draw_circle(xc, yc, r, round_over == 1 ? blue : red, r / 300.0f).display(disp.wait(20));
        //    for (int rr = 0; rr < 50; rr += 3)
        //        ((+img) *= (50 - rr) / 50.0f).draw_circle(xc, yc, (50 + rr), round_over == 1 ? blue : red, 1 / 6.0f).display(disp.wait(20));
        //    start_round = true;
        //}

        //// Wait a small amount of time
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