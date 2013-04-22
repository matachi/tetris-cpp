#include "game_model.h"

#include <stdlib.h>
#include <iostream>
#include <SDL2/SDL.h>

GameModel::GameModel(void) : blocks_(std::vector<Block*>()), clock_(0) {
  Block* b = new Block(1,1,(GLcolor){0.1f, 0.2f, 0.4f});
  b->set_x(5);
  blocks_.push_back(new Block(10,1,(GLcolor){0.0f, 0.0f, 0.0f}));
  blocks_.push_back(b);
  blocks_.push_back(new Block(1,1,(GLcolor){0.0f, 0.0f, 0.0f}));
  //figure_ = std::vector<Block*>();
  //figure_ = &create_figure(&blocks_);
}

GameModel::~GameModel() {
  for (std::vector<Block*>::iterator it = blocks_.begin(); it != blocks_.end();
      ++it) {
    Block* block = *it;
    delete block;
  }
}

void GameModel::update(float time, Direction direction) {
  //std::cout << (long)&figure_ << std::endl;
  if (&figure_ != NULL) {
    figure_ = *create_figure(&blocks_);
    std::cout << "......" << std::endl;
  }
  //switch (direction) {
  //  case UP:
  //    std::cout << "UP" << std::endl;
  //    break;
  //  case DOWN:
  //    move(figure_, direction);
  //    break;
  //  case LEFT:
  //  case RIGHT:
  //    move(figure_, direction);
  //    break;
  //}
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
        //block->set_x(block->get_x() - 1);
        break;
      case DOWN:
        //block->set_x(block->get_x() - 1);
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

bool GameModel::collides_with_grid(std::vector<Block*> figure) {
  for (std::vector<Block*>::iterator it = blocks_.begin(); it != blocks_.end();
      ++it) {
    Block* block = *it;
    //std::cout << (int)block->get_x() << std::endl;
  }
}

std::vector<Block*>* GameModel::create_figure(std::vector<Block*>* blocks) {
  std::vector<Block*>* figure = new std::vector<Block*>;
  GLcolor color = {0.2f, 0.4, 0.6f};
  create_block(2, 2, color, blocks, figure);
}

void GameModel::create_block(int x, int y, GLcolor color,
    std::vector<Block*>* blocks, std::vector<Block*>* figure) {
  Block* block = new Block(x, y, color);
  blocks->push_back(block);
  figure->push_back(block);
}
