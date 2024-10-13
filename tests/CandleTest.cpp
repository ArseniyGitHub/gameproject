#include <SFML/Graphics.hpp>
#include <Candle/Candle.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode(1000, 1000), L"свечка тест");
	candle::RadialLight light;
	light.setRange(1000);
	light.setColor(sf::Color(0, 150, 150));
	light.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	candle::EdgeVector a;
	a.emplace_back(sf::Vector2f(200, 100), sf::Vector2f(100, 200));
	light.castLight(a.begin(), a.end());
	window.setFramerateLimit(5);
	while (window.isOpen()) {
		sf::Event e;
		while (window.pollEvent(e)) {
			switch (e.type){
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		window.clear();
		window.draw(light);
		window.display();
	}
}