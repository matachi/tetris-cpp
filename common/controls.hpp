#ifndef CONTROLS_HPP
#define CONTROLS_HPP


struct Keys {
  bool up, down, left, right;
};

//bool computeMatricesFromInputs(SDL_Window *window, Keys);
bool computeMatricesFromInputs(SDL_Window *window, Uint8*);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif
