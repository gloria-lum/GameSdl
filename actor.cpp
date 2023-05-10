#include "actor.h"
#include <string>
#include <iostream>
#include "stb_image.h"
#include <chrono>

game::Actor::Actor(const std::string& path_image, int width, int height) : pos(*new position{ 10, 10 }), width(width), height(height)
{

}

game::Actor::~Actor()
{

}
