#include <GL/glew.h>
#include <GL/gl.h>
//#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

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

  // position
  //glm::vec3 position = glm::vec3( 0, 0, 5 );
  //glm::vec3 position = glm::vec3( 0, 0, 0 );
  // horizontal angle : toward -Z
  float horizontalAngle = 3.14f;
  // vertical angle : 0, look at the horizon
  float verticalAngle = 0.0f;
  // Initial Field of View
  float initialFoV = 45.0f;

  float speed = 3.0f; // 3 units / second
  float mouseSpeed = 0.005f;

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

  // One color for each vertex. They were generated randomly.
  static const GLfloat g_color_buffer_data[] = {
      0.583f,  0.771f,  0.014f,
      0.609f,  0.115f,  0.436f,
      0.327f,  0.483f,  0.844f,
      0.822f,  0.569f,  0.201f,
      0.435f,  0.602f,  0.223f,
      0.310f,  0.747f,  0.185f,
      0.597f,  0.770f,  0.761f,
      0.559f,  0.436f,  0.730f,
      0.359f,  0.583f,  0.152f,
      0.483f,  0.596f,  0.789f,
      0.559f,  0.861f,  0.639f,
      0.195f,  0.548f,  0.859f,
      0.014f,  0.184f,  0.576f,
      0.771f,  0.328f,  0.970f,
      0.406f,  0.615f,  0.116f,
      0.676f,  0.977f,  0.133f,
      0.971f,  0.572f,  0.833f,
      0.140f,  0.616f,  0.489f,
      0.997f,  0.513f,  0.064f,
      0.945f,  0.719f,  0.592f,
      0.543f,  0.021f,  0.978f,
      0.279f,  0.317f,  0.505f,
      0.167f,  0.620f,  0.077f,
      0.347f,  0.857f,  0.137f,
      0.055f,  0.953f,  0.042f,
      0.714f,  0.505f,  0.345f,
      0.783f,  0.290f,  0.734f,
      0.722f,  0.645f,  0.174f,
      0.302f,  0.455f,  0.848f,
      0.225f,  0.587f,  0.040f,
      0.517f,  0.713f,  0.338f,
      0.053f,  0.959f,  0.120f,
      0.393f,  0.621f,  0.362f,
      0.673f,  0.211f,  0.457f,
      0.820f,  0.883f,  0.371f,
      0.982f,  0.099f,  0.879f
  };

  //glBindAttribLocation(programID, 0, "vertexPosition_modelspace");
  //glBindAttribLocation(programID, 1, "vertexColor");

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

  //GLuint vertexColor = glGetAttribLocation(programID, "vertexColor");
  //cout << (int)vertexColor << endl;
  //cout << glGetAttribLocation(programID, "vertexPosition_modelspace") << endl;
  //GLuint colorbuffer;
  //glGenBuffers(1, &colorbuffer);
  //glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
  //glVertexAttribPointer(
  //    glGetAttribLocation(programID, "vertexColor"),        // attribute. No particular reason for 1, but must match the layout in the shader.
  //    3,        // size
  //    GL_FLOAT, // type
  //    GL_FALSE, // normalized?
  //    0,        // stride
  //    (void*)0  // array buffer offset
  //);
  //glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  GLuint vertexColor = glGetUniformLocation(programID, "vertexColor");

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit
  // <-> 100 units
  glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  // Camera matrix
  glm::mat4 View = glm::lookAt(
      glm::vec3(5,0,5), // Camera is at (4,3,3), in World Space
      glm::vec3(5,0,0), // and looks at the origin
      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
  );
  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 Model = glm::mat4(1500.0f);
  // Our ModelViewProjection : multiplication of our 3 matrices
  glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication
                                             // is the other way around


  SDL_Event event;
  GameModel model;
  model.init();
  glUseProgram(programID);
  GameModel::Direction previousDirection = GameModel::NONE;
  //glGetAttribLocation(programID, "vertexColor");
  //    glVertexAttrib3f(glGetAttribLocation(programID, "vertexColor"), color.r, color.g, color.b);

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
