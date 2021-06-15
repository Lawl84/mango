#include "mango.h"
#include <SDL.h>
#include <iostream>
#include "utils.h"

#define WHITE 255, 255, 255, 255
#define BG_COLOR 255, 228, 156, 255

Mango::Mango()
    :m_window(nullptr), m_rend(nullptr)
{
    SDL_Init(SDL_INIT_EVERYTHING);

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
    SDL_Quit();
}

void Mango::mainloop()
{
    bool running = true;
    SDL_Event evt;
    int window_w, window_h;
    SDL_Color white{ WHITE };
    SDL_Color bg_color{ BG_COLOR };
    SDL_Rect canvas;

    while (running)
    {
        SDL_GetWindowSize(m_window, &window_w, &window_h);
        canvas = { (int)((0.1 * window_w) / 2), (int)((0.2 * window_h) / 2), (int)(0.9 * window_w), (int)(0.8 * window_h) }; //resizing canvas as the window gets resized

        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                running = false;
            }
        }
        SDL_RenderClear(m_rend);
        SDL_SetRenderDrawColor(m_rend, WHITE);
        SDL_RenderFillRect(m_rend, &canvas);
        SDL_SetRenderDrawColor(m_rend, BG_COLOR);
        SDL_RenderPresent(m_rend);
    }
}