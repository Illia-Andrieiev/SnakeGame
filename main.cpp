#include "SnakeGame.h"
int main(int argc, char** argv) {

    SnakeGame snake(argc, argv);
    snake.game();
    return 0;
}
//draw_rectangle(0, 0, W - 1, H - 1, green, 1.0f)