#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <spdlog/spdlog.h>
#include "../PLAYER/Worm.cpp"

class Window {
	sf::RenderWindow windows;
public:
	Window(size_t x = 800, size_t y = 800, std::wstring name = L"Epic game") : windows(sf::VideoMode(x, y), name) {

	}
	void mainLoop() {
		std::shared_ptr<Worm> player;
		player = std::make_shared<Worm>(pow(10, 2) * 2, sf::Vector2f(windows.getSize().x / 2, windows.getSize().y / 2));
		Camera cam(windows);
		cam.setTargetObject(player);
		sf::Vector2f windowS;
		sf::Clock timer;
		sf::Time time;

		sf::Texture ground;
		ground.loadFromFile("ground.jpg");
		sf::Sprite sprite(ground);

		while (windows.isOpen()) {
			windowS = sf::Vector2f(windows.getSize().x, windows.getSize().y);
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
				}

			}
			sf::Vector2f mousePos = windows.mapPixelToCoords(sf::Mouse::getPosition(windows));
			player->setTargetPos(mousePos);
			player->update(elapsedTime.asSeconds());
			cam.update(windows, elapsedTime.asSeconds());
			spdlog::info("coords: x={}, y={}", player->getPosition().x, player->getPosition().y);
			windows.clear(sf::Color::White);

			windows.draw(sprite);

			player->draw(windows);
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