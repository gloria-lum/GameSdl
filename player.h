#pragma once
#include <string>
#include <vector>
#include "actor.h"

namespace game
{
	class Player : public Actor
	{
	public:
		Player() = delete;
		Player(const std::string& namepath, int width, int height);
		~Player();
		Player(const Player&) = delete;
		Player& operator=(const Player&) = delete;

	private:

	};
}
