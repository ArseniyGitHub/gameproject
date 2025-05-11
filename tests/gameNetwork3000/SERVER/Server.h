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
#include <box2d/box2d.h>
#include <boost/uuid.hpp>
#include <time.h>
#include <models/Player.hpp>

using json = nlohmann::json;

class Server {
	
	std::mutex           messageMutex;
	std::vector<std::string> messages;
	sf::TcpListener           listener;
	sf::UdpSocket udp;
	std::vector<sf::TcpSocket*>   clients;
	std::vector<Player*> players;
	std::thread udpThread;
	unsigned __int16 port;
	void broadcastMessage(json);
	void broadcastMessage(json, sf::TcpSocket*);
	void hClient(sf::TcpSocket* cSock, std::vector<sf::TcpSocket*>& clients);
	json processInit(const json& msg, sf::TcpSocket& sock);
	json processReq(const json& msg, sf::TcpSocket& sock);

public:
	void start();
	Server(unsigned __int16 port = 222 * 3) : port(port) {}
};

class P_L_A_Y_E_R {
public:
	static boost::uuids::random_generator generatorUUID;
	std::vector<float> coords;
	std::optional<sf::IpAddress> ip;
	unsigned short port;
	boost::uuids::uuid guid;
	std::string name;
	P_L_A_Y_E_R(std::string name = "unknow amogus 321(sus)",
		const std::optional<sf::IpAddress>& ip = std::optional<sf::IpAddress>(sf::IpAddress::LocalHost),
		const unsigned short port = 222 * 3, boost::uuids::uuid id = generatorUUID()) :
		ip(ip), port(port), name(name), guid(id) {;;}
};

class Game_State {
	std::vector<P_L_A_Y_E_R> players;
};