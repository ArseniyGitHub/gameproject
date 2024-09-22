#pragma once
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <memory.h>
#include <spdlog/spdlog.h>
//using namespace sf;
//using namespace ImGui;
//using namespace std;

class Window {
private:
	sf::RenderWindow* window;
	void mainLoop();
	void initImGui();
public:
	Window();
	~Window();
	void run() {
		mainLoop();
	}
	void stop() {

	}
};