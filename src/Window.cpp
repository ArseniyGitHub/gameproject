#include "Window.h"

Window::Window() : window(std::make_shared<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(800, 800), "Game"))) {

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
		}
		window->clear();
		window->display();
	}
}