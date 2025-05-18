#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include "imgui-SFML.h"

int main() {

	sf::Shader shader;
	if (!shader.loadFromFile("assets/shaders/pulseShader.frag", sf::Shader::Type::Fragment)) spdlog::error("cant load shader!");
	sf::Texture img("assets/images/epic2.jpg");
	sf::Sprite sprite1(img);

	sf::RenderWindow windows12(sf::VideoMode(sf::Vector2u(800, 800)), L"helloOpenGL.what");
	if (!ImGui::SFML::Init(windows12)) {
		spdlog::info("cant init imgui-sfml, your computer will shutdown in 60 sec");
		//system("shutdown /s /t 60");
	}
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("assets/Fonts/roboto/Roboto-Black.ttf", 20, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	ImGui::SFML::UpdateFontTexture();
	ImGui::StyleColorsLight();

	sf::Clock tmr;
	float time_ = 0;
	sprite1.setPosition(sf::Vector2f(0, 0));
	while (windows12.isOpen()) {
		sf::Time elapsedT = tmr.restart();
		time_ += elapsedT.asSeconds();
		shader.setUniform("time", time_);
		while (auto e = windows12.pollEvent()) {
			ImGui::SFML::ProcessEvent(windows12, *e);
			if (e->is<sf::Event::Closed>()) {
				windows12.close();
			}
		}
		ImGui::SFML::Update(windows12, elapsedT);
		windows12.clear();
		//ImGui::ShowDemoWindow();
		ImGui::SFML::Render();
		windows12.draw(sprite1, &shader);
		windows12.display();
	}
	ImGui::SFML::Shutdown();
}