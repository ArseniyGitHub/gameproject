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

class NotAClient {
private:
	sf::IpAddress ip;
	sf::Int16   port;
    size_t     delay;
	sf::TcpSocket socket;
public:
	NotAClient(const sf::IpAddress& ip = sf::IpAddress("127.0.0.1"), sf::Int16 port = 123456, size_t delay = 1000) : ip(ip), port(port), delay(delay) {

	}
};