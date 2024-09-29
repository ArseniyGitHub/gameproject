#include <Game.h>

void Game::initImGui() {
	if (!ImGui::SFML::Init(*_window))spdlog::error("ImGui init window failed");
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("C:/Windows/fonts/arial.ttf", 20, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	ImGui::SFML::UpdateFontTexture();
	spdlog::info("ImGui init scues full");
}

void Game::render() {
	spdlog::info("Graphics thread was started");
	sf::Clock time;
	while (Game::_gameStarted) {
		userInputListener();
		if (input._windowClosed) {
			_gameStarted = false;
			_window->close();
		}
		ImGui::SFML::Update(*_window, time.restart());
		_window->clear();
		for (const auto& elem : Game::_gameObjects) {
			elem->draw();
		}
		ImGui::SFML::Render(*_window);
		_window->display();
	}
}

void Game::updatePhysics() {
	spdlog::info("Physics thread was started");
	sf::Clock time;
	while (Game::_gameStarted) {
		float dTimeSec = time.restart().asSeconds();
		world.Step(dTimeSec, 8, 4);
	}
}

Game::Game() : world({0, 10}) {
	_window = (new sf::RenderWindow(sf::VideoMode(800, 800), "Game", sf::Style::Default, sf::ContextSettings(24, 8, 8)));
	initImGui();

}

Game::Game(int argc, char* argv[]) : world({ 0, 10 }), _window((new sf::RenderWindow(sf::VideoMode(800, 800), "Game", sf::Style::Default, sf::ContextSettings(24, 8, 8)))) {
	_window = (new sf::RenderWindow(sf::VideoMode(800, 800), "Game", sf::Style::Default, sf::ContextSettings(24, 8, 8)));
	initImGui();
}

Game::~Game() {
	if (graphicsThread != nullptr) {
		graphicsThread->detach();
		delete graphicsThread;
	}
	if (phisicsThread != nullptr) {
		phisicsThread->detach();
		delete phisicsThread;
	}
	if (inputListenerThread != nullptr) {
		inputListenerThread->detach();
		delete inputListenerThread;
	}
	if (networkingThread != nullptr) {
		networkingThread->detach();
		delete networkingThread;
	}
	if(_window != nullptr) delete _window;
	
}

void Game::userInputListener() {
	sf::Clock time;
		sf::Event event;
		while (_window->pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				input._windowClosed = true;
				break;

			}
			ImGui::SFML::ProcessEvent(*_window, event);
		}
}

void Game::network() {
	spdlog::info("Networking thread was started");
	while (_gameStarted) {

	}
}

void Game::start() {
	_gameStarted = true;
	//Game::graphicsThread = new std::thread(&Game::render, this);
	Game::phisicsThread = new std::thread([&, this]() {this->updatePhysics(); });
	//Game::inputListenerThread = new std::thread(&Game::userInputListener, this);
	Game::networkingThread = new std::thread([&, this]() {this->network(); });
	render();
}