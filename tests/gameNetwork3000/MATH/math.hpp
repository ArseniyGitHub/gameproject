#pragma once
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>

static float getDistance(const sf::Vector2f& p1, const sf::Vector2f& p2) {
	return sqrt(powf(p1.x - p2.x, 2) + powf(p1.y - p2.y, 2));
}

static sf::Vector2f getNormalizedDirection(const sf::Vector2f& start, const sf::Vector2f& end, float dist) {
	sf::Vector2f direction = end - start;
	float length = dist;
	if (length != 0) {
		direction /= length;
	}
	return direction;
}

static sf::Vector2f getNormalizedDirection(const sf::Vector2f& start, const sf::Vector2f& end) {
	return getNormalizedDirection(start, end, getDistance(start, end));
}

