#include "Server.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <MATH/math.hpp>
#include <unordered_map>
#include <mainParser.hpp>

using namespace PARSER_V2;

void Server::start() {
	if (udp.bind(port) == sf::Socket::Status::Done) {
		udp.setBlocking(false);
		spdlog::error("все норм");
		udpThread = std::thread([this]() {
			sf::SocketSelector selector;
			selector.add(udp);
			std::unordered_map<unsigned __int16, std::optional<sf::IpAddress>> clientsMap;
			while (true) {
				if (selector.wait(sf::milliseconds(10))) {
					if (selector.isReady(udp)) {
						std::vector<std::byte> buffer(1024 * 4);
						size_t sz;
						std::optional<sf::IpAddress> from;
						unsigned __int16 senderPort;
						auto status = udp.receive(buffer.data(), 1024 * 4, sz, from, senderPort);
						;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; ;;;;;;; ;; ;; ; ; ; ; ; ; ; ; ; ;; ; ; ; ; ; ; ; ;;;;;;; ;;; ;; ;;; ;;; ;;; ;;;;;;;;;;;; ;; ;;;; ;;; ;;; ;;;;;;;;; ;;; ;;; ;;; ;;; ;;;;;;; ; ;; ; ;; ;;; ;; ;; ;; ;;;;;;;;;;; ;; ;; ;;;; ;; ;; ;; ;; ;;; ;;; ;;; ;;
						if (status == sf::Socket::Status::Done) {
							clientsMap[senderPort] = from;
							spdlog::error("все норм");
							Parser p;
							try {
								p.parse(reinterpret_cast<bytesVec*>(&buffer), 0);
								for (const auto& [port, client] : clientsMap) {
									udp.send(buffer.data(), sz, client.value(), port);
								}

								if (p.isPackage()) {
									if (p.contains("coords")) {
										std::vector<float> data;
										p["coords"].exportData(data);
										for (size_t i = 0; i + 1 < data.size(); i += 2) {
											spdlog::error("[{}]: x = {}, y = {}", i / 2, data[i], data[i + 1]);
										}
										spdlog::warn("end of packet");
									}
									else {
										spdlog::info("нет блока \"coords\"!");
									}
								}
								else {
									spdlog::info("нет блока данных!");
								}
							}
							catch (const std::exception& ex) {
								spdlog::info("всё 100% норм(но это не точно): {}", ex.what());
							}
							
						}
					}
				}
			}
			});
	}
	while (true) {
		listener.listen(Server::port);
		sf::TcpSocket* clientSocket = new sf::TcpSocket;
		if (listener.accept(*clientSocket) == sf::Socket::Status::Done) {
			spdlog::error("new client = {}:{}", clientSocket->getRemoteAddress().value().toString(), Server::port);
			clients.push_back(clientSocket);
			//udpClients.push_back(new sf::UdpSocket());
			std::thread([clientSocket, this]() {this->hClient(clientSocket, std::ref(clients)); }).detach();
		}
	}
}

void Server::hClient(sf::TcpSocket* cSock, std::vector<sf::TcpSocket*>& clients) {
	Player player;
	players.push_back(&player);
	sf::Clock timer;
	while (true) {
		sf::Packet     paket;
		std::string requestStr;
		json answer;
		if (cSock->receive(paket) != sf::Socket::Status::Done) {
			spdlog::error("cannot receive packet");
			break;
		}
		paket >> requestStr;
		if (requestStr.empty()) continue;
		
		try {
			json req = json::parse(requestStr);
			std::string action = req["action"];
			//answer["action"] = req["action"];
			/*
			if (action == "moving") {
				float x = req["message"]["x"];
				float y = req["message"]["y"];
				size_t segments = req["message"]["segments"];
				size_t lSize = player.getBody().size();
				if (lSize != segments) {
					player.getBody().resize(segments);
					if (lSize < segments) {
						for (size_t i = lSize; i < segments; i++)
						{
							player.getBody()[i].setPosition(player.getBody()[lSize - 1].getPosition());
							player.getBody()[i].setFillColor(sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255, std::rand() % 255));
						}
					}
				}
				player.setTargetPos(sf::Vector2f(x, y));
				player.update(timer.restart().asSeconds());
			}
			

			//

			answer["action"] = "playersData";
			answer["status"] = "normal";
			
			for (size_t i = 0; i < player.getBody().size(); i++)
			{
				answer["message"]["you"]["body"][i]["coords"]["x"] = player.getBody()[i].getPosition().x;
				answer["message"]["you"]["body"][i]["coords"]["y"] = player.getBody()[i].getPosition().y;
				answer["message"]["you"]["body"][i]["color"] = (player.getBody()[i].getFillColor()).toInteger();
				answer["message"]["you"]["segmSize"] = player.getBody()[0].getRadius();
			}
			size_t pId = 0;
			for (Worm* obj : players) {
				if (obj == &player) continue;
				for (size_t i = 0; i < obj->getBody().size(); i++)
				{
					answer["message"]["other"][pId]["body"][i]["coords"]["x"] = obj->getBody()[i].getPosition().x;
					answer["message"]["other"][pId]["body"][i]["coords"]["y"] = obj->getBody()[i].getPosition().y;
					answer["message"]["other"][pId]["body"][i]["color"] = obj->getBody()[i].getFillColor().toInteger();
					answer["message"]["other"][pId]["segmSize"] = obj->getBody()[0].getRadius();
				}
				pId++;
			}
			*/
			
			//

		}
		catch (const json::parse_error& ex) {
			answer["action"] = "exception";
			answer["status"] = "error";
			answer["message"] = "invalid json format";
		}
		
		

		paket << answer.dump();
		cSock->send(paket);
	}
	delete cSock;
}


void Server::broadcastMessage(json answer) {
	sf::Packet p;
	p << answer.dump();
	for (auto& client : clients) {
		client->send(p);
	}
	//spam(answer);
}

void Server::broadcastMessage(json answer, sf::TcpSocket* ignore) {
	sf::Packet p;
	p << answer.dump();
	for (auto& client : clients) {
		if (client == ignore) continue;
		client->send(p);
	}
}