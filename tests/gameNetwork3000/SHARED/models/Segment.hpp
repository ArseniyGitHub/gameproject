#pragma once
struct Worm_Segment{
	float x, y;  unsigned int RGBA;
	Worm_Segment(float x = 0, float y = 0, unsigned int clr = 0, float rad = 1) : x(x), y(y), RGBA(clr) {}
};