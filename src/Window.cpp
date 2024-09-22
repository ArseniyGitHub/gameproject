#include "Window.h"

Window::Window() : window((new sf::RenderWindow(sf::VideoMode(800, 800), "Game"))) {
	initImGui();
}

void Window::mainLoop() {
	
	sf::Clock time;
	while (window->isOpen()) {

		sf::Event event;
		while (window->pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window->close();
				break;
			
			}
			ImGui::SFML::ProcessEvent(*window, event);
		}
		ImGui::SFML::Update(*window, time.restart());
		window->clear();
		ImGui::SFML::Render(*window);
		window->display();
	}
}

void Window::initImGui() {
	if(!ImGui::SFML::Init(*window))spdlog::error("ImGui init window failed");
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("C:/Windows/fonts/arial.ttf", 20, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	ImGui::SFML::UpdateFontTexture();
	spdlog::info("ImGui init scues full");
}

Window::~Window() {
	delete window;
}