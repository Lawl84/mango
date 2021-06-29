#include "mango.h"
#include <SDL.h>
#include <iostream>
#include "gui/button.h"
#include <SDL_ttf.h>
#include "gui/canvas.h"
#include "gui/pen.h"
#include "utils.h"
#include <map>
#include <thread>

#define WHITE 255, 255, 255, 255
#define BG_COLOR 6, 71, 69, 255

Mango::Mango()
    :m_window(nullptr), m_rend(nullptr)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    m_window = SDL_CreateWindow("Mango", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 800, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if (!m_window)
    {
        std::cout << SDL_GetError() << "\n";
    }
    m_rend = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderClear(m_rend);
    SDL_RenderPresent(m_rend);
}

int resizing_event_watcher(void* data, SDL_Event* event) {
    if (event->type == SDL_WINDOWEVENT &&
        event->window.event == SDL_WINDOWEVENT_RESIZED) {
        SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
    }
    return 0;
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

    TTF_Font* main_font = TTF_OpenFont("res/CascadiaCode-Regular-VTT.ttf", 14);
    SDL_Event evt;

    SDL_GetWindowSize(m_window, &window_w, &window_h);
    
    gui::Canvas canvas({ (int)((0.1 * window_w) / 2), (int)((0.2 * window_h) / 2), (int)(0.9 * window_w), (int)(0.8 * window_h) }, {255, 255, 255});

    canvas.center(window_w, window_h);
    SDL_Texture* canv_tex = SDL_CreateTexture(m_rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, canvas.rect().w, canvas.rect().h);

    SDL_Color user_main_color = { 0, 0, 0 };
    SDL_Color colorbuf;

    SDL_Color white{ 255, 255, 255 };

    SDL_Color bg_color{ BG_COLOR };

    
    

    gui::Pen p(m_rend, user_main_color);


    int bx, by;
    TTF_SizeText(main_font, " Select Color ", &bx, &by);
    
    SDL_Point b1 = {
        0,
        10,
    };


    
    gui::Button select_color(" Select Color ", b1, [&]() { p.pen_select_color(); }, m_rend, main_font);

    std::vector<gui::Button> buttons;
    buttons.emplace_back(select_color);

    texbuf = new uint32_t[canvas.rect().w * canvas.rect().h];

    while (running)
    {
        SDL_GetWindowSize(m_window, &window_w, &window_h);
        canvas.center(window_w, window_h);
        

        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
            {
                int mx, my;
                SDL_GetMouseState(&mx, &my);
                bool button_click = false;

                for (gui::Button& b : buttons)
                {
                    if (b.check_click(mx, my))
                    {
                        button_click = true;
                        b.handle_button_click(evt.button);
                        break;
                    }
                        
                }

                if (utils::collides(mx, my, canvas.rect()) && !button_click)
                {
                    mouse_left = true;
                    int x = mx - canvas.rect().x;
                    int y = my - canvas.rect().y;

                    texbuf[y * canvas.rect().w + x] = utils::hex(p.color());
                }
            } break;

            case SDL_MOUSEBUTTONUP:
                if (evt.button.button == SDL_BUTTON_LEFT)
                    mouse_left = false; break;
            
            case SDL_MOUSEMOTION:
                
                int mx, my;
                SDL_GetMouseState(&mx, &my);
                if (mouse_left && utils::collides(mx, my, canvas.rect()))
                {
                    int x = mx - canvas.rect().x;
                    int y = my - canvas.rect().y;

                    texbuf[y * canvas.rect().w + x] = utils::hex(p.color());
                }
            }
        } 

        SDL_RenderClear(m_rend);
        SDL_AddEventWatch(resizing_event_watcher, m_window);

        SDL_SetRenderDrawColor(m_rend, canvas.color().r, canvas.color().g, canvas.color().b, canvas.color().a);

        SDL_RenderFillRect(m_rend, &canvas.rect());
         
        SDL_UpdateTexture(canv_tex, 0, texbuf, canvas.rect().w * sizeof(uint32_t));
        SDL_RenderCopy(m_rend, canv_tex, nullptr, &canvas.rect());

        utils::draw_buttons(buttons);
        SDL_SetRenderDrawColor(m_rend, BG_COLOR);
        
        SDL_RenderPresent(m_rend);
    }
    SDL_Quit();
}
