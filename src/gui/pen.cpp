#include "pen.h"
#include "button.h"
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <sstream>
#include "../utils.h"
#include "textentry.h"

#define WHITE 255, 255, 255, 255

void gui::Pen::draw(SDL_Point p0)
{
	SDL_RenderDrawPoint(m_rend, p0.x, p0.y);
}

void gui::Pen::pen_select_color()
{
	bool running = true;
	std::stringstream window_title_ss;
	window_title_ss << "Current RGB: (" << (int)m_color.r << ", " << (int)m_color.g << ", " << (int)m_color.b << ")";
	SDL_Window* sm_window = SDL_CreateWindow(window_title_ss.str().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 300, 300, SDL_WINDOW_SHOWN);
	SDL_Renderer* sm_rend = SDL_CreateRenderer(sm_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	TTF_Font* sm_font = TTF_OpenFont("res/CascadiaCode-Regular-VTT.ttf", 14);
	SDL_SetWindowGrab(sm_window, SDL_TRUE);

	std::vector<gui::TextEntry> textentries;

	SDL_Color selected_color;
	gui::TextEntry* selected_entry = nullptr;

	int tx, ty;
	TTF_SizeText(sm_font, "a", &tx, &ty);
	
	int r, g, b;

	auto destroy = [&]() { running = false;};

	auto select = [&]() {
		m_color = { 0, 0, 0 };
		running = false;
		r = atoi(textentries[0].m_text.c_str());
		g = atoi(textentries[1].m_text.c_str());
		b = atoi(textentries[2].m_text.c_str());
		utils::color_adjust(r, g, b);
		m_color = { (Uint8)r, (Uint8)g, (Uint8)b };
	};

	gui::Button cancel_button(" Cancel ", { 10, 270 }, destroy, sm_rend, sm_font);

	gui::Button save_button("  Save  ", { 220, 270 }, select, sm_rend, sm_font);

	gui::TextEntry tr(sm_rend, { 60, 10, tx * 3, ty}, sm_font);
	gui::TextEntry tg(sm_rend, { 60, 100, tx * 3, ty }, sm_font);
	gui::TextEntry tb(sm_rend, { 60, 190, tx * 3, ty }, sm_font);

	textentries.emplace_back(tr);
	textentries.emplace_back(tg);
	textentries.emplace_back(tb);

	SDL_Event evt;

	while (running)
	{
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_MOUSEBUTTONDOWN)
			{
				cancel_button.handle_button_click(evt.button);
				save_button.handle_button_click(evt.button);
				int x, y;
				SDL_GetMouseState(&x, &y);
				for (gui::TextEntry& txt : textentries)
				{
					if (utils::collides(x, y, txt.rect()))
					{
						selected_entry = &txt;
						break;
					}
				}
			}

			if (evt.type == SDL_TEXTINPUT)
			{
				if (selected_entry)
				{
					int i = atoi(evt.text.text);
					if (selected_entry->m_text.length() < 3 && i || selected_entry->m_text.length() < 3 && evt.text.text[0] == '0')
						selected_entry->m_text += evt.text.text;

				}

			}

			if (evt.type == SDL_KEYDOWN)
			{
				switch (evt.key.keysym.sym)
				{
					case SDLK_BACKSPACE:
						selected_entry->m_text = selected_entry->m_text.substr(0, selected_entry->m_text.length() - 1);
				}
			}

		}
		

		SDL_RenderClear(sm_rend);
		SDL_SetRenderDrawColor(sm_rend, 217, 217, 217, 255);
		cancel_button.draw();
		save_button.draw();
		for (gui::TextEntry& txt : textentries)
		{
			txt.draw();
			if (selected_entry)
				txt.render(txt.m_text.c_str());
		}

		utils::render_text(sm_rend, { 10, 10, tx * 4, ty }, "Red:", sm_font);
		utils::render_text(sm_rend, { 10, 100, tx * 6, ty }, "Green:", sm_font);
		utils::render_text(sm_rend, { 10, 190, tx * 5, ty }, "Blue:", sm_font);

		SDL_SetRenderDrawColor(sm_rend, WHITE);
		SDL_RenderPresent(sm_rend);
	}
	
	SDL_DestroyRenderer(sm_rend);
	SDL_DestroyWindow(sm_window);

}

void gui::Pen::pen_select_thickness()
{
	bool running = true;
	std::stringstream window_title_ss;
	window_title_ss << "Current Thickness: " << m_thickness << " px";
	SDL_Window* t_window = SDL_CreateWindow(window_title_ss.str().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 300, 300, SDL_WINDOW_SHOWN);
	SDL_Renderer* t_rend = SDL_CreateRenderer(t_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	TTF_Font* t_font = TTF_OpenFont("res/CascadiaCode-Regular-VTT.ttf", 14);

	int thickness;
	int tx, ty;

	TTF_SizeText(t_font, "T", &tx, &ty);

	SDL_SetWindowGrab(t_window, SDL_TRUE);

	SDL_Event evt;

	gui::TextEntry thickness_entry(t_rend, { 95, 100, tx * 3, ty }, t_font);
	gui::TextEntry* selected_entry = nullptr;

	std::vector<gui::TextEntry> textentries;

	auto destroy = [&]() { running = false; };
	auto select = [&]()
	{
		thickness = atoi(textentries[0].m_text.c_str());
		utils::thickness_adjust(thickness);
		m_thickness = thickness;
		running = false;
	};
	gui::Button cancel_button(" Cancel ", { 10, 270 }, destroy, t_rend, t_font);
	gui::Button save_button("  Save  ", { 220, 270 }, select, t_rend, t_font);

	textentries.emplace_back(thickness_entry);
	while (running)
	{
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);

				cancel_button.handle_button_click(evt.button);
				save_button.handle_button_click(evt.button);

				for (gui::TextEntry& t : textentries)
				{
					if (utils::collides(x, y, t.rect()))
					{
						selected_entry = &t;
						break;
					}
				}
			}

			if (evt.type == SDL_TEXTINPUT)
			{
				if (selected_entry)
				{
					int i = atoi(evt.text.text);
					if (selected_entry->m_text.length() < 3 && i || selected_entry->m_text.length() < 3 && evt.text.text[0] == '0')
						selected_entry->m_text += evt.text.text;
				}
			}

			if (evt.type == SDL_KEYDOWN)
			{
				switch (evt.key.keysym.sym)
				{
				case SDLK_BACKSPACE:
					selected_entry->m_text = selected_entry->m_text.substr(0, selected_entry->m_text.length() - 1);
				}
			}
		}
		SDL_RenderClear(t_rend);
		SDL_SetRenderDrawColor(t_rend, 217, 217, 217, 255);

		cancel_button.draw();
		save_button.draw();
		thickness_entry.draw();

		utils::render_text(t_rend, { 10, 100, tx * 10, ty }, "Thickness:", t_font);

		for (gui::TextEntry& txt : textentries)
		{
			txt.draw();
			if (selected_entry)
				txt.render(txt.m_text.c_str());
		}

		

		SDL_SetRenderDrawColor(t_rend, WHITE);

		SDL_RenderPresent(t_rend);
	}

	SDL_DestroyRenderer(t_rend);
	SDL_DestroyWindow(t_window);
}
