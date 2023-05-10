#include "window.h"
#include "SDL.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <chrono>

#include <cstring>

#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>

#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#endif


game::Window::Window(const std::string& title, Player& player, Enemy& enemy1, const std::string& path_image, const int width, const int height) : my_player(player), enemy(enemy1)
{

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		failed = true;
		return;
	}

	window = SDL_CreateWindow(title.c_str(), 100, 100, width, height, 0);
	if (!window)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		failed = true;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		failed = true;
		return;
	}

	//load image
	int channels_player;
	int channels_enemy;

	const char* conv_path_image = path_image.c_str();
	const char* conv_path_image1 = path_image.c_str();

	unsigned char* load_player_pixels = stbi_load(conv_path_image, &my_player.width, &my_player.height, &channels_player, 4);
	unsigned char* load_enemy_pixels = stbi_load(conv_path_image1, &enemy.width, &enemy.height, &channels_enemy, 4);

	this->texture1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, my_player.width, my_player.height);
	this->texture2 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, enemy.width, enemy.height);

	if (!texture1 || !texture2)
	{
		SDL_Log("Unable to create texture: %s", SDL_GetError());
		free(load_player_pixels);
		free(load_enemy_pixels);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return;
	}

	SDL_UpdateTexture(this->texture1, NULL, load_player_pixels, my_player.width * 4);
	SDL_UpdateTexture(this->texture2, NULL, load_enemy_pixels, enemy.width * 4);
	SDL_SetTextureAlphaMod(this->texture1, 255);
	SDL_SetTextureBlendMode(this->texture1, SDL_BLENDMODE_BLEND);
	free(load_player_pixels);
	free(load_enemy_pixels);

}

bool game::Window::IsFailed() const
{
	return failed;
}


int32_t game::Window::buffer_to_int32(const char* buffer)
{
	const char* bytes = buffer;
	int32_t value = ntohl(*reinterpret_cast<const int32_t*>(bytes));
	return value;
}

void game::Window::Run()//(std::function<void(const float)> tick) const
{
	bool running = true;

	while (running)
	{
		StartFrame(running);
#ifdef _WIN32

		WSADATA wsa_data;
		if (WSAStartup(0x0202, &wsa_data))
		{
			printf("unable to initialize winsock2 \n");
			return;
		}
#endif
		//for WASD
	   // const Uint8* keys = SDL_GetKeyboardState(NULL);

		//socket
		int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (s < 0) {
			std::cerr << "unable to initialize the UDP socket\n";
			return;
		}

		struct sockaddr_in serverAddress;
		inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr); // this will create a big endian 32 bit address
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(9999);

		if (bind(s, (struct sockaddr*)&serverAddress, sizeof(serverAddress)))
		{
			std::cout << "unable to bind the UDP socket \n" << std::endl;

			return;
		}

		//for move deltatime 
	   // auto prev_time = std::chrono::high_resolution_clock::now();  
		for (;;)
		{
			char buffer[4096];
			struct sockaddr_in sender_in;


			int sender_in_size = sizeof(sender_in);
			int len = recvfrom(s, buffer, 4096, 0, (struct sockaddr*)&sender_in, &sender_in_size);
			if (len > 0)
			{
				char addr_as_string[64];

				inet_ntop(AF_INET, &sender_in.sin_addr, addr_as_string, 64);
				std::cout << ("received %d bytes from %s:%d\n", len, addr_as_string, ntohs(sender_in.sin_port)) << std::endl;


			}

			int data = std::abs(buffer_to_int32(buffer));

			my_player.pos.x = data;
			enemy.pos.y = data;

			EndFrame();


			//move player deltatime

			//float velocity = 20.0f;
		/*auto current_time = std::chrono::high_resolution_clock::now(); //12.01
		std::chrono::duration<float> delta_time = current_time - prev_time;
		prev_time = current_time;
		// my_player.pos.x += velocity * delta_time.count();
		enemy.pos.x += velocity * delta_time.count();
		std::cout << ("i player", my_player.pos.x, "-----", enemy.pos.x) << std::endl;*/



		}

			//movement for WASD

		/* player.pos.x += keys[SDL_SCANCODE_RIGHT];
		 player.pos.x -= keys[SDL_SCANCODE_LEFT];
		  player.pos.y += keys[SDL_SCANCODE_DOWN];
		  player.pos.y -= keys[SDL_SCANCODE_UP];
		  tick(1.f / 60.f);*/

	}
}

void game::Window::StartFrame(bool running) const
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

		if (event.type == SDL_QUIT)
		{
			running = false;
		}

	}
}

void game::Window::EndFrame() const
{

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_Rect target_rect = { my_player.pos.x, my_player.pos.y,32,  32 };
	SDL_Rect target_rect2 = { enemy.pos.x, enemy.pos.y,32,  32 };
	SDL_RenderCopy(renderer, this->texture1, NULL, &target_rect);
	SDL_RenderCopy(renderer, this->texture2, NULL, &target_rect2);
	SDL_RenderPresent(renderer);

}

game::Window::~Window()
{
	if (renderer)
	{
		SDL_DestroyRenderer(renderer);
	}
	if (window)
	{
		SDL_DestroyWindow(window);
	}
	if (SDL_WasInit(0) != 0)
	{
		SDL_Quit();
	}
}