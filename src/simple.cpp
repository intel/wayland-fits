#include <iostream>
#include <cstdlib>

#include "display.h"
#include "window.h"

int main(const int argc, const char** argv)
{
	wayland::Display display;
	wayland::Window window(display, 200, 200);

	display.run();

	return 0;
}
