/*
 * trySI.cpp
 *
 *  Created on: Sep 19, 2018
 *      Author: paul.mullins
 *
 *  Bare bones game structure for "space invaders"
 *  First... understand the code. Make changes to see that you have it.
 *
 *  Modifications needed
 *  checking for a hit and then dealing with it
 *     make the alien hit "blow up" - new animation
 *     the cannon should be able to fire again as soon as a hit occurs
 *  Checking for all aliens defeated and then dealing with it
 *  Ending animation, win or lose, whatever that may be
 *
 *  You may make other changes as you see fit to create an interactive game
 *  However, don't create a "2D screen"... there is a good chance we will
 *     modify the game to be 3D later (ROWS, COLS, RANKs).
 *     This means that if you are displaying text (or status) at all, you probably
 *     want it to be able to move with the camera (thus "appearing" stationary in
 *     the window).
 */

///////////////////////////////////////////////////////////////////////////////////
// Code pirated from spaceTravel.cpp
//
// Interaction:
// Press the left/right arrow keys to move the cannon.
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cmath>
#include <iostream>
#include "Alien.h"
#include <vector>
#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <gl/glew.h>
#  include <gl/freeglut.h>
#  include <gl/glew.h>
//#pragma comment(lib, "glew32.lib")
#endif

#define PI 3.14159265
static unsigned int animationPeriod = 1000; // Time interval between frames.
static float bottom = -80.0; // lowest y-coord for aliens (see animate)
static float top = 140; // top of "screen" (for cannon ball)
static float edge = 140.0; // a virtual "edge of screen" (x-coord checked in animate)
static float deltaX = 5.0; // speed for an alien
static float zPlane = -60.0; // where all the action is
static float yCannon = -100.0; // translate for cannon
static int gameOver = 0; // if game is over, probably started a final animation
static int canShoot = 1; // cannon ready to fire
static float oob = -330;

#define ROWS 4  // Number of rows of Aliens.
#define COLUMNS 8 // Number of columns of Aliens.

using namespace std;

// Globals.
static float xVal = 0;
//static float yVal = 0;// Co-ordinates of the cannon (xVal, 0, zPlane)
//static int isCollision = 0; // Is there collision between the cannonball and an Alien?
static unsigned int cannon; // Display lists base index.

// Alien class.

// Alien default constructor.

Alien arrayAliens[ROWS][COLUMNS]; // Global array of Aliens.
list<Projectile> plist;
//Projectile cannonBall;
list<Observer*> alienlist;
ScoreBoard score;

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1,
						     float x2, float y2, float z2, float r2)
{
   return ( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) <= (r1+r2)*(r1+r2) );
}

// Function to check if the cannon collides with an Alien when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the cannon we use a bounding sphere.
int AlienCraftCollision(float x, float z, float a)
{
   int i,j;

   // Check for collision with each Alien.
   for (j=0; j<COLUMNS; j++)
      for (i=0; i<ROWS; i++)
		 if (arrayAliens[i][j].getRadius() > 0 ) // If Alien exists.
            if ( checkSpheresIntersection( x - 5 * sin( (PI/180.0) * a), 0.0,
		         z - 5 * cos( (PI/180.0) * a), 7.072,
		         arrayAliens[i][j].getCenterX(), arrayAliens[i][j].getCenterY(),
		         arrayAliens[i][j].getCenterZ(), arrayAliens[i][j].getRadius() ) )
		       return 1;
   return 0;
}

