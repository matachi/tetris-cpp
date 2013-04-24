#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <common/shader.hpp>
#include <common/controls.hpp>

#include "game_model.h"
#include "block.h"
using namespace std;


SDL_Window* init_window(void) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window;
  SDL_Renderer* displayRenderer;
  SDL_RendererInfo rendererInfo;
  SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &window,
      &displayRenderer);
  SDL_GetRendererInfo(displayRenderer, &rendererInfo);
  SDL_RenderPresent(displayRenderer);
  return window;
}

void destroy_window(SDL_Window* window) {
  // Close and destroy the window
  SDL_DestroyWindow(window);
  // Clean up
  SDL_Quit();
}

void init_opengl(void) {
  glewExperimental = true;
  glewInit();
  glClearColor(0.6f, 0.2f, 0.2f, 0.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS); 
}

int main(int argc, char* argv[]){
  SDL_Window* window = init_window();
  init_opengl();

  // Create and compile our GLSL program from the shaders
  GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader",
      "SimpleFragmentShader.fragmentshader" );

  // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive
  // vertices give a triangle.  A cube has 6 faces with 2 triangles each, so
  // this makes 6*2=12 triangles, and 12*3 vertices
  static const GLfloat g_vertex_buffer_data[] = {
      -1.0f,-1.0f,-1.0f, // triangle 1 : begin
      -1.0f,-1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f, // triangle 1 : end
      1.0f, 1.0f,-1.0f, // triangle 2 : begin
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f, // triangle 2 : end
      1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
      1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,
      1.0f,-1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f,-1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f,
      1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,
      1.0f,-1.0f, 1.0f
  };

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
  glVertexAttribPointer(
     glGetAttribLocation(programID, "vertexPosition_modelspace"),         // attribute 0. No particular reason for 0, but must match the layout in the shader.
     3,         // size
     GL_FLOAT,  // type
     GL_FALSE,  // normalized?
     0,         // stride
     (void*)0   // array buffer offset
  );
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  GLuint vertexColor = glGetUniformLocation(programID, "vertexColor");


  SDL_Event event;
  GameModel model;
  model.init();
  glUseProgram(programID);
  GameModel::Direction previousDirection = GameModel::NONE;

  while (true) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    while (SDL_PollEvent(&event)) {}
    Uint8 *state = SDL_GetKeyboardState(NULL);

    GameModel::Direction direction = GameModel::NONE;
    if (state[SDL_SCANCODE_UP]) {
      direction = GameModel::UP;
    } else if (state[SDL_SCANCODE_DOWN]) {
      direction = GameModel::DOWN;
    } else if (state[SDL_SCANCODE_LEFT]) {
      direction = GameModel::LEFT;
    } else if (state[SDL_SCANCODE_RIGHT]) {
      direction = GameModel::RIGHT;
    } else if (state[SDL_SCANCODE_ESCAPE]) {
      break;
    }
    if (direction != GameModel::NONE && previousDirection == GameModel::NONE) {
      model.update(SDL_GetTicks(), direction);
    } else {
      model.update(SDL_GetTicks(), GameModel::NONE);
    }
    previousDirection = direction;

    if (computeMatricesFromInputs(window, state)) {
    }


    glm::mat4 ProjectionMatrix = getProjectionMatrix();
    glm::mat4 ViewMatrix = getViewMatrix();

    std::vector<Block*> blocks = model.get_blocks();

    for (std::vector<Block*>::iterator it = blocks.begin(); it != blocks.end();
        ++it) {
      Block* block = *it;

      glm::mat4 ModelMatrix = glm::mat4(1.0);
      ModelMatrix = ModelMatrix * glm::translate(glm::mat4(1.0f),
          glm::vec3(2 * block->get_x(), 2 * block->get_y(), 0.0f));
      glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

      // 1rst attribute buffer : vertices
      glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

      glBindVertexArray(VertexArrayID);
      GLcolor color = block->get_color();
      glUniform3f(vertexColor, color.r, color.g, color.b);
      glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
      glBindVertexArray(0);
    }
    SDL_GL_SwapWindow(window);
  }
  destroy_window(window);

  return 0;
}
