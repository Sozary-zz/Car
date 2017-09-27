#include "core.hpp"

int main()
{
	Core* app = new Core(1200, 650, "Cars");
	app->run();
	return 0;
}
