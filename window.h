#pragma once
#include "tickable.h"
#include <memory>
#include <string>
#include <functional>
#include "player.h"
#include "enemy.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace game
{
	class Window
	{
	protected:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		SDL_Texture* texture1 = nullptr;
		SDL_Texture* texture2 = nullptr;
		bool failed = false;
		void StartFrame(bool running) const;
		void EndFrame() const;
		Player& my_player;
		Enemy& enemy;

	public:
		Window() = delete;
		Window(const std::string& title, Player& player, Enemy& enemy, const std::string& path_image, const int width, const int height);//const std::string& playerpath
		Window(const Window& other) = delete;
		Window& operator=(const Window& other) = delete;

		~Window();
		bool IsFailed() const;
		void Run();
		int32_t buffer_to_int32(const char* buffer);

	};
}