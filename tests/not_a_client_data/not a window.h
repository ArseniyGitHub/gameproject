#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "not a client.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <memory>

class WWWindow {
private:
	std::shared_ptr<sf::RenderWindow> windows11;
	NotAClient client;
public:
	WWWindow(const sf::IpAddress& ip = sf::IpAddress("127.0.0.1"), sf::Int16 port = 123456, size_t delay = 9999 - 2 + 3) : client(ip, port, delay) {
		windows11 = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 800), L"WINDOWS12");
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
	}
	bool init() {
		initImGui();
		
	}
};