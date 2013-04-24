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
    // Check if the figure collides with the blocks
    bool collides_with_grid(std::vector<Block*>& figure,
        std::vector<Block*>& blocks);
    // Move figure in given direction
    void move(std::vector<Block*>& figure, Direction direction);
    // Rotate the figure 1 step clockwise
    void rotate_figure(std::vector<Block*>& figure);
    // Create a figure, returns a refernce to a new figure
    std::vector<Block*>* create_figure(std::vector<Block*>* blocks);
    // Check and clear all full rows
    void delete_full_rows(std::vector<Block*>& blocks);
    // Move down all blocks above given level 1 step
    void move_down_blocks_above_level(std::vector<Block*>& blocks, int level);
    // Create a new block and add it to a figure and the collection of all
    // blocks.
    void create_block(int x, int y, GLcolor color, std::vector<Block*>* blocks,
        std::vector<Block*>* figure);

    // Collection of all blocks
    std::vector<Block*> blocks_;

    // When the figure should move down 1 step
    Uint32 next_tick_;

    // The figure, i.e. the moving block formation
    std::vector<Block*> figure_;
};

#endif
