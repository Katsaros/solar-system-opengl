#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include "gl/glut.h"
#include "visuals.h"

static bool play = true;														//Flag implementing animation play and pause
static float rotationX = 0.0;													//Angle of camera rotation on x axis
static float rotationY = 0.0;													//Angle of camera rotation on y axis
static float rotationZ = 0.0;													//Angle of camera rotation on z axis
static float earthRotation = 0.0;												//Angle of earth rotation on x axis
static float moonRotation = 0.0;												//Angle of moon rotation on y axis
static float opacity = 1.0;														//A factor of suns shinning sphere
static char decreasing = 1;														//Flag showing if a shinning sphere is increasing or decreasing
static star starArray[STARSINSYSTEM];												//Number of stars on the sky
model myModel;

using namespace std;

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);						// Clean up the colour of the window and the depth buffer
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -100);

	DrawText("PAUSE", 0.02f);

	glRotatef(rotationX, 1, 0, 0);												//Rotate everything around x axis at a rotationX angle
	glRotatef(rotationY, 0, 1, 0);												//Rotate everything around y axis at a rotationY angle
	glRotatef(rotationZ, 0, 0, 1);												//Rotate everything around z axis at a rotationZ angle

	DrawAxes();																	

	DrawStars();															

	DrawEarth();																

	DrawMoon();																	

	DrawSun();																

	glutSwapBuffers();														// All drawing commands applied to the hidden buffer, so now, bring forwardthe hidden buffer and hide the visible one
}

void Resize(int w, int h)
{
	if (h == 0)																// Define the visible area of the window ( in pixels )
	{
		h = 1;
	}
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);											// Setup viewing volume
	glLoadIdentity();

	gluPerspective(60.0, (float)w / (float)h, 1.0, 500.0);
}

void Idle()
{
	if (play)															//If motion is enabled
	{
		if (decreasing == 1)												//If opacity shoud decrease
		{
			opacity -= 0.01;												//Decrease it
			if (opacity < 0)												//Opaciti should increase now
			{
				decreasing = 0;												//Set decrease to 0
			}
		}
		else																//If opacity should increase
		{
			opacity += 0.01;												//Increase it
			if (opacity > 1.0)												//Opacity should decrease now
			{
				decreasing = 1;												//Set decrease to 1
			}
		}

		earthRotation += 0.3f;													//Change the angle of earth rotation
		moonRotation += 1.0f;													//Change the angle of moon rotation
	}

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:																//Exit animation using ESC key
			delete[] myModel.obj_points;
			delete[] myModel.obj_faces;
			exit(0);
			break;
		case 'A':																//Use 'a' key to rotate camera left 
		case 'a':
			rotationY += 0.2f;
			break;
		case 'D':																//Use 'd' key to rotate camera right 
		case 'd':
			rotationY -= 0.2f;
			break;
		case 'W':																//Use 'w' key to rotate camera up 
		case 'w':
			rotationX += 0.2f;
			break;
		case 'S':																//Use 's' key to rotate camera down 
		case 's':
			rotationX -= 0.2f;
			break;
		case 'Q':																//Use 'q' key to rotate camera counter clockwise
		case 'q':
			rotationZ -= 0.2f;
			break;
		case 'E':																//Use 'e' key to rotate camera clockwise
		case 'e':
			rotationZ += 0.2f;
			break;
		case ' ':																//Use 'space' key to pause or unpause the animation
			play = !play;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void Setup()
{
	srand(time(NULL));														// initialize random seed
	for (int i = 0; i < STARSINSYSTEM; i++)
	{
		do {																//Define a sphere of radius 100 around the sun where no star appears
			starArray[i].xPosition = (rand() % 481) - 240;
			starArray[i].yPosition = (rand() % 481) - 240;
			starArray[i].zPosition = (rand() % 481) - 240;
		} while ((starArray[i].xPosition < 100) && (starArray[i].xPosition > -100) && (starArray[i].yPosition < 100) && (starArray[i].yPosition > -100) && (starArray[i].zPosition < 100) && (starArray[i].zPosition > -100));

	}

	ReadFile(&myModel);															//Read the model from planet.obj

	//Parameter handling
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);													//Renders a fragment if its z value is less or equal of the stored value
	glClearDepth(1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//Set up light source
	GLfloat light_position[] = { 0.0, 0.0, -100.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuseLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);									// Black background

}

void ReadFile(model *myModel)
{
	ifstream obj_file("Release\\planet.obj");											// Open the file for reading OBJINFO.TXT
	if (obj_file.fail())
	{
		exit(1);
	}
	char token;
	int trashnum;
	string trash;
	//////////////////////READING POINTS STARTS HERE////////////////////////////////
	for (int i = 0; i < 5; i++)
	{
		getline(obj_file, trash);
	}

	obj_file >> token;

	obj_file >> myModel->vertices;

	myModel->obj_points = new point[myModel->vertices];

	getline(obj_file, trash);

	for (int i = 0; i < myModel->vertices; i++)
	{
		obj_file >> token;

		obj_file >> myModel->obj_points[i].x;
		obj_file >> myModel->obj_points[i].y;
		obj_file >> myModel->obj_points[i].z;
	}
	//////////////////////READING POINTS ENDS HERE/////////////////////////////////////

	//////////////////////READING FACES STARTS HERE/////////////////////////////////////
	for (int i = 0; i < 9129; i++)															//Skip UV and VN
	{
		getline(obj_file, trash);
	}

	for (int i = 0; i < 4; i++)
	{
		obj_file >> trash;
	}

	obj_file >> myModel->faces;

	myModel->obj_faces = new face[myModel->faces];

	obj_file >> trash;

	getline(obj_file, trash);
	getline(obj_file, trash);
	getline(obj_file, trash);

	for (int i = 0; i < myModel->faces; i++)
	{
		obj_file >> token;

		obj_file >> myModel->obj_faces[i].vtx[0];

		obj_file >> token;
		obj_file >> token;
		obj_file >> trashnum;

		obj_file >> myModel->obj_faces[i].vtx[1];

		obj_file >> token;
		obj_file >> token;
		obj_file >> trashnum;

		obj_file >> myModel->obj_faces[i].vtx[2];

		obj_file >> token;
		obj_file >> token;
		obj_file >> trashnum;

	}
	//////////////////////READING FACES ENDS HERE////////////////////////////////////////
	obj_file.close();
}

void DisplayModel(model myModel)
{

	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < myModel.faces; i++)
	{
		glVertex3f(myModel.obj_points[myModel.obj_faces[i].vtx[0] - 1].x, myModel.obj_points[myModel.obj_faces[i].vtx[0] - 1].y, myModel.obj_points[myModel.obj_faces[i].vtx[0] - 1].z);
		glVertex3f(myModel.obj_points[myModel.obj_faces[i].vtx[1] - 1].x, myModel.obj_points[myModel.obj_faces[i].vtx[1] - 1].y, myModel.obj_points[myModel.obj_faces[i].vtx[1] - 1].z);
		glVertex3f(myModel.obj_points[myModel.obj_faces[i].vtx[2] - 1].x, myModel.obj_points[myModel.obj_faces[i].vtx[2] - 1].y, myModel.obj_points[myModel.obj_faces[i].vtx[2] - 1].z);
	}

	glEnd();
	glPopMatrix();

}

