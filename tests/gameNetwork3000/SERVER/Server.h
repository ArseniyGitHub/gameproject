#pragma once
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
#include <fstream>
#include <fmt/core.h>
#include <imgui_freetype.h>
#include <box2d/box2d.h>
#include "../PLAYER/Worm.cpp"

using json = nlohmann::json;

class Server {
	
	std::mutex           messageMutex;
	std::vector<std::string> messages;
	sf::TcpListener           listener;
	std::vector<sf::UdpSocket*>udpClients;
	std::vector<sf::TcpSocket*>   clients;
	std::vector<Worm*> players;
	unsigned __int16 port;
	void broadcastMessage(json);
	void broadcastMessage(json, sf::TcpSocket*);
	void hClient(sf::TcpSocket* cSock, std::vector<sf::TcpSocket*>& clients);

public:
	void start();
	Server(unsigned __int16 port = 12345) : port(port) {}
};