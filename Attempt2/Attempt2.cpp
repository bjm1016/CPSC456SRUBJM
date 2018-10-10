// Attempt2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <GL\GL.h>
#include <cmath>
#include <gl/glew.h>
//#include <GL\glxew.h>
#include <GL\freeglut.h>
#include <GL/glut.h>
#define PI 3.14159265
bool isWire = 0;
// Globals.

void drawScene(void)
{
	float R = 20.0; // Radius of helix.

	float t; // Angle parameter.

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	//if (isWire) glPolygonMode(GL_FRONT, GL_LINE); else glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_TRIANGLE_STRIP);
	for (t = -10 * PI; t <= 10 * PI; t += PI/30)
	{
		//glVertex3f(R * cos(t), R * sin(t), t - 60.0);
		glVertex3f(R * cos(t), t, R * sin(t) - 60.0);
		glVertex3f((R * cos(t)) + 7, t, R * sin(t) - 60.0);
		//glVertex3f(R * cos(t), t, R * sin(t) - 60.0);
		//glVertex3f(R * cos(t), t, R * sin(t) - 60.0);

	}
	glEnd();

	glFlush();
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

	//glOrtho(-50.0, 50.0, -50.0, 50.0, 0.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Main routine.
int render(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("square.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	//glewExperimental = GL_TRUE;
	//glewInit();

	setup();

	glutMainLoop();
}

