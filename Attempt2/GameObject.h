#pragma once
#define MAX_OBSERVERS 40
#include <list>
#  include <gl/glew.h>
#  include <gl/freeglut.h>
# include <iostream>
using namespace std;

class GameObject
{
public:
	GameObject();
	GameObject(float x, float y, float z, float r, unsigned char colorR,
		unsigned char colorG, unsigned char colorB);
	float getCenterX() { return centerX; }
	float getCenterY() { return centerY; }
	float getCenterZ() { return centerZ; }
	float getRadius() { return radius; }
	virtual void draw();
	void adjustCenter(float x, float y, float z);
	void setCenter(float x, float y, float z);
	void setRadius(float r) { radius = r; }
	int checkSpheresIntersection(float x1, float y1, float z1, float r1,
		float x2, float y2, float z2, float r2)
	{
		return ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) <= (r1 + r2)*(r1 + r2));
	}
protected:
	float centerX, centerY, centerZ, radius;
	unsigned char color[3];
};

class Observer
{
public:
	virtual ~Observer() { }
	virtual void onNotify(GameObject* entity) { };
};

class Subject
{
private:
	list<Observer*> observers;
	int numObservers_;
protected:
	void notify(GameObject* object)
	{

		for (auto const& i : observers)
		{
			i->onNotify(object);
		}
	}

public:
	void addObserver(Observer* observer)
	{
		observers.push_back(observer);
		// Add to array...
	}

	void removeObserver(Observer* observer)
	{
		observers.remove(observer);
		// Remove from array...
	}
	void setObsList(list<Observer*> *obs)
	{
		observers = *obs;
	}
	// Other stuff...
};

class Projectile : public Subject, public GameObject
{
public:
	//list<Observer*>* obslist; 
	Projectile::Projectile() { };
	Projectile::Projectile(float x, float y, float z, float r, unsigned char colorR,
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
	void Projectile::moved()
	{
		//cout << "x: " << getCenterX() << " y: " << getCenterY() << " z: " << getCenterZ() << endl;
		notify(this);
	}
	void Projectile::draw() override
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
	bool operator==(Projectile b)
	{
		return (this->getCenterX() == b.getCenterX() && this->getCenterY() == b.getCenterY() && this->getCenterZ() == b.getCenterZ());
	}

};

class ScoreBoard : public Observer, public GameObject
{
private: 
	int score = 0;
	string scorestr;
protected: 
	virtual void onNotify(GameObject* entity)
	{
		score += 1;
		//scorestr = "Score: " + (score);
		cout << "Score: " << score << endl;
	}

public: 
	string getScore() { return scorestr; }
};