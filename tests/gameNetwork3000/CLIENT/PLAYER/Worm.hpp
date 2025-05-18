#pragma once
#include <SFML/Graphics.hpp>
#include <MATH/math.hpp>
#include "TransformableObj.hpp"
#include "Camera.hpp"
#include <vector>
#include <memory>

class Worm : public TransformableObj {
private:
	std::vector<CustomWormCircle> body;
	sf::Vector2f targetPos;
	float speed;
	float headSpeed;
	size_t colorSeed = time(nullptr);
	std::string name;
public:
	const sf::Vector2f& _getPosition() const override {
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
	void importBody(const std::vector<float>& coords) {
		body.clear();
		body.resize(coords.size() / 2);
		sf::Color randColor(rand(), rand(), rand());
		for (size_t i = 0; i < body.size(); i++) {
			body[i].setPosition(sf::Vector2f(coords[i * 2], coords[i * 2 + 1]));
			body[i].setOrigin({(float)body.size() / 2, (float)body.size() / 2});
			body[i].setRadius(body.size() / 2);
			body[i].setFillColor(sf::Color(randColor.r * i, randColor.g * i, randColor.b * i, 210));
		}
	}
	const sf::Vector2f& _getTargetPos() const {
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
	sf::Vector2f _getSize() const {
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
		body[0].move(normDir * dTime * headSpeed * _getDimentinalK());
	}
	const float _getDimentinalK() const {
		float dimentinalK = (float)body.size() / 100;
		return dimentinalK;
	}
	sf::Vector2f _getTargetFraction() const {
		if (body.empty()) return sf::Vector2f(0, 0);
		return body[0].getGlobalBounds().size;
	}
};