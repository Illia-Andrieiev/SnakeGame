#include"libs/boost/ut.hpp"
#include "SnakeGame.h"
constexpr auto sum(int a, int b) { return (a + b); }

void test() {
    using namespace boost::ut;
    "true"_test = [] {
        expect(true);
        };
    "false"_test = [] {
        expect(false);
        };
    "sum"_test = [] {
        expect(sum(0, 0) == 0_i);
        expect(sum(1, 2) == 3_i);
        expect(sum(1, 2) > 0_i and 41_i == sum(40, 2));
        };
}
int main(int argc, char** argv) {
    //test();
    SnakeGame snake(argc, argv);
    snake.gameTest();
    return 0;
}
