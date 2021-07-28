#include "kronos.h"

#include "test.h"

void init() {
    board_init();
    sysclk_init();
    console_init();
}

class A {


};

int main() {
    init();

    printf("here");

    return 0;
}