#ifndef _game_cpp_
#define _game_cpp_


#include "game.hpp"
#include "graphics.hpp"
#include "interface.hpp"
#include "entity.hpp"
#include "entities.hpp"
#include "random.hpp"


bool Game::is_running = true;
float Game::time = .0f;
float Game::delta = .0f;


void Game::init_all() {
  Graphics::init_all();

  Random::init_all();

  // Set the function that updates game logic and interface n times per second.
  glutTimerFunc(15, &Game::update_all, 0);

  // Set the function that redraws in glut's inner loop
  glutDisplayFunc(Game::draw_all);

  Interface::init_all();
  Entity::init_all();

  // Create a box in the middle
  Entity* my_box = new Box();
  Entity::world->add_child(my_box);

  // Creates a terrain
  Terrain::current = new Terrain();
  Terrain::current->load_terrain();
  Entity::world->add_child(Terrain::current);

  // Create a node in the air
  Node* my_node = new Node();
  my_node->position = SetVector( 100, 100, 100);
  Entity::world->add_child(my_node);
  Interface::player = my_node;


  time = (GLfloat)glutGet(GLUT_ELAPSED_TIME)*0.001;
}


void Game::draw_all() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Entity::draw_all();
  Interface::draw_all();          //TODO: Render the 2d GUI
  Graphics::draw_all();
}


void Game::update_all(int value) {
  glutTimerFunc(15, &Game::update_all, value);
  glutPostRedisplay();

  // Time info for shaders and game logic
	float t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)*0.001;
	glUniform1f(glGetUniformLocation(Graphics::shader_program, "time"), t);
  delta = t - time;
  time = t;

  Entity::update_all();
  Interface::keyboard_update();
  Graphics::update_lights();
  glutWarpPointer(200,200);       //Resets the mouse position
}

#endif