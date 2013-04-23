#include "block.h"
#include <iostream>

Block::Block(int x, int y, GLcolor color) : x_(x), y_(y) {
  color_ = color;
}

GLcolor Block::get_color(void) {
  return color_;
}

void Block::set_color(GLcolor color) {
  color_ = color;
}

int Block::get_y(void) {
  return y_;
}

void Block::set_y(int y) {
  y_ = y;
}

int Block::get_x(void) {
  return x_;
}

void Block::set_x(int x) {
  x_ = x;
}
