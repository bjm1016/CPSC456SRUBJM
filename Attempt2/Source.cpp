#include "render.h"
#include <iostream>
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
int score;
using namespace std;
#pragma once
int main(int argc, char **argv)
{
	render(argc, argv);
	return 0;
}