#include "Server.h"

int main() {
	std::string configPath = "configS.json";
	json config;
	std::ifstream configFile(configPath);
	//std::string data;
	if (configFile.is_open()) {
		configFile >> config;
		configFile.close();
	}
	else {
		config["port"] = 123456;
		std::ofstream ofConfig(configPath);
		ofConfig << config.dump(4);
		ofConfig.close();
		//config["delay"] = 1000;
	}
	unsigned __int16 port;
	if (config.contains("port")) {
		port = config["port"].get<unsigned __int16>();
	}
	Server server(port);
	server.start();
}