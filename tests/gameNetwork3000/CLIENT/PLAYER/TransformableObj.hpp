#pragma once
#include <SFML/Graphics.hpp>
#include <MATH/math.hpp>
#include <vector>
#include <memory>

__interface TransformableObj {
public:
	virtual const sf::Vector2f& _getPosition() const = 0;
	virtual const float _getDimentinalK() const = 0;
	virtual sf::Vector2f _getTargetFraction() const = 0;
	virtual sf::Vector2f _getSize() const = 0;
};

class CustomWormCircle : public sf::CircleShape {};