#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <spdlog/spdlog.h>
#include "../PLAYER/Worm.cpp"
#include "ParserAPI.hpp"

template <typename type, typename type2> sf::Vector2<type> operator * (const sf::Vector2<type>& a, const sf::Vector2<type2>& b) {
	sf::Vector2<type> ret;
	ret.x = a.x * b.x;
	ret.y = a.y * b.y;
	return ret;
}

template <typename type, typename type2> sf::Vector2<type> operator / (const sf::Vector2<type>& a, const sf::Vector2<type2>& b) {
	sf::Vector2<type> ret;
	ret.x = a.x / b.x;
	ret.y = a.y / b.y;
	return ret;
}

class Window {
	sf::RenderWindow windows;
public:
	Window(size_t x = 800, size_t y = 800, std::wstring name = L"Epic game") : windows(sf::VideoMode(sf::Vector2u(x, y)), name) {

	}
	void mainLoop() {
		std::shared_ptr<Worm> player;
		player = std::make_shared<Worm>(5, sf::Vector2f(windows.getSize().x / 2, windows.getSize().y / 2));
		Camera cam(windows);
		cam.setTargetObject(player);
		sf::Vector2f windowS;
		sf::Clock timer;
		sf::Time time;

		sf::Texture ground;
		ground.loadFromFile("ground.jpg");
		sf::Sprite sprite(ground);

		Parser netparser;

		while (windows.isOpen()) {
			windowS = sf::Vector2f(windows.getSize().x, windows.getSize().y);
			sf::Time elapsedTime = timer.restart();
			time += elapsedTime;
			while (const std::optional e = windows.pollEvent()) {
				if (e->is<sf::Event::Closed>()) {
					//windows.close();
				}
				
			}
			sf::Vector2f mousePos = windows.mapPixelToCoords(sf::Mouse::getPosition(windows));
			player->setTargetPos(mousePos);
			player->update(elapsedTime.asSeconds());

			//spdlog::info("coords: x={}, y={}", player->getPosition().x, player->getPosition().y);

			sf::Vector2i currentChank(cam.getCenter().x / ground.getSize().x, cam.getCenter().y / ground.getSize().y);
			sf::Vector2i screenChanks(windows.getSize().x / ground.getSize().x + 4, windows.getSize().y / ground.getSize().y + 4);


			for (unsigned __int32 x = 0; x < screenChanks.x; x++) {
				for (unsigned __int32 y = 0; y < screenChanks.y; y++) {

					sprite.setPosition((sf::Vector2f)ground.getSize() * (currentChank - (screenChanks / (int)2) + sf::Vector2i(x, y)));
					windows.draw(sprite);
				}
			}
			cam.update(windows, elapsedTime.asSeconds());

			player->draw(windows);
			windows.display();

			netparser.clear();
			for (size_t i = 0; i < player->getBody().size() * 2; i += 2) {
				netparser["coords"][i] = (float)player->getBody()[i / 2].getPosition().x;
				netparser["coords"][i + 1] = (float)player->getBody()[i / 2].getPosition().y;
			}
			__bytes* cpy = netparser.pack();

			netparser.clear();
			netparser.parse(cpy);

			for (size_t i = 0; i < player->getBody().size() * 2; i += 2) {
				player->getBody()[i / 2].setPosition(sf::Vector2f(netparser["coords"][i].getAs<float>(), netparser["coords"][i + 1].getAs<float>()));
			}
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