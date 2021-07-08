#include "pen.h"
#include "button.h"
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <sstream>
#include "../utils.h"
#include "textentry.h"
#include <algorithm>

#define WHITE 255, 255, 255, 255
#define BLACK 0, 0, 0, 255
#define SLIDER_CIRCLE_COLOR 158, 52, 235, 255
#define BG_COLOR 150, 150, 150, 255

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

    SDL_Color selected_color;

    int tx, ty;
    TTF_SizeText(sm_font, "a", &tx, &ty);
    
    int in_r = 0;
    int in_g = 0;
    int in_b = 0;

    auto destroy = [&]() { running = false;};

    auto select = [&]() {
        m_color = { 0, 0, 0 };
        running = false;
        utils::color_adjust(in_r, in_g, in_b);
        m_color = { (Uint8)in_r, (Uint8)in_g, (Uint8)in_b };
    };

    gui::Button cancel_button(" Cancel ", { 10, 270 }, destroy, sm_rend, sm_font);
    gui::Button save_button("  Save  ", { 225, 270 }, select, sm_rend, sm_font);

    std::vector <gui::Button> buttons = { cancel_button, save_button };

    SDL_Texture* cancel_tex = cancel_button.draw();
    SDL_Texture* save_tex = save_button.draw();

    SDL_Rect slider_r = {
        100,
        19,
        150,
        4
    };

    SDL_Rect slider_g = {
        100,
        109,
        150,
        4
    };

    SDL_Rect slider_b = {
        100,
        199,
        150,
        4
    };

    SDL_Rect current_color_indicator_outline = {
        124,
        229,
        52,
        52
    };

    SDL_Rect current_color_indicator = {
        125,
        230,
        50,
        50
    };


    float r, g, b;

    std::string str_r, str_g, str_b;

    utils::Circle* selected_circle{ nullptr };

    std::vector<utils::Circle> circles{ utils::Circle(5, 100, 21), utils::Circle(5, 100, 111), utils::Circle(5, 100, 201) };

    SDL_Event evt;
    bool mouse_left = false;

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
                if (evt.button.button == SDL_BUTTON_LEFT)
                {
                    for (auto& circle : circles)
                    {
                        if (utils::collides(x, y, { circle.xc() - circle.radius(), circle.yc() - circle.radius(), 2 * circle.radius(), 2 * circle.radius() }))
                        {
                            selected_circle = &circle;
                            mouse_left = true;
                            break;
                        }
                    }
                }
            }

            if (evt.type == SDL_MOUSEMOTION)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (mouse_left)
                {
                    selected_circle->move(x - selected_circle->xc(), 0);

                    if (selected_circle->xc() < slider_b.x)
                    {
                        selected_circle->move(slider_b.x - selected_circle->xc(), 0);
                    }

                    if (selected_circle->xc() > slider_b.x + slider_b.w)
                    {
                        selected_circle->move(slider_b.x + slider_b.w - selected_circle->xc(), 0);
                    }
                }

                for (auto& button : buttons)
                {
                    if (utils::collides(x, y, button.rect()))
                    {
                        button.m_color = { 201, 199, 199 };
                    }

                    else
                    {
                        button.m_color = { 255, 255, 255 };
                    }
                }
            }

            if (evt.type == SDL_MOUSEBUTTONUP)
            {
                if (evt.button.button == SDL_BUTTON_LEFT)
                {
                    mouse_left = false; 
                }
            }
        }
        

        SDL_RenderClear(sm_rend);
        SDL_SetRenderDrawColor(sm_rend, 255, 255, 255, 255);

        for (auto& button : buttons)
        {
            SDL_SetRenderDrawColor(sm_rend, button.m_color.r, button.m_color.g, button.m_color.b, 255);
            SDL_RenderFillRect(sm_rend, &button.rect());
        }

        SDL_RenderCopy(sm_rend, save_tex, nullptr, &save_button.rect());
        SDL_RenderCopy(sm_rend, cancel_tex, nullptr, &cancel_button.rect());

        

        utils::render_text(sm_rend, { 10, 10, tx * 4, ty }, "Red:", sm_font);
        utils::render_text(sm_rend, { 10, 100, tx * 6, ty }, "Green:", sm_font);
        utils::render_text(sm_rend, { 10, 190, tx * 5, ty }, "Blue:", sm_font);

        SDL_SetRenderDrawColor(sm_rend, 255, 255, 255, 255);
        
        SDL_RenderFillRect(sm_rend, &slider_r);
        SDL_RenderFillRect(sm_rend, &slider_g);
        SDL_RenderFillRect(sm_rend, &slider_b);

        r = ((float)(circles[0].xc() - slider_r.x) / (float)slider_r.w) * 255;
        g = ((float)(circles[1].xc() - slider_g.x) / (float)slider_g.w) * 255;
        b = ((float)(circles[2].xc() - slider_b.x) / (float)slider_b.w) * 255;

        str_r = std::to_string((int)std::floor(r));
        str_g = std::to_string((int)std::floor(g));
        str_b = std::to_string((int)std::floor(b));

        utils::render_text(sm_rend, { 260, 10, (int)(tx * str_r.length()), ty }, str_r.c_str(), sm_font);
        utils::render_text(sm_rend, { 260, 100, (int)(tx * str_g.length()), ty }, str_g.c_str(), sm_font);
        utils::render_text(sm_rend, { 260, 190, (int)(tx * str_b.length()), ty }, str_b.c_str(), sm_font);

        in_r = atoi(str_r.c_str());
        in_g = atoi(str_g.c_str());
        in_b = atoi(str_b.c_str());

        SDL_SetRenderDrawColor(sm_rend, SLIDER_CIRCLE_COLOR);

        for (auto& circle : circles)
        {
            circle.draw(sm_rend);
        }

        SDL_SetRenderDrawColor(sm_rend, in_r, in_g, in_b, 255);

        SDL_RenderFillRect(sm_rend, &current_color_indicator);

        SDL_SetRenderDrawColor(sm_rend, BLACK);

        SDL_RenderDrawRect(sm_rend, &current_color_indicator_outline);

        SDL_SetRenderDrawColor(sm_rend, BG_COLOR);
        SDL_RenderPresent(sm_rend);
    }
    
    SDL_DestroyRenderer(sm_rend);
    SDL_DestroyWindow(sm_window);
}

