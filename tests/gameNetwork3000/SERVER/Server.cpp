#include "Server.h"
#include <iostream>

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
			answer["action"] = req["action"];
			if (action == "send_message") {
				answer["status"] = "success";
				json message;
				message["action"] = "new_message";
				message["message"] = req["message"];
				broadcastMessage(message);
				spdlog::info("input message: {}", (std::string)req["message"]);
			}
			else if (action == "command") {
				json message;
				message["action"] = "command";
				message["message"] = req["message"];
				broadcastMessage(message, cSock);
			}
			else if (action == "command_spam") {
				json message;
				message["action"] = "command_spam";
				message["message"] = req["message"];
				broadcastMessage(message, cSock);
			}
			continue;
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