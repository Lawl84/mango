#pragma once
#include <filesystem>
#include <SDL.h>
#include "button.h"
#include <SDL_image.h>

namespace fs = std::filesystem;

namespace gui
{
	class FileButton : public gui::Button
	{

	public:
		FileButton(const char* title, const SDL_Point& point, const std::function<void(fs::path, std::string)>& on_click, SDL_Renderer* rend, TTF_Font* font, SDL_Color rect_color = { 255, 255, 255 }, SDL_Color text_color = { 0, 0, 0 });
	};
}