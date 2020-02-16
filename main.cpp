#include <stdio.h>     // - Just for some ASCII messages
#include "gl/glut.h"   // - An interface and windows 
                       //   management library
#include "visuals.h"   // Header file for our OpenGL functions

using namespace std;

int main(int argc, char* argv[])
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glutInit(&argc, argv);															// initialize GLUT library state
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);						// Set up the display using the GLUT functions to get rid of the window setup details:
																					// - Use true RGB colour mode ( and transparency )
																					// - Enable double buffering for faster window update
																					// - Allocate a Depth-Buffer in the system memory or in the video memory if 3D acceleration available
	glutInitWindowSize(1280, 960);													// Define the main window size and initial position 
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Universe");													// Create and label the main window
	Setup();																		// Configure various properties of the OpenGL rendering context
																					// Callbacks for the GL and GLUT events:
	glutDisplayFunc(Render);														// The rendering function 				
	glutReshapeFunc(Resize);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();																	//Enter main event handling loop
	return 0;
}
