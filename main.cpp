#include "Window.h"
#include <spdlog/spdlog.h>

int main(int argc, char* args[]) {
	Window _window;
	_window.run();
	spdlog::info("Window is closed. Why you close me?");
	return 0;
}