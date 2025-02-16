#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../MATH/math.hpp"

class CustomWormCircle : public sf::CircleShape {

};

class Camera : public sf::View {};

class Worm {
private:
	std::vector<CustomWormCircle> body;
	sf::Vector2f targetPos;
	float speed;
	float headSpeed;
	size_t colorSeed = time(nullptr);
public:
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
			buffer.setOrigin(sz / 2, sz / 2);
			buffer.setPosition(stPos);
			body.emplace_back(std::move(buffer));
		}
	}
	const sf::Vector2f& getTargetPos() const {
		return targetPos;
	}
	void setTargetPos(const sf::Vector2f& obj) {
		targetPos = obj;
	}
	std::vector<CustomWormCircle>& getBody() {
		return body;
	}
	void draw(sf::RenderWindow& window) {
		for (auto& el : body) {
			window.draw(el);
		}
	}
	void update(float dTime) {
		for (size_t i = 0; i < body.size() - 1; i++)
		{
			sf::Vector2f direction = body[i].getPosition() - body[i + 1].getPosition();
			body[i + 1].move(direction * dTime * speed);
		}
		sf::Vector2f normDir = getNormalizedDirection(body[0].getPosition(), targetPos);
		body[0].move(normDir * dTime * headSpeed);
	}
};

class Window {
	sf::RenderWindow windows;
public:
	Window(size_t x = 800, size_t y = 800, std::wstring name = L"Epic game") : windows(sf::VideoMode(x, y), name) {

	}
	void mainLoop() {
		Worm player(pow(10, 2) * 2, sf::Vector2f(windows.getSize().x / 2, windows.getSize().y / 2));
		sf::Clock timer;
		sf::Time time;
		while (windows.isOpen()) {
			sf::Time elapsedTime = timer.restart();
			time += elapsedTime;
			sf::Event e;
			while (windows.pollEvent(e)) {
				switch (e.type) {

				case sf::Event::Closed:
					break;
				case sf::Event::KeyPressed:
					switch (e.key.code) {
					case sf::Keyboard::Escape:
						windows.close();
						break;
					}
					break;
				case sf::Event::MouseMoved:
					sf::Vector2f mousePos = windows.mapPixelToCoords(sf::Vector2i(e.mouseMove.x, e.mouseMove.y));
					player.setTargetPos(mousePos);
					break;
				}

			}
			player.update(elapsedTime.asSeconds());
			windows.clear(sf::Color::White);

			player.draw(windows);
			windows.display();

		}
	}
	void testScene(Worm& player, float time) {
		sf::Vector2f coords = { sin(time) * 200 + windows.getSize().x / 2, cos(time) * 200 + windows.getSize().y / 2 };
		auto& body = player.getBody();
		for (size_t i = 0; i < body.size() - 1; i++) {
			body[i + 1].setPosition(body[i].getPosition() * 0.90f);
		}
		body[0].setPosition(coords);
	}
};