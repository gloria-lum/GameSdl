#include "window.h"
#include <iostream>
#include <vector>
#include "player.h"


int main(int argc, char** argv)
{
	game::Player player("pippo", 10, 10);
	game::Enemy player2("pippo2", 20, 20);
	game::Window window("C++ addicted", player, player2, "pinguin.png", 512, 512);

	window.Run();
	return 0;
}