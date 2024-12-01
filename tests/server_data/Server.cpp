#include "server.h"

void Server::start() {
	listener.listen(Server::port);
	while (true) {
		sf::TcpSocket* neClientSocket = new sf::TcpSocket;
		if (listener.accept(*neClientSocket) == sf::Socket::Done) {
			clients.push_back(neClientSocket);
			std::thread([&, this]() {this->hClient(neClientSocket, std::ref(clients)); }).detach();
			spdlog::info("I have no clients:( Help me and connect!");
		}
	}
}

void Server::hClient(sf::TcpSocket* cSock, std::vector<sf::TcpSocket*>& clients) {
	while (true) {
		sf::Packet     paket;
		std::string requestStr;
		json answer;
		if (cSock->receive(paket) != sf::Socket::Done) {
			break;
		}
		paket >> requestStr;
		spdlog::info("a {}", requestStr);
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
			answer["messange"] = "invalid_json_format";
		}
		paket << answer.dump();
		cSock->send(paket);

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