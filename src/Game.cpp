#include <Game.h>

void Game::mainLoop() {

}

void Game::render() {
	while (Game::_gameStarted) {
		for (const auto& elem : Game::_gameObjects) {
			elem->draw();
		}

	}
}

void Game::updatePhysics() {
	sf::Clock time;
	while (Game::_gameStarted) {
		float dTimeSec = time.restart().asSeconds();
		world.Step(dTimeSec, 8, 4);
	}
}

Game::Game() : world({0, 10}) {
	
}

Game::Game(int argc, char* argv[]) : world({ 0, 10 }) {

}

void Game::userInputListener() {

}

void Game::network() {

}

void Game::start() {
	Game::graphicsThread = new std::thread(Game::render);
	Game::phisicsThread  = new std::thread(Game::updatePhysics);
	Game::inputListenerThread = new std::thread(Game::userInputListener);
	Game::networkingThread    = new std::thread(Game::network);
}