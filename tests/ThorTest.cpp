#include <Thor/Animations.hpp>
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include <cstdint>

typedef unsigned __int64 ui64;
typedef unsigned __int32 ui32;
typedef unsigned __int16 ui16;
typedef unsigned  __int8  ui8;
typedef signed    __int64 i64;
typedef signed    __int32 i32;
typedef signed    __int16 i16;
typedef signed     __int8  i8;
typedef char                c;
typedef wchar_t          wc_t;
typedef const wchar_t   cwc_t;
typedef const char         cc;

template <ui64 sz> ui64 getsize(cc txt[sz]) {
	return sz;
}

int main() {

	

	sf::RenderWindow window(sf::VideoMode(512, 512), L"что-то");
	sf::Texture at;
	at.loadFromFile("assets/a.png");
	sf::Sprite a(at);
	a.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	thor::FrameAnimation b;
	thor::AnimationMap<sf::Sprite, std::string> d;
	for (unsigned __int16 i = 0; i < 9; i++) {
		for (unsigned __int16 u = 0; u < 9; u++)
		{
			b.addFrame(1, sf::IntRect(100 * u, 100 * i, 100, 100));
			
		}
	}
	d.addAnimation("smth", b, sf::seconds(3.456789));
	thor::Animator<sf::Sprite, std::string> c(d);
	c.play() << "smth";
	sf::Clock time;
	while (window.isOpen()) {
		sf::Time dTime = time.restart();
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::Resized) {
				a.setPosition(window.getSize().x / 2, window.getSize().y / 2);
			}
		}
		c.update(dTime);
		c.animate(a);
		window.clear(sf::Color(0, 100, 150, 200));
		window.draw(a);
		window.display();
	}
}