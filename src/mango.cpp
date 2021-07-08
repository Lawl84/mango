#include "mango.h"
#include <SDL.h>
#include <iostream>
#include "gui/button.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "gui/canvas.h"
#include "gui/pen.h"
#include "utils.h"
#include <map>
#include <thread>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


#define WHITE 255, 255, 255, 255
#define BG_COLOR_MAIN 120, 120, 120, 255
#define BLACK 0, 0, 0, 0

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

    SDL_Rect color_square = {
        70,
        734,
        49,
        49
    };

    SDL_Rect inner_color_square = {
        70,
        735,
        48,
        48
    };
    

    gui::Pen p(m_rend, user_main_color);
    
    SDL_Point select_color_point = {
        0,
        10,
    };

    SDL_Point select_thickness_point = {
        150,
        10
    };

    SDL_Point save_image_point = {
        300,
        10
    };
    
    gui::Button select_color(" Change Color ", select_color_point, [&]() { p.pen_select_color(); }, m_rend, main_font);
    gui::Button select_thickness(" Change Thickness ", select_thickness_point, [&]() { p.pen_select_thickness(); }, m_rend, main_font);
    gui::Button save_image_button(" Save Image ", save_image_point, [&]() { save_image(canvas.rect().w, canvas.rect().h); }, m_rend, main_font);

    std::vector<gui::Button> buttons;
    buttons.emplace_back(select_color);
    buttons.emplace_back(select_thickness);
    buttons.emplace_back(save_image_button);

    SDL_Texture* select_colorTex = select_color.draw();
    SDL_Texture* select_thickTex = select_thickness.draw();
    SDL_Texture* save_imageTex = save_image_button.draw();

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

                    utils::draw_with_thickness(texbuf, p, canvas, x, y, p.thickness());
                }
            } break;

            case SDL_MOUSEBUTTONUP:
                if (evt.button.button == SDL_BUTTON_LEFT)
                    mouse_left = false; break;
            case SDL_KEYDOWN:
            {
                switch (evt.key.keysym.sym)
                {
                case SDLK_z:
                    save_image(canvas.rect().w, canvas.rect().h);
                    break;
                }
            } break;
            
            case SDL_MOUSEMOTION:
                
                int mx, my;
                SDL_GetMouseState(&mx, &my);
                if (mouse_left && utils::collides(mx, my, canvas.rect()))
                {
                    int x = mx - canvas.rect().x;
                    int y = my - canvas.rect().y;

                    utils::draw_with_thickness(texbuf, p, canvas, x, y, p.thickness());
                }

                for (auto& button : buttons)
                {
                    if (utils::collides(mx, my, button.rect()))
                    {
                        button.m_color = { 201, 199, 199 };
                    }
                    
                    else
                    {
                        button.m_color = { 255, 255, 255 };
                    }
                }
            }
        } 

        SDL_RenderClear(m_rend);
        SDL_AddEventWatch(resizing_event_watcher, m_window);

        SDL_SetRenderDrawColor(m_rend, canvas.color().r, canvas.color().g, canvas.color().b, canvas.color().a);

        SDL_RenderFillRect(m_rend, &canvas.rect());
         
        SDL_UpdateTexture(canv_tex, 0, texbuf, canvas.rect().w * sizeof(uint32_t));
        SDL_RenderCopy(m_rend, canv_tex, nullptr, &canvas.rect());

        for (auto& button : buttons)
        {
            SDL_SetRenderDrawColor(m_rend, button.m_color.r, button.m_color.g, button.m_color.b, 255);
            SDL_RenderFillRect(m_rend, &button.rect());
        }

        SDL_RenderCopy(m_rend, select_colorTex, nullptr, &select_color.rect());
        SDL_RenderCopy(m_rend, select_thickTex, nullptr, &select_thickness.rect());
        SDL_RenderCopy(m_rend, save_imageTex, nullptr, &save_image_button.rect());

        utils::label(m_rend, "Color: ", main_font, { 10, 750 });

        SDL_SetRenderDrawColor(m_rend, BLACK);
        SDL_RenderDrawRect(m_rend, &color_square);

        SDL_SetRenderDrawColor(m_rend, p.color().r, p.color().g, p.color().b, 255);
        SDL_RenderFillRect(m_rend, &inner_color_square);
        SDL_SetRenderDrawColor(m_rend, BG_COLOR_MAIN);
        
        SDL_RenderPresent(m_rend);
    }

    SDL_DestroyTexture(select_colorTex);
    SDL_DestroyTexture(select_thickTex);

    SDL_Quit();
}


void Mango::save_image(int w, int h)
{
    auto hex_to_rgb = [&](uint32_t hex) {
        SDL_Color color{ 0, 0, 0 };
        color.r = (hex >> 16) & 0xFF;
        color.g = (hex >> 8) & 0xFF;
        color.b = hex & 0xFF;

        return color;
    };

    cv::Mat image(h, w, CV_8UC4);

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            cv::Vec4b& bgra = image.at<cv::Vec4b>(i, j);
            SDL_Color col = hex_to_rgb(texbuf[i * w + j]);

            bgra[0] = col.b;
            bgra[1] = col.g;
            bgra[2] = col.r;
            bgra[3] = UCHAR_MAX;
        }
    }

    cv::imwrite("out.png", image);
}

