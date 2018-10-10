#pragma once
#include "GameObject.h"
class Alien : public GameObject, public Observer, public Subject
{
public:
	void draw();
	bool dead = false;
	Alien::Alien() { };
	Alien::Alien(float x, float y, float z, float r, unsigned char colorR,
		unsigned char colorG, unsigned char colorB) {
		centerX = x;
		centerY = y;
		centerZ = z;
		radius = r;
		color[0] = colorR;
		color[1] = colorG;
		color[2] = colorB;
	}
	void Alien::onNotify(GameObject* entity) override;
	void Alien::die();
};