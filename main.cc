#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <common/shader.hpp>
#include <common/controls.hpp>

#include "game_model.h"
#include "block.h"


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
  glClearColor(0.71f, 0.82f, 0.65f, 0.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS); 
}

int main(int argc, char* argv[]){
  SDL_Window* window = init_window();
  init_opengl();

  // Create and compile our GLSL program from the shaders
  GLuint programId = LoadShaders( "SimpleVertexShader.vertexshader",
      "SimpleFragmentShader.fragmentshader" );
  glUseProgram(programId);

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

  // Create a VAO
  GLuint vertexArrayId;
  glGenVertexArrays(1, &vertexArrayId);
  glBindVertexArray(vertexArrayId);

  // Create a VBO for the vertices
  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
      g_vertex_buffer_data, GL_STATIC_DRAW);
  GLuint index = glGetAttribLocation(programId, "vertexPosition");
  glVertexAttribPointer(
     index,     // index
     3,         // size
     GL_FLOAT,  // type
     GL_FALSE,  // normalized?
     0,         // stride
     (void*)0   // array buffer offset
  );

  // End of VBO
  glEnableVertexAttribArray(0);

  // End of VAO
  glBindVertexArray(0);

  // Get a handle for our "MVP" uniform and vertexColor in the vertex shader
  GLuint matrixId = glGetUniformLocation(programId, "MVP");
  GLuint vertexColor = glGetUniformLocation(programId, "vertexColor");

  SDL_Event event;
  GameModel model; // Create a game model
  model.init();
  // Keep track of previous movement direction
  GameModel::Direction previousDirection = GameModel::NONE;

  while (true) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // This is necessary for the inputs to not freak out
    while (SDL_PollEvent(&event)) {}

    // Poll keyboard to check which keys are pressed
    const Uint8 *state = SDL_GetKeyboardState(NULL);

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

    computeMatricesFromInputs(window, state);

    glm::mat4 projectionMatrix = getProjectionMatrix();
    glm::mat4 viewMatrix = getViewMatrix();

    // Get all blocks
    std::vector<Block*> blocks = model.get_blocks();

    // Draw all blocks
    for (std::vector<Block*>::iterator it = blocks.begin(); it != blocks.end();
        ++it) {
      Block* block = *it;

      glm::mat4 modelMatrix = glm::mat4(1.0);
      modelMatrix = modelMatrix * glm::translate(glm::mat4(1.0f),
          glm::vec3(2 * block->get_x() - 10, 2 * block->get_y() - 15, 0.0f));
      glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

      // 1rst attribute buffer : vertices
      glUniformMatrix4fv(matrixId, 1, GL_FALSE, &MVP[0][0]);
      GLcolor color = block->get_color();
      glUniform3f(vertexColor, color.r, color.g, color.b);

      glBindVertexArray(vertexArrayId);
      glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
      glBindVertexArray(0);
    }
    SDL_GL_SwapWindow(window);
  }
  destroy_window(window);

  return 0;
}
