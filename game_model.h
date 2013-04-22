#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "block.h"

#include <vector>

class GameModel {
  public:
    enum Direction { UP, DOWN, LEFT, RIGHT };

    GameModel(void);
    ~GameModel();

    void update(float, Direction);
    std::vector<Block*> get_blocks(void);
    static const int NUM_OF_BLOCKS = 220;
    static const int BLOCKS_IN_ROW = 10;
    static const int BLOCK_IN_COL = 22;
  private:
    bool collides_with_grid(std::vector<Block*>);
    void move(std::vector<Block*>, Direction);
    std::vector<Block*>* create_figure(std::vector<Block*>* blocks);
    // Create a new block and add it to a figure and the collection of all
    // blocks.
    void create_block(int x, int y, GLcolor color, std::vector<Block*>* blocks, std::vector<Block*>* figure);

    std::vector<Block*> blocks_;

    float clock_;
    std::vector<Block*> figure_;
};

#endif