void DrawSun()
{
	glColor3f(1.0, 1.0, 0.0);												// Set inner sphere colour to be yellow
	glutSolidSphere(10.0, 30, 24);

	glColor4f(1.0, 1.0, 0.0, opacity);										// Set outer sphere colour to be  yellow and transparent
	glutSolidSphere(15, 30, 24);
}

void DrawStars()
{
	for (int y = 0; y < STARSINSYSTEM; y++)
	{
		glPushMatrix();
		glTranslatef(starArray[y].xPosition, starArray[y].yPosition, starArray[y].zPosition);			//Put the star on its random position
		glColor3f(1.0, 1.0, 1.0);											// Set star to white colour
		glutSolidSphere(0.2, 15, 10);										// Draw a star
		glPopMatrix();
	}
}

void DrawEarth()
{
	glPushMatrix();
	glRotatef(earthRotation, 0, 1, 0);											//Earth will be rotating around y axis
	glRotatef(earthRotation,1 , 0, 0);											//Earth will be rotating around y axis
	glTranslatef(40, 0, 0);
	glColor3f(0.0, 0.0, 1.0);												
	glutSolidSphere(3.0, 30, 24);
	glScalef(0.060f, 0.060f, 0.060f);
	DisplayModel(myModel);														//Draw the earth
	glPopMatrix();
}

void DrawMoon()
{
	glPushMatrix();
	glRotatef(earthRotation, 0, 1, 0);											//Moon will be rotating around y axis
	glRotatef(moonRotation, 1, 0, 0);											//Moon will be rotating around x axis
	glTranslatef(40, 15, 0);
	glColor3f(0.6, 0.6, 0.6);												
	glutSolidSphere(1.0, 30, 24);
	glScalef(0.005f, 0.005f, 0.005f);
	DisplayModel(myModel);														//Draw the moon
	glPopMatrix();
}

void DrawAxes()
{
	glColor3f(0.6, 0.6, 0.6);												// Set drawing colour
	glPushMatrix();
	glTranslatef(0, 0, -1.0);
	glBegin(GL_LINES);
	glVertex2f(0.0, 0.0);
	glVertex2f(100.0, 0.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 100.0);
	glEnd();
	glPopMatrix();
}

void DrawText(const char *str, float size)
{
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, !play);										// Set drawing colour and transparency
	glTranslatef(-4.0, 17.0, 0.0);
	glScalef(size, size, size);
	for (int i = 0; i < strlen(str); i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();

}