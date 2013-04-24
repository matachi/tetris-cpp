#include "game_model.h"

#include <stdlib.h>
#include <iostream>
#include <SDL2/SDL.h>

GameModel::GameModel(void) : blocks_(std::vector<Block*>()) {
  Block* b = new Block(0,0,(GLcolor){0.1f, 0.2f, 0.4f});
  b->set_x(5);
  blocks_.push_back(new Block(0,0,(GLcolor){0.0f, 0.0f, 0.0f}));
  blocks_.push_back(b);
  blocks_.push_back(new Block(-5,0,(GLcolor){0.0f, 0.0f, 0.0f}));
  next_tick_ = SDL_GetTicks() + 1000;
}

GameModel::~GameModel() {
  for (std::vector<Block*>::iterator it = blocks_.begin(); it != blocks_.end();
      ++it) {
    Block* block = *it;
    delete block;
  }
}

void GameModel::init(void) {
  // Create the first figure
  figure_ = *create_figure(&blocks_);
}

void GameModel::update(Uint32 time, Direction direction) {
  if (time > next_tick_) {
    next_tick_ += 1000;
    move(figure_, DOWN);
    if (collides_with_grid(figure_, blocks_)) {
      move(figure_, UP);
    }
  }
  switch (direction) {
    case UP:
      //std::cout << "UP" << std::endl;
      break;
    case DOWN:
      //move(figure_, direction);
      break;
    case LEFT:
    case RIGHT:
      //move(figure_, direction);
      break;
    default:
      break;
  }
}

std::vector<Block*> GameModel::get_blocks(void) {
  return blocks_;
}

void GameModel::move(std::vector<Block*> figure, Direction direction) {
  for (std::vector<Block*>::iterator it = figure_.begin(); it != figure_.end();
      ++it) {
    Block* block = *it;
    switch (direction) {
      case UP:
        block->set_y(block->get_y() + 1);
        break;
      case DOWN:
        block->set_y(block->get_y() - 1);
        break;
      case LEFT:
        block->set_x(block->get_x() - 1);
        break;
      case RIGHT:
        block->set_x(block->get_x() + 1);
        break;
    }
  }
}

bool GameModel::collides_with_grid(std::vector<Block*> figure, std::vector<Block*> blocks) {
  for (std::vector<Block*>::iterator it = figure.begin(); it != figure.end();
      ++it) {
    Block* a = *it;
    for (std::vector<Block*>::iterator it2 = blocks.begin();
        it2 != blocks.end(); ++it2) {
      Block* b = *it2;
      if (a != b) {
        if (a->get_x() == b->get_x() && a->get_y() == b->get_y()) {
          return true;
        }
      }
    }
  }
}

std::vector<Block*>* GameModel::create_figure(std::vector<Block*>* blocks) {
  std::vector<Block*>* figure = new std::vector<Block*>;
  int i = 2;
  switch (i) {
    case 1: // Square
      GLcolor color = {0.2f, 0.4, 0.6f};
      create_block(4, 21, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      create_block(4, 20, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      break;
    case 2: // 4 line
      GLcolor color = {0.8f, 0.4, 0.6f};
      create_block(5, 21, color, blocks, figure);
      create_block(5, 20, color, blocks, figure);
      create_block(5, 19, color, blocks, figure);
      create_block(5, 18, color, blocks, figure);
      break;
    case 3:
      GLcolor color = {0.8f, 0.4, 0.2f};
      create_block(4, 21, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      create_block(5, 20, color, blocks, figure);
      create_block(6, 20, color, blocks, figure);
      break;
    case 4:
      GLcolor color = {0.2f, 0.4, 0.2f};
      create_block(4, 20, color, blocks, figure);
      create_block(5, 20, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      create_block(6, 21, color, blocks, figure);
      break;
    case 5:
      GLcolor color = {0.8f, 0.4, 0.6f};
      create_block(4, 21, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      create_block(5, 20, color, blocks, figure);
      create_block(5, 19, color, blocks, figure);
      break;
    case 6:
      GLcolor color = {0.8f, 0.4, 0.6f};
      create_block(6, 21, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      create_block(5, 20, color, blocks, figure);
      create_block(5, 19, color, blocks, figure);
      break;
}

void rotate_figuare(std::vector<Block*>* figure) {
  int max_x = -1, min_x = 100, max_y = -1, min_y = 100;
  for (std::vector<Block*>::iterator it = figure.begin(); it != figure.end();
      ++it) {
    Block* a = *it;
    max_x = (a->get_x() > max_x) ? a->get_x() : max_x;
    min_x = (a->get_x() < min_x) ? a->get_x() : min_x;
    max_y = (a->get_y() > max_y) ? a->get_y() : max_y;
    min_y = (a->get_y() < min_y) ? a->get_y() : min_y;
  }
  for (std::vector<Block*>::iterator it = figure.begin(); it != figure.end();
      ++it) {
   //t 
  }
}

void GameModel::create_block(int x, int y, GLcolor color,
    std::vector<Block*>* blocks, std::vector<Block*>* figure) {
  Block* block = new Block(x, y, color);
  blocks->push_back(block);
  figure->push_back(block);
}
