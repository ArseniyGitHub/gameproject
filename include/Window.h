#pragma once
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <memory.h>
//using namespace sf;
//using namespace ImGui;
//using namespace std;

class Window {
private:
	std::shared_ptr<sf::RenderWindow> window;
	void mainLoop();
public:
	Window();
	void run() {
		
	}
	void stop() {

	}
};