#include "mango.h"
#include <SDL.h>
#include <iostream>
#include "gui/button.h"
#include <SDL_ttf.h>
#include "gui/canvas.h"
#include "gui/pen.h"
#include "utils.h"
#include <map>

#define WHITE 255, 255, 255, 255
#define BG_COLOR 6, 71, 69, 255

Mango::Mango()
    :m_window(nullptr), m_rend(nullptr)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    m_window = SDL_CreateWindow("Mango", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 800, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (!m_window)
    {
        std::cout << SDL_GetError() << "\n";
    }
    m_rend = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderClear(m_rend);
    SDL_RenderPresent(m_rend);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

Mango::~Mango()
{
    SDL_DestroyRenderer(m_rend);
    SDL_DestroyWindow(m_window);
    TTF_Quit();
    SDL_Quit();
}

void Mango::mainloop()
{
    int window_w, window_h;

    bool running = true;
    bool mouse_left = false;

    TTF_Font* main_font = TTF_OpenFont("Montserrat-SemiBold.ttf", 21);
    SDL_Event evt;

    SDL_Rect canvas_rect;
    gui::Canvas canvas(canvas_rect);
    SDL_Texture* canv_tex = SDL_CreateTexture(m_rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, canvas.rect().w, canvas.rect().h);

    SDL_Color user_main_color = { 0, 0, 0 };

    SDL_Color white{ 255, 255, 255 };

    SDL_Color bg_color{ BG_COLOR };

    

    gui::Pen p(m_rend, user_main_color);

    SDL_Rect b1 = {
        50,
        50,
        50,
        50
    };

    gui::Button button("Button", b1, [&]() {std::cout << "Lmao\n"; }, m_rend, main_font);

    texbuf = new uint32_t[canvas.rect().w * canvas.rect().h];

    while (running)
    {
        SDL_GetWindowSize(m_window, &window_w, &window_h);
        canvas.adjust_to_window(window_w, window_h);

        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (!button.handle_button_click(evt.button) && utils::collides(evt.button.x, evt.button.y, canvas.rect()))
                {
                    mouse_left = true;
                    int x = evt.button.x;
                    int y = evt.button.y;
                    texbuf[x + y * canvas.rect().w] = utils::hex(p.color());
                }
                    
                break;

            case SDL_MOUSEBUTTONUP:
                if (evt.button.button == SDL_BUTTON_LEFT)
                {
                    mouse_left = false;
                }
                break;
            case SDL_MOUSEMOTION:
                if (mouse_left && utils::collides(evt.button.x, evt.button.y, canvas.rect()))
                {
                    mouse_left = true;
                    int x = evt.button.x;
                    int y = evt.button.y;
                    texbuf[x + y * canvas.rect().w] = utils::hex(p.color());
                }
                break;
            }

        }
        SDL_RenderClear(m_rend);
        SDL_SetRenderDrawColor(m_rend, WHITE);
        SDL_RenderFillRect(m_rend, &canvas.rect());

        SDL_UpdateTexture(canv_tex, &canvas.rect(), texbuf, canvas.rect().w * sizeof(uint32_t));
        SDL_RenderCopy(m_rend, canv_tex, nullptr, &canvas.rect());

        button.draw();
        SDL_SetRenderDrawColor(m_rend, BG_COLOR);
        
        
        
        SDL_RenderPresent(m_rend);
    }
}
