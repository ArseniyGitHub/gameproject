#include "Server.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <MATH/math.hpp>
#include <unordered_map>
#include <mainParser.hpp>
#include <algorithm>

using namespace PARSER_V2;

Parser* Server::getPlayersData() {
	Parser* data = new Parser;
	(*data)["action"] = std::string("playerInfo");
	std::vector<float> coords;
	for (size_t i = 0; i < players.size(); i++) {
		for (size_t o = 0; o < players[i]->body.size(); o++) {
			coords.clear();
			coords[i * 2] = players[i]->body[o].x;
			coords[i * 2 + 1] = players[i]->body[o].y;
		}
		(*data)["data"][i]["coords"] = coords;
		(*data)["data"][i]["name"] = players[i]->getName();
	}
	return data;
}

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
						

						if (status == sf::Socket::Status::Done) {
							clientsMap[senderPort] = from;
							spdlog::error("все норм");
							Parser p;
							try {
								p.parse(reinterpret_cast<bytesVec*>(&buffer), 0);
								

								if (p.isPackage()) {
									if (p.contains("coords")) {
										std::vector<float> data;
										p["coords"].exportData(data);
										for (size_t i = 0; i + 1 < data.size(); i += 2) {
											spdlog::error("[{}]: x = {}, y = {}", i / 2, data[i], data[i + 1]);
										}
										spdlog::warn("end of packet");
										for (Player* e : players) {
											if (e->getUUID() != *p["uuid"].getAs<boost::uuids::uuid>()) continue;
											e->body.clear();
											e->body.resize(data.size() / 2);
											for (size_t i = 0; i < data.size() / 2; i++) {
												e->body[i].x = data[i * 2];
												e->body[i].y = data[i * 2 + 1];
											}
										}
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

						Parser* packet = getPlayersData();
						bytesVec packetD;
						packet->pack(&packetD, 0);

						for (const auto& [port, client] : clientsMap) {
							udp.send(packetD.data(), sz, client.value(), port);
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

json Server::processInit(const json& msg, sf::TcpSocket& sock) {
	if (msg.contains("name")) {
		std::string name = msg["name"];
		auto player = std::find_if(this->players.begin(), this->players.end(), [&](Player* o) {return o->getName() == name; });
		if (this->players.end() != player) {
			json answer;
			answer["action"] = "init";
			answer["status"] = "you are noob hacker :)";

			return answer;
		}
		else {
			json answer;
			answer["action"] = "init";
			answer["status"] = "OK, you beat me";
			spdlog::info("new initialized client!");
			Player* obj = new Player(name, sock.getRemoteAddress(), sock.getRemotePort());
			this->players.push_back(obj);
			answer["data"]["uuid"] = obj->getUUID();
			answer["data"]["ip"] = obj->getIP().value().toString();
			answer["data"]["port"] = obj->getPort();
			
			return answer;
		}
	}
	else {
		json answer;
		answer["action"] = "init";
		answer["status"] = "Your connection has some problems... Please try hacking later";

		return answer;
	}
}

json Server::processReq(const json& msg, sf::TcpSocket& sock) {
	json answer;
	if (!msg.contains("action")) {
		answer["action"] = "info";
		answer["status"] = "error";
		answer["info"] = "cant process request";
		return answer;
	}
	std::string action = msg["action"];

	if (action == "init") return processInit(msg, sock);
	
	answer["action"] = "info";
	answer["status"] = "error";
	answer["info"] = "cant process request";
	return answer;
}

void Server::hClient(sf::TcpSocket* cSock, std::vector<sf::TcpSocket*>& clients) {
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
			spdlog::info("какой-то пакет пришёл, надо изучить...({}:{})", cSock->getRemoteAddress().value().toString(), cSock->getRemotePort());
			std::string action = req["action"];
			if (action == "init") {
				answer = processInit(req, *cSock);
			}
		}
		catch (const json::parse_error& ex) {
			answer["action"] = "exception";
			answer["status"] = "error";
			answer["message"] = "invalid json format";
		}

		paket.clear();
		paket << answer.dump();
		cSock->send(paket);
	}

end:
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