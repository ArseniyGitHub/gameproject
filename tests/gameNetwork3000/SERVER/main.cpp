#include "Server.h"

int main() {
	system("chcp 65001");
	std::string configPath = "ConfigServer.json";
	json config;
	std::ifstream configFile(configPath);
	//std::string data;
	if (configFile.is_open()) {
		configFile >> config;
		configFile.close();
	}
	else {
		config["port"] = 12345;
		std::ofstream ofConfig(configPath);
		ofConfig << config.dump(4);
		ofConfig.close();
	}
	unsigned __int16 port = 12345;
	if (config.contains("port")) {
		port = config["port"].get<unsigned __int16>();
	}
	Server server(port);
	server.start();
}