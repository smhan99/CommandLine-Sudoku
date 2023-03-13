#include "cell.hpp"

cell::cell() {}

void cell::fill(int v, bool g) {
    value = v;
    empty = v == 0 ? true : false;
    guess = g;
}

void cell::erase() {
    value = 0;
    empty = true;
    guess = false;
}

bool cell::isEmpty() {
    return empty;
}

int cell::getVal() {
    return value;
}

bool cell::isGuess() {
    return guess;
}

void cell::keep() {
    guess = false;
}