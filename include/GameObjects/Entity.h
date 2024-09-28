#pragma once
#include "GameObject.h"
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

class Entity : public GameObject {
private:
	b2Body* _body;
	sf::RenderWindow* _window;
	
public:
	Entity();
	void update() override;
	void draw() const override;
	void setBody(b2Body* body);
	b2Body* getBody();
};