#include "server.h"

void Server::start() {

	while (true) {
		listener.listen(Server::port);
		sf::TcpSocket* clientSocket = new sf::TcpSocket;
		if (listener.accept(*clientSocket) == sf::Socket::Done) {
			spdlog::error("new client = {}:{}", clientSocket->getRemoteAddress().toString(), Server::port);
			clients.push_back(clientSocket);
			clientSocket->setBlocking(true);
			std::thread([&, this]() {this->hClient(clientSocket, std::ref(clients)); }).detach();
		}
	}
}

void Server::hClient(sf::TcpSocket* cSock, std::vector<sf::TcpSocket*>& clients) {

	/*
	sf::Packet     paket;
	std::string requestStr;
	json answer;
	while (cSock->receive(paket) != sf::Socket::Done) {}
	paket >> requestStr;

	try {
		json req = json::parse(requestStr);
		std::string action = req["action"];
		answer["action"] = req["action"];

		if (action == "send_messange") {
			answer["status"] = "success";
			json messange;
			messange["action"] = "new_messange";
			messange["messange"] = req["messange"];
			broadcastMessange(messange);
		}
	}
	catch (const json::parse_error& ex) {
		answer["action"] = "exception";
		answer["status"] = "error";
		answer["messange"] = "invalid json format";
	}
	paket << answer.dump();
	cSock->send(paket);
	*/
	
	while (true) {
		sf::Packet     paket;
		std::string requestStr;
		json answer;
		//spdlog::info("iter!");
		if (cSock->receive(paket) != sf::Socket::Done) break;
		paket >> requestStr;
		if (requestStr.empty()) continue;
		
		try {
			json req = json::parse(requestStr);
			std::string action = req["action"];
			answer["action"] = req["action"];
			if (action == "send_messange") {
				answer["status"] = "success";
				json messange;
				messange["action"] = "new_messange";
				messange["messange"] = req["messange"];
				broadcastMessange(messange);
				spdlog::info("input messange: {}", (std::string)req["messange"]);
			}
		}
		catch (const json::parse_error& ex) {
			answer["action"] = "exception";
			answer["status"] = "error";
			answer["messange"] = "invalid json format";
		}
		paket << answer.dump();
		cSock->send(paket);
		//spdlog::info("my clients: {}", Server::clients.size());

	}
	delete cSock;
	
}

void Server::broadcastMessange(json answer) {
	for (auto& client : clients) {
		sf::Packet p;
		p << answer.dump();
		client->send(p);
	}
	//spam(answer);
}