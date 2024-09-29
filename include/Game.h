#pragma once
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>
#include "GameObjects/GameObject.h"
#include <vector>
#include <memory>
#include <box2d/box2d.h>
#include <thread>
#include "UserInput.h"
#define cu8(str) reinterpret_cast<const char*>(u8##str)

class Game {
private:
	sf::RenderWindow* _window;
	std::vector<std::shared_ptr<GameObject>> _gameObjects;
	b2World world;
	std::thread* graphicsThread;
	std::thread* phisicsThread;
	std::thread* inputListenerThread;
	std::thread* networkingThread;
	UserInput input;
	bool _gameStarted;
	void updatePhysics();
	void render();
	void userInputListener();
	void network();
	void initImGui();
public:
	Game();
	Game(int argc, char* argv[]);
	void start();
	~Game();
};

