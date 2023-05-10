#include "enemy.h"
#include <string>
#include <iostream>
#include "stb_image.h"
#include <chrono>

namespace game
{
	Enemy::Enemy(const std::string& namepath, int width, int height)
		: Actor(namepath, width, height)
	{
		this->name = namepath;
		pos.x = 10;
		pos.y = 10;
	}

	Enemy::~Enemy()
	{

	}
}