#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <thread>
#include <vector>
#include <string>
#include <queue>
#include <spdlog/spdlog.h>
#include "PLAYER/Worm.hpp"
#include "mainParser.hpp"
#include "Client.h"
#include <nlohmann/json.hpp>

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
	sf::IpAddress ip;
	ui16 port = 222 * 3;
	sf::TcpSocket tcp;
	sf::UdpSocket udp;

	std::thread network;
	std::thread  udpNet;
	std::thread listener;
	std::thread udpListener;
	std::atomic<bool> active = false;

	std::queue<std::string>  requestsTCP;
	std::mutex           requestMutexTCP;
	std::condition_variable requestCvTCP;

	std::queue<PARSER_V2::bytesVec>   requestsUDP;
	std::mutex           requestMutexUDP;
	std::condition_variable requestCvUDP;
public:
	Window(size_t x = 800, size_t y = 800, std::wstring name = L"Epic game", sf::IpAddress _ip = sf::IpAddress::LocalHost, ui16 _port = 222 * 3) : windows(sf::VideoMode(sf::Vector2u(x, y)), name), ip(_ip), port(_port) {

	}
	void initConnection() {

	}
	void initThreads() {
		udp.bind(port);
		udp.setBlocking(false);
		network = std::thread([this]() {
		connect:
			auto status = tcp.connect(ip, port);
			if (status != sf::Socket::Status::Done) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));   goto connect;
			}
			while (active.load()) {
				std::string req/* = "what time is now?"*/;
				{
					std::unique_lock <std::mutex> lock(requestMutexTCP);
					requestCvTCP.wait(lock, [this]() {return !requestsTCP.empty() || !active; });
					if (!active) break;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
					req = requestsTCP.front();
					requestsTCP.pop();
				}
				sf::Packet packet;
				packet << req;
				if (tcp.send(packet) == sf::Socket::Status::Done) {
					spdlog::error("хорошо");
				}
				else spdlog::info("всё плохо");
				std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 100));
			}
			});

		listener = std::thread([this]() {
			while (active.load()) {
				sf::Packet response;
				if (tcp.receive(response) == sf::Socket::Status::Done) {
					spdlog::error("все ок");
				}
				else {
					spdlog::info("все не очень хорошо...");
				}
			}
			});

		udpNet = std::thread([this]() {
			while (active.load()) {
				std::unique_lock <std::mutex> lock(requestMutexUDP);
				requestCvUDP.wait(lock, [this]() { return !requestsUDP.empty() || !active; });
				if (!active) break;
				if (udp.send(requestsUDP.front().data(), requestsUDP.front().size(), ip, port) != sf::Socket::Status::Done) {
					spdlog::error("cant send packet!");
				}
				else requestsUDP.pop();
			}
			});

		udpListener = std::thread([this]() {
			while (active.load()) {
				sf::Packet receiveP;
				std::optional<sf::IpAddress> senderIp;
				unsigned short senderPort;
				if (udp.receive(receiveP, senderIp, senderPort) != sf::Socket::Status::Done) {
					//spdlog::info("err");
					continue;
				}
				else {
					PARSER_V2::bytesVec v(receiveP.getDataSize(), (ui8*)receiveP.getData());
					try {
						PARSER_V2::Parser p;
						p.parse(&v, 0);

					}
					catch (const std::exception& ex) {
						spdlog::info("cant parse received data: {}", ex.what());
					}
				}
			}
			});
	}
	void mainLoop() {
		system("chcp 65001");
		active = true;
		initThreads();
		std::shared_ptr<Worm> player;
		player = std::make_shared<Worm>(100, sf::Vector2f(windows.getSize().x / 2, windows.getSize().y / 2));
		Camera cam(windows);
		cam.setTargetObject(player);
		sf::Vector2f windowS;
		sf::Clock timer;
		sf::Clock parserTimer;
		sf::Time time;

		sf::Texture ground;
		ground.loadFromFile("ground.jpg");
		sf::Sprite sprite(ground);

		PARSER_V2::Parser netparser;
		PARSER_V2::bytesVec bytes;



		windows.setFramerateLimit(120);

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
			sf::Vector2i screenChanks(windows.getSize().x / ground.getSize().x + 4 * player->_getDimentinalK(), windows.getSize().y / ground.getSize().y + 4 * player->_getDimentinalK());


			for (unsigned __int32 x = 0; x < screenChanks.x; x++) {
				for (unsigned __int32 y = 0; y < screenChanks.y; y++) {

					sprite.setPosition((sf::Vector2f)ground.getSize() * (currentChank - (screenChanks / (int)2) + sf::Vector2i(x, y)));
					windows.draw(sprite);
				}
			}
			

			player->draw(windows);
			windows.display();
			cam.update(windows, elapsedTime.asSeconds());
			if (parserTimer.getElapsedTime().asMilliseconds() >= 1000 / 60) {
				std::vector<float> coords;
				coords.resize(player->getBody().size() * 2);

				netparser.deleteData();
				for (size_t i = 0; i < player->getBody().size() * 2; i += 2) {
					coords[i] = (float)player->getBody()[i / 2].getPosition().x;
					coords[i + 1] = (float)player->getBody()[i / 2].getPosition().y;
				}
				netparser["coords"] = coords;
				netparser.pack(&bytes, 0);

				{
					std::lock_guard<std::mutex> lock(requestMutexUDP);
					requestsUDP.push(bytes);
				}
				
				requestCvUDP.notify_one();
				//spdlog::error("PARSING!!!\nPARSING!!!");
				parserTimer.restart();
				bytes.clear();
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