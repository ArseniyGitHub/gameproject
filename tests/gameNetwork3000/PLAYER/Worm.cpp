#pragma once
#include <SFML/Graphics.hpp>
#include "../MATH/math.hpp"
#include <vector>
#include <memory>

__interface TransformableObj {
public:
	virtual const sf::Vector2f& getPosition() const = 0;
	virtual const float getDimentinalK() const      = 0;
	virtual sf::Vector2f getTargetFraction() const  = 0;
	virtual sf::Vector2f getSize() const            = 0;
};

class CustomWormCircle : public sf::CircleShape {

};

class Camera : public sf::View {
	std::shared_ptr<TransformableObj> targetObject = nullptr;
	float speed = 5;
	float _zoom = 1;
	float k = 0.4;
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
		
		float idk = _zoom - targetObject->getDimentinalK();
		if (abs(idk) > 0.00001) {
			_zoom = targetObject->getDimentinalK();
			//this->zoom(_zoom);
			sf::Vector2f targetFraction(targetObject->getTargetFraction());
			targetFraction.x /= windowsIdk.getSize().x * k;
			targetFraction.y /= windowsIdk.getSize().y * k;
			float zoom = std::max(targetFraction.x, targetFraction.y);
			this->zoom(zoom);
		}

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
	Worm(size_t sz = 50, sf::Vector2f startPos = sf::Vector2f(0, 0), sf::Vector2f _targetPos = sf::Vector2f(0, 0), float spd = 100, float headSpd = 1000) : targetPos(_targetPos), speed(spd), headSpeed(headSpd) {
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
	sf::Vector2f getSize() const {
		if (body.empty()) return sf::Vector2f(0, 0);
		return sf::Vector2f(body[0].getRadius(), body[0].getRadius());
	}
	void update(float dTime) {
		for (size_t i = 0; i < body.size() - 1; i++)
		{
			sf::Vector2f direction = body[i].getPosition() - body[i + 1].getPosition();
			body[i + 1].move(direction * dTime * speed);
		}
		sf::Vector2f normDir = getNormalizedDirection(body[0].getPosition(), targetPos);
		sf::Vector2f length = getLength(normDir, speed * 1);
		body[0].move(normDir * dTime * headSpeed * getDimentinalK());
	}
	const float getDimentinalK() const {
		float dimentinalK = (float)body.size() / 100;
		return dimentinalK;
	}
	sf::Vector2f getTargetFraction() const {
		if (body.empty()) return sf::Vector2f(0, 0);
		return body[0].getGlobalBounds().size;
	}
};