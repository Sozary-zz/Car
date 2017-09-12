#include "core.hpp"

int main()
{
	Core* app = new Core(800, 600, "Cars");
	app->run();
	return 0;
}
