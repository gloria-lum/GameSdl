#pragma once
#include <string>
#include <vector>
#include "actor.h"

namespace game
{
    class Enemy : public Actor
    {
    public:
       Enemy() = delete;
       Enemy(const std::string& namepath, int width, int height);
        ~Enemy();
        Enemy(const Enemy&) = delete;
        Enemy& operator=(const Enemy&) = delete;
     
      
    private:




    };
}