void gui::Pen::pen_select_thickness()
{
    bool running = true;
    bool mouse_left = false;
    std::string tt;
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

    auto destroy = [&]() { running = false; };
    auto select = [&]()
    {
        thickness = atoi(tt.c_str());
        utils::thickness_adjust(thickness);
        m_thickness = thickness;
        running = false;
    };

    gui::Button cancel_button(" Cancel ", { 10, 270 }, destroy, t_rend, t_font);
    gui::Button save_button("  Save  ", { 220, 270 }, select, t_rend, t_font);

    std::vector<gui::Button> buttons = { cancel_button, save_button };

    SDL_Texture* cancel_tex = cancel_button.draw();
    SDL_Texture* save_tex = save_button.draw();

    SDL_Rect slider_t = {
        100,
        109,
        150,
        4
    };

    utils::Circle* selected_circle{ nullptr };

    std::vector<utils::Circle> circles{ utils::Circle{ 5, 100, 111 } };

    float t;

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

                if (evt.button.button == SDL_BUTTON_LEFT)
                {
                    for (auto& circle : circles)
                    {
                        if (utils::collides(x, y, { circle.xc() - circle.radius(), circle.yc() - circle.radius(), 2 * circle.radius(), 2 * circle.radius() }))
                        {
                            selected_circle = &circle;
                            mouse_left = true;
                            break;
                        }
                    }
                }
            }

            if (evt.type == SDL_MOUSEMOTION)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (mouse_left)
                {
                    selected_circle->move(x - selected_circle->xc(), 0);

                    if (selected_circle->xc() < slider_t.x)
                    {
                        selected_circle->move(slider_t.x - selected_circle->xc(), 0);
                    }
                    if (selected_circle->xc() > slider_t.x + slider_t.w)
                    {
                        selected_circle->move(slider_t.x + slider_t.w - selected_circle->xc(), 0);
                    }
                }

                for (auto& button : buttons)
                {
                    if (utils::collides(x, y, button.rect()))
                    {
                        button.m_color = { 201, 199, 199 };
                    }

                    else
                    {
                        button.m_color = { 255, 255, 255 };
                    }
                }
            }

            if (evt.type == SDL_MOUSEBUTTONUP)
            {
                if (evt.button.button == SDL_BUTTON_LEFT)
                {
                    mouse_left = false;
                }
            }
        }

        utils::Circle* selected_circle{ nullptr };

        SDL_RenderClear(t_rend);
        SDL_SetRenderDrawColor(t_rend, 217, 217, 217, 255);

        for (auto& button : buttons)
        {
            SDL_SetRenderDrawColor(t_rend, button.m_color.r, button.m_color.g, button.m_color.b, 255);
            SDL_RenderFillRect(t_rend, &button.rect());
        }

        SDL_RenderCopy(t_rend, cancel_tex, nullptr, &cancel_button.rect());
        SDL_RenderCopy(t_rend, save_tex, nullptr, &save_button.rect());


        t = ((float)(circles[0].xc() - slider_t.x) / (float)slider_t.w) * 50;

        tt = std::to_string((int)std::floor(t));

        utils::render_text(t_rend, { 10, 100, tx * 10, ty }, "Thickness:", t_font);

        utils::render_text(t_rend, { 260, 100, tx * (int)tt.length(), ty }, tt.c_str(), t_font);

        SDL_SetRenderDrawColor(t_rend, WHITE);

        SDL_RenderFillRect(t_rend, &slider_t);

        SDL_SetRenderDrawColor(t_rend, SLIDER_CIRCLE_COLOR);

        for (auto& circle : circles)
        {
            circle.draw(t_rend);
        }

        SDL_SetRenderDrawColor(t_rend, BLACK);

        thickness = atoi(tt.c_str());

        if (thickness > 0)
            utils::draw_circle(t_rend, 150, 200, thickness);

        SDL_SetRenderDrawColor(t_rend, BG_COLOR);

        SDL_RenderPresent(t_rend);
    }

    SDL_DestroyRenderer(t_rend);
    SDL_DestroyWindow(t_window);
}
