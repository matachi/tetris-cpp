#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 5 ); 
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


//bool computeMatricesFromInputs(SDL_Window *window, Keys keys){
bool computeMatricesFromInputs(SDL_Window *window, Uint8* state){

  bool exit = false;

	// glfwGetTime is called only once, the first time this function is called
	//static double lastTime = SDL_GetTicks();

	// Compute time difference between current and last frame
	double currentTime = SDL_GetTicks();
	float deltaTime = float(currentTime - lastTime) / 1000.0f;
  lastTime = currentTime;

  //SDL_PollEvent(&event);

	// Get mouse position
	int xpos, ypos;
  SDL_GetMouseState(&xpos, &ypos);

  //xpos = 0;
  //ypos = 0;
	// Reset mouse position for next frame
	//SDL_WarpMouse(800/2, 600/2);
  if (xpos != 400 || ypos != 300) {
    std::cout << "HEJ" << xpos << ypos << std::endl;
    SDL_WarpMouseInWindow(window, 400, 300);
  }

	// Compute new orientation
	horizontalAngle += float(800/2 - xpos ) / 1000;
	verticalAngle   += float(600/2 - ypos ) / 1000;
	//horizontalAngle += mouseSpeed * deltaTime * float(800/2 - xpos );
	//verticalAngle   += mouseSpeed * deltaTime * float(600/2 - ypos );
    //std::cout << mouseSpeed * deltaTime * float(800/2 - xpos ) << std::endl;
	//horizontalAngle += 0;
	//verticalAngle   += 0;

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

  //Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_UP]){
        //if (keys.up) {
std::cout << "OUT" << std::endl;
          position += direction * deltaTime * speed;
        }
        // Move backward
        if (state[SDL_SCANCODE_DOWN]){
        //if (keys.down) {
          position -= direction * deltaTime * speed;
        }
        // Strafe right
        if (state[SDL_SCANCODE_RIGHT]){
        //if (keys.right){
          position += right * deltaTime * speed;
        }
        // Strafe left
        if (state[SDL_SCANCODE_LEFT]){
        //if (keys.left){
          position -= right * deltaTime * speed;
        }
        //if (state[SDL_SCANCODE_A]){
        //  horizontalAngle = 3.14f;
        //  verticalAngle   = 0;
        //}
//  //std::cout << "OUT" << std::endl;
//  while(SDL_PollEvent(&event)) {
//
//  //std::cout << "IN" << std::endl;
//    //Check the event structure -> type member
//    switch(event.type){
//
//      //Event where user has closed the window (x button). Break from the loop
//      case SDL_QUIT:
//        exit = true;
//        break;
//
//      //Event where the user has pressed a key on the keyboard
//      case SDL_KEYDOWN:
//        //switch(event.key.keysym.sym) {
//        //  case SDLK_ESCAPE:
//        //    exit = true;
//        //    break;
//        //  case SDLK_UP:
//        //    position += direction * deltaTime * speed;
//        //    break;
//        //}
//        if(event.key.keysym.sym == SDLK_ESCAPE){
//          exit = true;
//          break;
//        }
//        if (event.key.keysym.sym == SDLK_UP){
//          position += direction * deltaTime * speed;
//        }
//        // Move backward
//        if (event.key.keysym.sym == SDLK_DOWN){
//          position -= direction * deltaTime * speed;
//        }
//        // Strafe right
//        if (event.key.keysym.sym == SDLK_RIGHT){
//          position += right * deltaTime * speed;
//        }
//        // Strafe left
//        if (event.key.keysym.sym == SDLK_LEFT){
//          position -= right * deltaTime * speed;
//        }
//    }
//  }

	//float FoV = initialFoV - 5 * glfwGetMouseWheel();
	float FoV = initialFoV;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;

  return exit;
}
