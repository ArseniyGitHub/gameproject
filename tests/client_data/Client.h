#pragma once
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
#include <imgui_freetype.h>
#include <string>
#include <atomic>



using json = nlohmann::json;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
class Client {
public:
	enum ConnectionStatus {
		Connected,
		Disconnected,
		Connecting
	};
private:
	sf::IpAddress ip;
	sf::Int16   port;
    size_t     delay;
	sf::TcpSocket socket;
	std::atomic<bool> isStarted = false;
	std::atomic<ConnectionStatus> connectionStatus = ConnectionStatus::Disconnected;
	std::thread receiveThread;
	std::thread requestThread;

public:
	
	Client(const sf::IpAddress& ip = sf::IpAddress("127.0.0.1"), sf::Int16 port = 123456, size_t delay = 1000) : ip(ip), port(port), delay(delay) {
		//isStarted = true;
	}
	bool connect() {
	    return socket.connect(ip, port) == sf::Socket::Done;
	}
	void sendMessangeRequest(std::string messange) {
		json request;
		request["action"] = "send_messange";
		request["messange"] = messange;
		sf::Packet p;
		p << request.dump();
		socket.send(p);
	}
	bool sendRequest(const json& msg) {
		sf::Packet packet;
		packet << msg.dump();
		return socket.send(packet) == sf::Socket::Done;
	}
	bool receiveResponse(sf::Packet& packet) {
		return socket.receive(packet) == sf::Socket::Done;
	}
	void processServerResponse(sf::Packet& packet) {
		std::string responseMessage;
		packet >> responseMessage;
		json response;
		try {
			response = json::parse(responseMessage);
			spdlog::info("json dump: \n{}", response.dump());
		}
		catch (const std::exception& ex) {
			spdlog::error("cannot parse json packet: \n{}", ex.what());
			return;
		}
	}
	void start() {
		requestThread = std::thread([this]() {
			isStarted.store(true);
			while (isStarted.load()) {
				if (connectionStatus != ConnectionStatus::Connected && isStarted.load()) {
					connectionStatus = ConnectionStatus::Connecting;
					while (connectionStatus != ConnectionStatus::Connected && isStarted.load()) {
						spdlog::info("try connection to server...");
						if (connect()) {
							connectionStatus = ConnectionStatus::Connected;
							spdlog::info("connected to the server {}:{}", ip.toString(), port);
						}
					}
				}
			}
		});
		receiveThread = std::thread([this]() {
			while (!isStarted.load()) std::this_thread::sleep_for(std::chrono::milliseconds(10));
			while (isStarted.load()) {
				sf::Packet packet;
				if (receiveResponse(packet)) {

				}
			}
			});
	}
};

