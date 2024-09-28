#include "GameObjects/Entity.h"

b2Body* Entity::getBody() {
	return Entity::_body;
}

void Entity::setBody(b2Body* body) {
	Entity::_body = body;
}

void Entity::draw() const {

}

void Entity::update() {

}