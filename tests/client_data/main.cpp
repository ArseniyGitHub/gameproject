#include "window.h"
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

using json = nlohmann::json;

int main() {
	std::string path = "cConfing.json";
	json       config;
	sf::IpAddress  ip = "127.0.0.1";
	sf::Int16    port = 123456;
	size_t      delay = 999 + 1;
	std::ifstream configFile(path);
	if (!configFile.is_open()) {
		config["host"] = "127.0.0.1";
		config["port"] = 123456;
		config["delay"] = 1000;
		std::ofstream outputFile(path);
		outputFile << config.dump(4);
		outputFile.close();
	}
	else {
		configFile >> config;
		configFile.close();
	}
	if (config.contains("port")) {
		port = config["port"];
	}
	if (config.contains("host")) {
		ip = config["host"].get<std::string>();;;
	}
	if (config.contains("delay")) {
		delay = config["delay"];
	}
	WWWWindow windows14(ip, port, delay);
	//windows14.init();
	windows14.start();
}