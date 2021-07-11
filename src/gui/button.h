#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <functional>

namespace gui
{
    class Button
    {
        std::function<void(void)> m_func;
        SDL_Point m_point;
        SDL_Rect m_rect;
        SDL_Renderer* m_rend;
        const char* m_title;
        TTF_Font* m_font;
        SDL_Color text_color;
        SDL_Texture* m_tex;

    public:
        SDL_Color m_color;
        Button(const char* title, const SDL_Point& point, const std::function<void(void)>& on_click, SDL_Renderer* rend, TTF_Font* font, SDL_Color rect_color = { 255, 255, 255 }, SDL_Color text_color = { 0, 0, 0 });
        bool check_click(int mx, int my);
        bool handle_button_click(const SDL_MouseButtonEvent& btn);
        void draw();
        void call() { m_func(); }
        SDL_Rect& rect() { return m_rect; }
        SDL_Texture* tex() { return m_tex; }
        ~Button();
    };

};
