#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "client.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <memory>

class WWWWindow {
private:
	std::shared_ptr<sf::RenderWindow> windows11;
	Client client;
public:
	WWWWindow(const sf::IpAddress& ip = sf::IpAddress("127.0.0.1"), sf::Uint16 port = 12345, size_t delay = 10000) : client(ip, port, delay) {
		windows11 = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 800), L"WINDOWS12");
		init();
	}
	bool initImGui() {
		ImGui::SFML::Init(*windows11);
		ImGui::CreateContext();
		auto& io = ImGui::GetIO();
		io.Fonts->Clear();
		io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Times.ttf", 20, nullptr, io.Fonts->GetGlyphRangesCyrillic());
		ImFontConfig c;
		c.OversampleH = c.OversampleV = 1;
		c.MergeMode = 1;
		c.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
		ImWchar ranges[] = { 0x1, 0xFFFF, 0 };
		io.Fonts->AddFontFromFileTTF("assets/Fonts/seguiemj.ttf", 20 / 0.1, &c, ranges);
		ImGui::SFML::UpdateFontTexture();
		return true;
	}
	bool init() {
		windows11->setFramerateLimit(61);
		client.start();
		return initImGui();
	}
	
	void mainWindow() {
		static float winX = 400, winY = 400;
		static std::string t;
		ImGui::SetNextWindowSize(ImVec2(winX, winY), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(windows11->getSize().x / 4, windows11->getSize().y / 4));
		if (ImGui::Begin(("CHAAAT!!! DONT FORGET ABOUT IT!!!!"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar)) {
			ImGui::End();
		}

		ImGui::SetNextWindowSize(ImVec2(winX, 40), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(windows11->getSize().x / 4, windows11->getSize().y / 4 + winY + 10));
		if (ImGui::Begin(("text"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar)) {
			ImGui::InputText("smth", &t, 0);
			ImGui::SameLine();
			if (ImGui::Button("BUTTON! DONT FORGET MY NAME!!!", ImVec2(40, 40))) {
				if (!t.empty()) {
					if (t[0] == '/') {
						client.sendCommand(t.substr(1, t.size() - 1));
					}
					else {
						client.sendMessageRequest(t);
					}
					t.clear();
				}
			}
			ImGui::End();
		}
	}
	void render() {
		sf::Clock timer;
		while (windows11->isOpen()) {
			
			//windows11->setPosition(sf::Vector2i(std::rand() % 1980, std::rand() % 1080));
			//windows11->setSize(sf::Vector2u(std::rand() % 1980, std::rand() % 1080));
			sf::Time time = timer.restart();;;;;;;;;;;;;;;;;;;;;;;;
			sf::Event e;
			while (windows11->pollEvent(e)) {
				ImGui::SFML::ProcessEvent(*windows11, e);

				switch (e.type) {

				case sf::Event::Closed:
					//std::cout << "hehe" << std::endl;
					windows11->close();
					return;
				case sf::Event::KeyPressed:
					switch (e.key.code) {
					case sf::Keyboard::Escape:
						client.stop();
						windows11->close();
						break;
					}
					break;
				}

			}
			ImGui::SFML::Update(*windows11, time);
			mainWindow();
			windows11->clear();
			ImGui::SFML::Render();
			windows11->display();

		}
	}
	void start() {
		render();
	}
};