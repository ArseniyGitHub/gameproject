#include "Game.h"
#include <spdlog/spdlog.h>

int main(int argc, char* args[]) {
	Game _window;
	_window.start();
	spdlog::info("Window is closed. Why you close me?");
	return 0;
}