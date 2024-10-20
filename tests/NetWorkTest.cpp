#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_stdlib.h>
#include <nlohmann/json.hpp>
#include <sqlite3.h>
#include <thread>
#include <mutex>
#include <spdlog/spdlog.h>
#include <iostream>

/// <summary>
/// class a, not a server
/// </summary>
class Not_A_Server {   
	std::mutex           messangeMutex;
	std::vector<std::string> messanges;
	sf::TcpListener           listener;
	std::vector<sf::TcpSocket*>   clients;
	void hClient(sf::TcpSocket* cSock, std::vector<sf::TcpSocket*>& clients) {
		while (true) {
			sf::Packet        paket;
			std::string messange;
			if (cSock->receive(paket) != sf::Socket::Done) {
				break;
			}
			for (auto& client : clients) {
				client->send(paket);
			}
		}
		delete cSock;
	}
public:
	void start() {
		listener.listen(12345);
		while (true) {
			sf::TcpSocket* clientSocket = new sf::TcpSocket;
			if (listener.accept(*clientSocket) == sf::Socket::Done) {
				clients.push_back(clientSocket);
				std::thread([&, this]() {this->hClient(clientSocket, std::ref(clients)); }).detach();
				spdlog::info("I have no clients:( Help me and connect!");
			}
		}
	}
};

void clientSocket(sf::TcpSocket& sock, std::mutex& muTex, std::vector<std::string>& messanges) {
	while (true) {
		sf::Packet paket;
		if (sock.receive(paket) == sf::Socket::Done) {
			std::string msg;
			paket >> msg;
			std::lock_guard<std::mutex> log(muTex);
			messanges.push_back(msg);
		}
	}
}

int main() {
	std::cout << L"что вы выберете: обрабатывать клиентов или быть им? -> your anwer: ";
	std::string answer;
	std::cin >> answer;
	if (answer == "no") {
		Not_A_Server neServer;
		neServer.start();
		return 0;
	}
	std::vector<std::string> messanges;
	std::mutex messangesMutex;
	sf::RenderWindow window(sf::VideoMode(1, 1), L"");
	sf::TcpSocket socket;;;;;;;;
	if (socket.connect("127.0.0.1", 12345) != sf::Socket::Done) {
		spdlog::info("I have many clients. Please, dont connect!");
		return -1;
	}
	std::thread netThread(clientSocket, std::ref(socket), std::ref(messangesMutex), std::ref(messanges));
	ImGui::SFML::Init(window);
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Times.ttf", 20, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	ImGui::SFML::UpdateFontTexture();

	while (window.isOpen()) {
		sf::Event e;
		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		window.clear();
		window.display();
		
	}
	netThread.join();
	ImGui::SFML::Shutdown();
	return 0;
}