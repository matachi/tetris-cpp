#include "game_model.h"

#include <stdlib.h>
#include <time.h>
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
  switch (direction) {
    case UP:
      rotate_figure(figure_);
      if (collides_with_grid(figure_, blocks_)) {
        rotate_figure(figure_);
        rotate_figure(figure_);
        rotate_figure(figure_);
      }
      break;
    case DOWN:
      while (!collides_with_grid(figure_, blocks_)) {
        move(figure_, DOWN);
      }
      move(figure_, UP);
      figure_ = *create_figure(&blocks_);
      break;
    case LEFT:
      move(figure_, LEFT);
      if (collides_with_grid(figure_, blocks_)) {
        move(figure_, RIGHT);
      }
      break;
    case RIGHT:
      move(figure_, RIGHT);
      if (collides_with_grid(figure_, blocks_)) {
        move(figure_, LEFT);
      }
      break;
    case NONE:
      if (time > next_tick_) {
        next_tick_ += 1000;
        move(figure_, DOWN);
        if (collides_with_grid(figure_, blocks_)) {
          move(figure_, UP);
          figure_ = *create_figure(&blocks_);
        }
      }
      break;
  }
  delete_full_rows(blocks_);
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
    if (a->get_x() < 0 || a->get_x() >= BLOCKS_IN_ROW || a->get_y() < 0 ||
        a->get_y() > BLOCKS_IN_COL) {
      return true;
    }
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
  return false;
}

std::vector<Block*>* GameModel::create_figure(std::vector<Block*>* blocks) {
  std::vector<Block*>* figure = new std::vector<Block*>;
  srand(time(NULL));
  int i = rand() % 7;
  std::cout << i << std::endl;
  GLcolor color;
  switch (i) {
    case 0: // Square
      color = {0.2f, 0.4, 0.6f};
      create_block(4, 21, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      create_block(4, 20, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      break;
    case 1: // 4 line
      color = {0.8f, 0.4, 0.6f};
      create_block(5, 21, color, blocks, figure);
      create_block(5, 20, color, blocks, figure);
      create_block(5, 19, color, blocks, figure);
      create_block(5, 18, color, blocks, figure);
      break;
    case 2:
      color = {0.8f, 0.4, 0.2f};
      create_block(4, 21, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      create_block(5, 20, color, blocks, figure);
      create_block(6, 20, color, blocks, figure);
      break;
    case 3:
      color = {0.2f, 0.4, 0.2f};
      create_block(4, 20, color, blocks, figure);
      create_block(5, 20, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      create_block(6, 21, color, blocks, figure);
      break;
    case 4:
      color = {0.8f, 0.4, 0.6f};
      create_block(4, 21, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      create_block(5, 20, color, blocks, figure);
      create_block(5, 19, color, blocks, figure);
      break;
    case 5:
      color = {0.8f, 0.4, 0.6f};
      create_block(6, 21, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      create_block(5, 20, color, blocks, figure);
      create_block(5, 19, color, blocks, figure);
      break;
    case 6:
      color = {0.8f, 0.4, 0.6f};
      create_block(4, 20, color, blocks, figure);
      create_block(5, 20, color, blocks, figure);
      create_block(5, 21, color, blocks, figure);
      create_block(6, 20, color, blocks, figure);
      break;
  }
}

void GameModel::rotate_figure(std::vector<Block*>& figure) {
  int max_x = -1, min_x = 100, max_y = -1, min_y = 100;
  for (std::vector<Block*>::iterator it = figure.begin(); it != figure.end();
      ++it) {
    Block* a = *it;
    max_x = (a->get_x() > max_x) ? a->get_x() : max_x;
    min_x = (a->get_x() < min_x) ? a->get_x() : min_x;
    max_y = (a->get_y() > max_y) ? a->get_y() : max_y;
    min_y = (a->get_y() < min_y) ? a->get_y() : min_y;
  }
  int width = max_x - min_x;
  int height = max_y - min_y;
  for (std::vector<Block*>::iterator it = figure.begin(); it != figure.end();
      ++it) {
    Block* a = *it;
    int x = a->get_x();
    int y = a->get_y();
    a->set_y(min_y + (height - (x - min_x)));
    a->set_x(min_x + (y - min_y));
  }
}

void GameModel::delete_full_rows(std::vector<Block*>& blocks) {
  for (int y = BLOCKS_IN_COL - 1; y >= 0; --y) {
    int blocks_in_row = 0;
    for (std::vector<Block*>::iterator it = blocks.begin(); it != blocks.end();
        ++it) {
      Block* block = *it;
      if (block->get_y() == y) {
        ++blocks_in_row;
      }
    }
    if (blocks_in_row == BLOCKS_IN_ROW) {
      move_down_blocks_above_level(blocks, y);
    }
  }
}

void GameModel::move_down_blocks_above_level(std::vector<Block*>& blocks,
    int level) {
  for (std::vector<Block*>::iterator it = blocks.begin(); it != blocks.end();
      ++it) {
    Block* block = *it;
    if (block->get_y() > level) {
      block->set_y(block->get_y() - 1);
    }
  }
}

void GameModel::create_block(int x, int y, GLcolor color,
    std::vector<Block*>* blocks, std::vector<Block*>* figure) {
  Block* block = new Block(x, y, color);
  blocks->push_back(block);
  figure->push_back(block);
}
