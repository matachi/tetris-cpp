#ifndef BLOCK_H
#define BLOCK_H

//#include <SDL2/SDL.h>
#include <GL/gl.h>

struct GLcolor {
  GLfloat r;
  GLfloat g;
  GLfloat b;
};

class Block {
  public:
    Block(int, int, GLcolor);
    GLcolor get_color(void);
    void set_color(GLcolor);
    int get_x(void);
    void set_x(int);
    int get_y(void);
    void set_y(int);
  private:
    int x_;
    int y_;
    GLcolor color_;
};

#endif
