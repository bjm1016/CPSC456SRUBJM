//#include <game>
/*class GameObject : Observer
{
public:
	GameObject();
	GameObject(float x, float y, float z, float r, unsigned char colorR,
		unsigned char colorG, unsigned char colorB);
	float getCenterX() { return centerX; }
	float getCenterY() { return centerY; }
	float getCenterZ() { return centerZ; }
	float getRadius() { return radius; }
	void draw();
	void adjustCenter(float x, float y, float z);
	void setCenter(float x, float y, float z);
	void setRadius(float r) { radius = r; }

private:
	float centerX, centerY, centerZ, radius;
	unsigned char color[3];
};*/
#include "GameObject.h"

GameObject::GameObject()
{
	centerX = 0.0;
	centerY = 0.0;
	centerZ = 0.0;
	radius = 0.0; // Indicates no Alien exists in the position.
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
}

// Alien constructor.
GameObject::GameObject(float x, float y, float z, float r, unsigned char colorR,
	unsigned char colorG, unsigned char colorB)
{
	centerX = x;
	centerY = y;
	centerZ = z;
	radius = r;
	color[0] = colorR;
	color[1] = colorG;
	color[2] = colorB;
}

// Alien change location
void GameObject::adjustCenter(float x, float y, float z)
{
	centerX += x;
	centerY += y;
	centerZ += z;
}

// Alien set location
void GameObject::setCenter(float x, float y, float z)
{
	centerX = x;
	centerY = y;
	centerZ = z;
}

void GameObject::draw()
{

}