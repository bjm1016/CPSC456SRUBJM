#  include <gl/glew.h>
#  include <gl/freeglut.h>
#  include <gl/glew.h>
# include "Alien.h"
#include <iostream>

// Alien constructor.

#define PI 3.14159265


// Function to draw Alien.
void Alien::draw()
{
	if (radius > 0.0) // If Alien exists.
	{
		glPushMatrix();
		glTranslatef(centerX, centerY, centerZ);
		glColor3ubv(color);
		glutWireSphere(radius, (int)radius * 6, (int)radius * 6);
		glPopMatrix();
	}
}
void Alien::onNotify(GameObject* entity)
{
	if (checkSpheresIntersection(entity->getCenterX() - 5 * sin((PI / 180.0) * entity->getCenterZ()), 0.0,
		entity->getCenterZ() - 5 * cos((PI / 180.0) * entity->getCenterZ()), 7.072,
		getCenterX(), getCenterY(),
		getCenterZ(), getRadius()))
	{
		die();
	}
}
	void Alien::die()
	{
		if (!dead) {
			//cout << "dead";
			setRadius(0);
			dead = true;
			notify(this);
		}
	}
