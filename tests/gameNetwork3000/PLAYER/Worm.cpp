#pragma once
#include <SFML/Graphics.hpp>
#include "../MATH/math.hpp"
#include <vector>
#include <memory>

class TransformableObj {
public:
	virtual const sf::Vector2f& getPosition() const = 0;
};

class CustomWormCircle : public sf::CircleShape {

};

class Camera : public sf::View {
	std::shared_ptr<TransformableObj> targetObject = nullptr;
	float speed = 5;
    
public:
	Camera(const sf::RenderWindow& window) : View(window.getView()) {}
	void update(sf::RenderWindow& windowsIdk, float dT) {
		if (targetObject == nullptr)
			return;
		sf::Vector2f coords = targetObject->getPosition();
		sf::Vector2f currentView = this->getCenter();
		float dist = getDistance(currentView, coords);
		sf::Vector2f normVec = getNormalizedDirection(currentView, coords, dist);
		sf::Vector2f step = normVec * speed * dist * dT;

		this->move(step);
		windowsIdk.setView(*this);
	}
	void setTargetObject(const std::shared_ptr<TransformableObj> from) { targetObject = from; }
	std::shared_ptr<TransformableObj> getTargetObject()                { return targetObject; }
};



class Worm : public TransformableObj {
private:
	std::vector<CustomWormCircle> body;
	sf::Vector2f targetPos;
	float speed;
	float headSpeed;
	size_t colorSeed = time(nullptr);
	std::string name;
public:
	const sf::Vector2f& getPosition() const override {
		return body.empty() ? sf::Vector2f() : body[0].getPosition();
	}
	Worm(size_t sz = 5, sf::Vector2f startPos = sf::Vector2f(0, 0), sf::Vector2f _targetPos = sf::Vector2f(0, 0), float spd = 100, float headSpd = 1000) : targetPos(_targetPos), speed(spd), headSpeed(headSpd) {
		generateWorm(sz, startPos);
	}
	void generateWorm(size_t sz, sf::Vector2f stPos) {
		body.clear();
		//body.resize(sz);
		srand(colorSeed);
		sf::Color randColor(rand(), rand(), rand());
		for (size_t i = 0; i < sz; i++)
		{
			CustomWormCircle buffer;
			buffer.setFillColor(sf::Color(randColor.r * i, randColor.g * i, randColor.b * i, 210));
			buffer.setRadius(sz / 2);
			buffer.setOrigin({ (float)sz / 2, (float)sz / 2 });
			buffer.setPosition(stPos);
			body.emplace_back(std::move(buffer));
		}
	}
	const sf::Vector2f& getTargetPos() const {
		return targetPos;
	}
	const std::string& getName() const {
		return name;
	}
	void setTargetPos(const sf::Vector2f& obj) {
		targetPos = obj;
	}
	void setName(const std::string& from) {
		name = from;
	}
	std::vector<CustomWormCircle>& getBody() {
		return body;
	}
	void draw(sf::RenderWindow& window) {
		CustomWormCircle buffer; sf::Vector2f coords;
		for (auto& el : body) {
			buffer = el;
			coords = buffer.getPosition();
			buffer.setPosition(coords);
			window.draw(buffer);
		}
	}
	void update(float dTime) {
		for (size_t i = 0; i < body.size() - 1; i++)
		{
			sf::Vector2f direction = body[i].getPosition() - body[i + 1].getPosition();
			body[i + 1].move(direction * dTime * speed);
		}
		sf::Vector2f normDir = getNormalizedDirection(body[0].getPosition(), targetPos);
		sf::Vector2f length = getLength(normDir, speed);
		body[0].move(normDir * dTime * headSpeed);
	}
};