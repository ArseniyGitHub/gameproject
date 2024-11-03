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
#include <fmt/core.h>

#define cu8(str) reinterpret_cast<const char*>(u8##str)
using json = nlohmann::json;

namespace windowParam {
	float windowX = 800, windowY = 800;
}

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
			sf::Packet     paket;
			std::string messange;
			if (cSock->receive(paket) != sf::Socket::Done) {
				break;
			}
			paket >> messange;
			spdlog::info("a {}", messange);
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
			sf::TcpSocket* neClientSocket = new sf::TcpSocket;
			if (listener.accept(*neClientSocket) == sf::Socket::Done) {
				clients.push_back(neClientSocket);
				std::thread([&, this]() {this->hClient(neClientSocket, std::ref(clients)); }).detach();
				spdlog::info("I have no clients:( Help me and connect!");
			}
		}
	}
};

void neClientSocket(sf::TcpSocket& sock, std::mutex& muTex, std::vector<std::string>& messanges) {
	while (true) {
		sf::Packet paket;
		if (sock.receive(paket) == sf::Socket::Done) {
			std::string msg;
			paket >> msg;
			spdlog::info("smth went nice {}", msg);
			std::lock_guard<std::mutex> log(muTex);
			messanges.push_back(msg);
		}
	}
}

void NotGraphic(sf::TcpSocket& client) {
	static float winX = 400, winY = 400;
	static std::string t;
	winX = windowParam::windowX / 2, winY = windowParam::windowY / 2;
	ImGui::SetNextWindowSize(ImVec2(winX, winY), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(windowParam::windowX / 4, windowParam::windowY / 4));
	if (ImGui::Begin(cu8("CHAAAT!!! DONT FORGET ABOUT IT!!!!"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar)) {
		ImGui::End();
	}
	
	ImGui::SetNextWindowSize(ImVec2(winX, 40), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(windowParam::windowX / 4, windowParam::windowY / 4 + winY + 10));
	if (ImGui::Begin(cu8("text"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar)) {
		ImGui::InputText("smth", &t, 0);
		ImGui::SameLine();
		if (ImGui::Button("BUTTON! DONT FORGET MY NAME!!!", ImVec2(40, 40))) {
			if (!t.empty()) {
				sf::Packet products;
				products << t;
				client.send(products);
				t.clear();
			}
		}
		ImGui::End();
	}

}

int main() {
	srand(std::time(nullptr));
	//setlocale(LC_ALL, "rus");
	system("chcp 65001");
	std::cout << cu8("что вы выберете: обрабатывать клиентов или быть им? -> your anwer:😀 ");
	std::string answer;
	std::cin >> answer;
	if (answer == "no") {
		Not_A_Server neServer;
		neServer.start();
		return 0;
	}
	std::vector<std::string> messanges;
	std::mutex messangesMutex;
	sf::RenderWindow window(sf::VideoMode(800, 800), L"");
	sf::TcpSocket socket;;;;;;;;
	if (socket.connect("127.0.0.1", 12345) != sf::Socket::Done) {
		spdlog::info("I have many clients. Please, dont connect!");
		return -1;
	}
	std::thread netThread(neClientSocket, std::ref(socket), std::ref(messangesMutex), std::ref(messanges));
	ImGui::SFML::Init(window);
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Times.ttf", 20, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	ImGui::SFML::UpdateFontTexture();
	sf::Clock timer;
	while (window.isOpen()) {
		
		window.setPosition(sf::Vector2i(std::rand() % 1980, std::rand() % 1080));
		window.setSize(sf::Vector2u(std::rand() % 1980, std::rand() % 1080));
		sf::Time time = timer.restart();;;;;;;;;;;;;;;;
		sf::Event e;
		while (window.pollEvent(e)) {
			ImGui::SFML::ProcessEvent(window, e);
			
			switch (e.type) {
				
			case sf::Event::Closed:
				std::cout << "hehe" << std::endl;
				//window.close();
				break;
			case sf::Event::KeyPressed:
				switch (e.key.code) {
				case sf::Keyboard::Space: {
					sf::Packet   z;
					z <<  "privet";
					socket.send(z);
				}
					break;
				}
				break;
			case sf::Event::Resized: {
				windowParam::windowX = (float)window.getSize().x;
				windowParam::windowY = (float)window.getSize().y;
				break;
			}
			}
			
		}
		ImGui::SFML::Update(window, time);
		ImGui::ShowDemoWindow();
		NotGraphic(socket);
		window.clear();
		
		
		ImGui::SFML::Render();
		window.display();
		
	}
	netThread.join();
	ImGui::SFML::Shutdown();
	return 0;
}