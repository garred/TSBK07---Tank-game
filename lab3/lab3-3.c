// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-2.frag", "lab1-2.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"


// Consts & Globals

// Frustrum
#define near 0.1
#define far 1000.0
#define right 0.05
#define left -0.05
#define top 0.05
#define bottom -0.05

GLuint program; //Reference to shader program
mat4 projectionMatrix;

// Objects management.
#include "src/helpers3-3.c"
#include "src/windmill3-3.c"
#include "src/camera3-3.c"
#include "src/static3-3.c"
#include "src/skybox3-3.c"


// Initialize everything

void init(void) {
	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-3.vert", "lab3-3.frag");
	printError("init shader");

	// Initialize objects of the world
	init_camera();
	load_windmill();
	load_static_objects();
	load_skybox();

	// Set and send projection matrix to the vertex shader
	projectionMatrix = frustum(left,right, bottom,top, near,far);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	printError("init arrays");
}


// Display everything

void display(void) {
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Time info for shaders
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME)*0.001;
	glUniform1f(glGetUniformLocation(program, "time"), t);

	// Setting the world
	update_camera(t);
	draw_skybox();
	draw_windmill();
	draw_static_objects();

	// Flip
	glutSwapBuffers();
}


// Updates 50 frames per second

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
		windmill.blade_angle += 0.05;
		keyboard_func();
}


// Main

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(800,800);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	glutTimerFunc(20, &OnTimer, 0);
	glutPassiveMotionFunc(&mouse_func);

	init ();
	glutMainLoop();
	return 0;
}