// Drawing routine.
void drawScene(void)
{
	int i, j;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glLoadIdentity();
	//glutBitmapString(GLUT_BITMAP_HELVETICA_12, rscore.getScore().c_str);
	gluLookAt(xVal, yCannon, 15, xVal, yCannon, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(125.0, -90.0, zPlane,  0.0, -90.0, zPlane,  0.0, 1.0, 0.0); // side view
	//gluLookAt(25.0, -100.0, zPlane,  0.0, -100.0, zPlane,  0.0, 1.0, 0.0); // side close up

	// Draw all the Aliens (with radius > 0) in arrayAliens
	for (j = 0; j < COLUMNS; j++)
		for (i = 0; i < ROWS; i++)
		{
			arrayAliens[i][j].draw();
		}
	for (Projectile p : plist)
	{
		p.draw(); // only appears if r>0
	}
   glTranslatef(xVal, yCannon, zPlane);
   glCallList(cannon);

   glutSwapBuffers();
}

void animateEndGame(int value)
{
	cout << "You lose" << endl;
	// if this is really an animation, do your thing and set a timer
}

// Routine to animate with a recursive call made after animationPeriod msecs.
void animateAliens(int value)
{
	float deltaY = 0.0;

	if (!gameOver)
	{
		// move aliens
		if ((arrayAliens[0][0].getCenterX() <= -edge && deltaX < 0) ||
				(arrayAliens[0][COLUMNS-1].getCenterX() >= edge && deltaX > 0))
		{ // step down and reverse direction
			deltaY = -5 * abs(deltaX);  // adjust step down here
			deltaX = -deltaX;
		}

		for (int j=0; j<COLUMNS; j++)
			  for (int i=0; i<ROWS; i++)
			  {
				  // if this alien is "marked" then deal... at least set radius to 0
				  if (arrayAliens[i][j].getRadius() > 0 &&
						  arrayAliens[i][j].getCenterY() + deltaY  <= bottom)
					  gameOver = 1; // lose on next cycle
				  arrayAliens[i][j].adjustCenter(deltaX, deltaY, 0.0);
			  }
	}

	glutPostRedisplay();

	if (!gameOver) // currently only one way for game to be "over"
		glutTimerFunc(animationPeriod, animateAliens, 1);
	else
		glutTimerFunc(animationPeriod, animateEndGame, 1);

}

// Initialization routine.
void setup(void)
{
   int i, j;

   cannon = glGenLists(1);
   glNewList(cannon, GL_COMPILE);
	  glPushMatrix();
	  glRotatef(90.0, -1.0, 0.0, 0.0); // To make the cannon point up the y-axis
	  glColor3f (1.0, 1.0, 1.0);
      glutWireCone(5.0, 10.0, 10, 10);
	  glPopMatrix();
   glEndList();

   //cannonBall = Projectile(0.0, 0.0, 0.0, 0, 255, 0, 255);
   // Initialize global arrayAliens.
   for (j=0; j<COLUMNS; j++)
	   for (i = 0; i < ROWS; i++) {
		   // assume even number of columns.
			 // x,y,z r (2..3 radius)  RGB
		   arrayAliens[i][j] = Alien(15 + 30.0*(-COLUMNS / 2 + j), 120.0 - 30.0*i, zPlane,
			   (double)(rand() % 2 + 2), 188, 188, 0);
		   arrayAliens[i][j].addObserver(&score);
		   alienlist.push_back(&arrayAliens[i][j]);

	   }
   //cannonBall.setObsList(&alienlist);
   glEnable(GL_DEPTH_TEST);
   glClearColor (0.0, 0.0, 0.0, 0.0);

   animateAliens(1); // start the animation
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport (0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-10.0, 10.0, -10.0, 10.0, 5.0, 250.0);
   glMatrixMode(GL_MODELVIEW);

}

void animateCannonBall(int value)
{
	//std::list<Projectile>::iterator i = plist.begin();
	//Projectile ball;
	for (list<Projectile>::iterator i = plist.begin(); i != plist.end();){
		Projectile* ball = &*i;
		if (ball->getCenterZ() > oob)
		{
			//cout << ball.getCenterX();
			//cout << ball->getCenterX() << " " << ball->getCenterY() << " " << ball->getCenterZ() << endl;
			ball->adjustCenter(0, 0, -1);
			// check for a hit
			i++;
			ball->moved();
		}
		else
		{
			//cout << "Here";
			ball->setRadius(0);
			i = plist.erase(i);
			//canShoot = 1;
		}
	}
	glutTimerFunc(15, animateCannonBall, 1);
	glutPostRedisplay();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key)
   {
      // use space key to shoot upwards from cannon location
   	  case ' ':
   		  if (canShoot)
   		  {
			  Projectile cannonBall = Projectile(0.0, 0.0, 0.0, 0, 255, 0, 255);
			  //Projectile cannonBall; 
   			  //canShoot = 0; // need a delay here
   			  cannonBall.setRadius(1); // displayable
   			  cannonBall.setCenter(xVal, yCannon, zPlane);
   			  animateCannonBall(1);
			  cannonBall.setObsList(&alienlist);
			  plist.push_back(cannonBall);

   			  glutTimerFunc(15, animateCannonBall, 1);
   		  }
   		  break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   float tempxVal = xVal;
   float tempyVal = yCannon;
   // Compute next position.
   if (key == GLUT_KEY_LEFT) tempxVal = xVal - 5.0;
   if (key == GLUT_KEY_RIGHT) tempxVal = xVal + 5.0;
   if (key == GLUT_KEY_UP) tempyVal = yCannon + 5.0;
   if (key == GLUT_KEY_DOWN) tempyVal = yCannon - 5.0;
   // Move cannon to next position only if not offscreen.

   if (abs(tempxVal) < edge)
   {
      xVal = tempxVal;
	  
   }
   if (abs(yCannon) <= top)
   {
	   yCannon = tempyVal;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the left/right arrow keys to turn the craft." << endl
        << "Press the up/down arrow keys to move the craft." << endl;
}

// Main routine.
void render(int argc, char **argv)
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 0);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(800, 400);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("spaceTravel.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   //glewExperimental = GL_TRUE;
   //glewInit();

   setup();

   glutMainLoop();
}
