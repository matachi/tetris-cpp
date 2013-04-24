#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "controls.hpp"
#include <math.h>

glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;

glm::mat4 getViewMatrix(){
  return viewMatrix;
}
glm::mat4 getProjectionMatrix(){
  return projectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 5, 10, 55 );
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 5.0f; // 3 units / second
//float mouseSpeed = 0.005f;
float mouseSpeed = 5;

double lastTime;

SDL_Event event;


void computeMatricesFromInputs(SDL_Window *window, Uint8* state) {

  // Compute time difference between current and last frame
  double currentTime = SDL_GetTicks();
  float deltaTime = float(currentTime - lastTime) / 1000.0f;
  lastTime = currentTime;

  // Get mouse position
  int xpos, ypos;
  SDL_GetMouseState(&xpos, &ypos);

  // Reset mouse position for next frame
  if (xpos != 400 || ypos != 300) {
    SDL_WarpMouseInWindow(window, 400, 300);
  }

  // Compute new orientation
  horizontalAngle += float(800/2 - xpos ) / 1000;
  verticalAngle   += float(600/2 - ypos ) / 1000;

  // Direction : Spherical coordinates to Cartesian coordinates conversion
  glm::vec3 direction(
    cos(verticalAngle) * sin(horizontalAngle),
    sin(verticalAngle),
    cos(verticalAngle) * cos(horizontalAngle)
  );

  // Right vector
  glm::vec3 right = glm::vec3(
    sin(horizontalAngle - 3.14f/2.0f), 
    0,
    cos(horizontalAngle - 3.14f/2.0f)
  );

  // Up vector
  glm::vec3 up = glm::cross( right, direction );

  // Move forward
  if (state[SDL_SCANCODE_W]){
    position += direction * deltaTime * speed;
  }
  // Move backward
  if (state[SDL_SCANCODE_S]){
    position -= direction * deltaTime * speed;
  }
  // Strafe right
  if (state[SDL_SCANCODE_A]){
    position += right * deltaTime * speed;
  }
  // Strafe left
  if (state[SDL_SCANCODE_D]){
    position -= right * deltaTime * speed;
  }

  //float FoV = initialFoV - 5 * glfwGetMouseWheel();
  float FoV = initialFoV;

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  projectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
  // Camera matrix
  viewMatrix = glm::lookAt(
    position,           // Camera is here
    position+direction, // and looks here : at the same position, plus "direction"
    up                  // Head is up (set to 0,-1,0 to look upside-down)
  );

  // For the next frame, the "last time" will be "now"
  lastTime = currentTime;
}
