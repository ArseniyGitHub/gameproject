#include "Server.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../MATH/math.hpp"

void Server::start() {
	while (true) {
		listener.listen(Server::port);
		sf::TcpSocket* clientSocket = new sf::TcpSocket;
		if (listener.accept(*clientSocket) == sf::Socket::Done) {
			spdlog::error("new client = {}:{}", clientSocket->getRemoteAddress().toString(), Server::port);
			clients.push_back(clientSocket);
			std::thread([clientSocket, this]() {this->hClient(clientSocket, std::ref(clients)); }).detach();
		}
	}
}

void Server::hClient(sf::TcpSocket* cSock, std::vector<sf::TcpSocket*>& clients) {
	Worm player;
	players.push_back(&player);
	sf::Clock timer;
	while (true) {
		sf::Packet     paket;
		std::string requestStr;
		json answer;
		if (cSock->receive(paket) != sf::Socket::Done) {
			spdlog::error("cannot receive packet");
			break;
		}
		paket >> requestStr;
		if (requestStr.empty()) continue;

		try {
			json req = json::parse(requestStr);
			std::string action = req["action"];
			//answer["action"] = req["action"];
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
				answer["message"]["you"]["body"][i]["color"] = *((int32*) & player.getBody()[i].getFillColor());
				answer["message"]["you"]["segmSize"] = player.getBody()[0].getRadius();
			}
			size_t pId = 0;
			for (Worm* obj : players) {
				if (obj == &player) continue;
				for (size_t i = 0; i < obj->getBody().size(); i++)
				{
					answer["message"]["other"][pId]["body"][i]["coords"]["x"] = obj->getBody()[i].getPosition().x;
					answer["message"]["other"][pId]["body"][i]["coords"]["y"] = obj->getBody()[i].getPosition().y;
					answer["message"]["other"][pId]["body"][i]["color"] = *((int32*) & obj->getBody()[i].getFillColor());
					answer["message"]["other"][pId]["segmSize"] = obj->getBody()[0].getRadius();
				}
				pId++;
			}
			
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