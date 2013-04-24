#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "block.h"

#include <vector>

#include <SDL2/SDL.h>

class GameModel {
  public:
    enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

    GameModel(void);
    ~GameModel();

    void init(void);
    void update(Uint32, Direction);
    std::vector<Block*>& get_blocks(void);
    static const int NUM_OF_BLOCKS = 220;
    static const int BLOCKS_IN_ROW = 10;
    static const int BLOCKS_IN_COL = 22;
  private:
    bool collides_with_grid(std::vector<Block*>, std::vector<Block*>);
    void move(std::vector<Block*>, Direction);
    void rotate_figure(std::vector<Block*>&);
    std::vector<Block*>* create_figure(std::vector<Block*>* blocks);
    void delete_full_rows(std::vector<Block*>& blocks);
    void move_down_blocks_above_level(std::vector<Block*>& blocks, int level);

    // Create a new block and add it to a figure and the collection of all
    // blocks.
    void create_block(int x, int y, GLcolor color, std::vector<Block*>* blocks, std::vector<Block*>* figure);

    std::vector<Block*> blocks_;

    //float clock_;
    Uint32 next_tick_;
    std::vector<Block*> figure_;
    //std::vector<Block*>* figure2_;
};

#endif
