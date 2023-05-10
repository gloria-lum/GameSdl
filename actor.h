#pragma once
#include <string>
#include <vector>
namespace game
{
	struct  position
	{
		float x;
		float y;
	};

	class Actor
	{


	public:
		int width;
		int height;
		std::string name;
		position& pos;
		Actor() = delete;
		Actor(const std::string& namepath, int width, int height);
		~Actor();

	private:

	};
}


