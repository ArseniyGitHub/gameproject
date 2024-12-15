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

using json = nlohmann::json;

class Server {
	std::mutex           messangeMutex;
	std::vector<std::string> messanges;
	sf::TcpListener           listener;
	std::vector<sf::TcpSocket*>   clients;
	unsigned __int16 port;
	void broadcastMessange(json answer);
	void hClient(sf::TcpSocket* cSock, std::vector<sf::TcpSocket*>& clients);
public:
	void start();
	Server(unsigned __int16 port = 12345) : port(port) {}
};