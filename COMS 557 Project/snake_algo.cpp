#include "snake.h"

Snake::Snake(int x, int y, int maxlen) : initX(x), initY(y) {
    this->maxlen = maxlen + minlen;
    init();
}

Snake::Snake() {
    maxlen = 99 + minlen;
    initX = 0;
    initY = 0;
    init();
}

Snake::~Snake() {
    delete[] snake_tail;
}

void Snake::init() {
    snake_tail = new double* [maxlen];
    
    for(int i = 0; i < maxlen; i++) {
        snake_tail[i] = new double[2];
        snake_tail[i][0] = snake_tail[i][1] = 0;
    }
    
    reset();
}

void Snake::grab_point() {
    len++;
}

bool Snake::full() {
    return len == maxlen - 1;
}

bool Snake::self_collision(int nX, int nY) {
    for (int i = len; i > 0; i--) {
        if (snake_tail[i][0] == nX && snake_tail[i][1] == nY) return true;
    }
    
    return false;
}

bool Snake::move_direction_to(int x, int y) {
    int nX, nY;
    
    for (int i = len; i > 0; i--) {
        snake_tail[i][0] = snake_tail[i - 1][0];
        snake_tail[i][1] = snake_tail[i - 1][1];
    }
    
    nX = snake_tail[1][0] + x;
    nY = snake_tail[1][1] + y;;
    
    snake_tail[0][0] = nX;
    snake_tail[0][1] = nY;
    
    return !self_collision(nX, nY);
}

void Snake::reset() {
    len = minlen;
    
    snake_tail[0][0] = initX;
    snake_tail[0][1] = initY;
    snake_tail[1][0] = initX - 1;
    snake_tail[1][1] = initY;
}

int Snake::current_x_At(int i) {
    return snake_tail[i][0];
}

int Snake::current_y_At(int i) {
    return snake_tail[i][1];
}

int Snake::x() {
    return current_x_At(0);
}

int Snake::y() {
    return current_y_At(0);
}
