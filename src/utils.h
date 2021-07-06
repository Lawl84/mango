#pragma once
#include <SDL.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include "gui/button.h"
#include "gui/canvas.h"
#include "gui/pen.h"
#include <map>
#include <SDL_ttf.h>
#include <iostream>
#include <algorithm>

#define HEXPEN hex(pen.color())

namespace utils
{
    struct CirclePoint
    {
        int xval;
        int yval;
        int arrayval;
    };

    struct Circle
    {
        int radius;
        int xc;
        int yc;
        

    private:
        void drawCircle(std::map<int, std::vector<int>>& circ_points, SDL_Renderer* rend, int xc, int yc, int x, int y)
        {
            SDL_RenderDrawPoint(rend, xc + x, yc + y);
            circ_points[yc + y].emplace_back(xc + x);
            SDL_RenderDrawPoint(rend, xc - x, yc + y);
            circ_points[yc + y].emplace_back(xc - x);
            SDL_RenderDrawPoint(rend, xc + x, yc - y);
            circ_points[yc - y].emplace_back(xc + x);
            SDL_RenderDrawPoint(rend, xc - x, yc - y);
            circ_points[yc - y].emplace_back(xc - x);
            SDL_RenderDrawPoint(rend, xc + y, yc + x);
            circ_points[yc + x].emplace_back(xc + y);
            SDL_RenderDrawPoint(rend, xc - y, yc + x);
            circ_points[yc + x].emplace_back(xc - y);
            SDL_RenderDrawPoint(rend, xc + y, yc - x);
            circ_points[yc - x].emplace_back(xc + y);
            SDL_RenderDrawPoint(rend, xc - y, yc - x);
            circ_points[yc - x].emplace_back(xc - y);
        }
    public:
        void draw(SDL_Renderer* rend)
        {
            std::map<int, std::vector<int>> circ_points;
            int x = 0, y = this->radius;
            int d = 3 - 2 * this->radius;
            drawCircle(circ_points, rend, this->xc, this->yc, x, y);
            while (y >= x)
            {
                x++;
                if (d > 0)
                {
                    y--;
                    d = d + 4 * (x - y) + 10;
                }
                else
                    d = d + 4 * x + 6;

                drawCircle(circ_points, rend, this->xc, this->yc, x, y);
            }
            for (auto& [y, values] : circ_points)
            {
                int max = *std::max_element(values.begin(), values.end());
                int min = *std::min_element(values.begin(), values.end());

                for (int i = min; i < max; i++)
                {
                    SDL_RenderDrawPoint(rend, i, y); 
                }
                    
            }
        }

    };

    inline bool collides(int mx, int my, const SDL_Rect& rect)
    {
        return (mx >= rect.x && mx <= rect.x + rect.w) && (my >= rect.y && my <= rect.y + rect.h);
    }

    inline uint32_t hex(SDL_Color color)
    {
        return ((color.r & 0xff) << 16) + ((color.g & 0xff) << 8) + (color.b & 0xff);
    }

    inline void draw_buttons(std::vector<gui::Button>& buttons)
    {
        for (gui::Button& button : buttons)
        {
            button.draw();
        }
    }

    inline void render_text(SDL_Renderer* rend, const SDL_Rect& rect, const char* text, TTF_Font* font)
    {
        SDL_Surface* surface = TTF_RenderText_Blended(font, text, SDL_Color{ 0, 0, 0 });

        SDL_Texture* texture{ 0 };

        if (surface)
            texture = SDL_CreateTextureFromSurface(rend, surface);

        if (texture)
            SDL_RenderCopy(rend, texture, nullptr, &rect);

        if (surface)
            SDL_FreeSurface(surface);

        if (texture)
            SDL_DestroyTexture(texture);
    }

    inline void color_adjust(int& r, int& g, int& b)
    {
        if (r > 255) r = 255;
        if (r < 0) r = 0;

        if (g > 255) g = 255;
        if (g < 0) g = 0;

        if (b > 255) b = 255;
        if (b < 0) b = 0;

    }

    inline void label(SDL_Renderer* rend, const char* text, TTF_Font* font, const SDL_Point& point)
    {
        int x, y;
        TTF_SizeText(font, text, &x, &y);

        SDL_Surface* surface = TTF_RenderText_Blended(font, text, SDL_Color{ 255, 255, 255 });
        SDL_Texture* texture{ 0 };
        SDL_Rect rect = {
            point.x,
            point.y,
            x,
            y
        };

        if (surface)
            texture = SDL_CreateTextureFromSurface(rend, surface);

        if (texture)
            SDL_RenderCopy(rend, texture, nullptr, &rect);

        if (surface)
            SDL_FreeSurface(surface);

        if (texture)
            SDL_DestroyTexture(texture);

    }

    inline void thickness_adjust(int& thickness)
    {
        if (thickness > 100) thickness = 100;
        if (thickness < 1) thickness = 1;
    }

    inline bool valid_point(gui::Canvas& canvas, CirclePoint c)
    {
        return c.arrayval >= 0 && c.arrayval < canvas.rect().w * canvas.rect().h && c.xval < canvas.rect().w && c.xval >= 0;
    }

    inline void draw_with_thickness(uint32_t* arr, gui::Pen& pen, gui::Canvas& canvas, int xc, int yc, int r)
    {
        std::map<int, std::vector<int>> circ_points;

        int wid = canvas.rect().w;

        auto draw_circle = [&](int xc, int yc, int x, int y, int r)
        {
            CirclePoint c1, c2, c3, c4, c5, c6, c7, c8;
            c1 = { xc + x, yc + y, xc + x + (yc + y) * wid };
            c2 = { xc - x, yc + y, xc - x + (yc + y) * wid };
            c3 = { xc + x, yc - y, xc + x + (yc - y) * wid };
            c4 = { xc - x, yc - y, xc - x + (yc - y) * wid };
            c5 = { xc + y, yc + x, xc + y + (yc + x) * wid };
            c6 = { xc - y, yc + x, xc - y + (yc + x) * wid };
            c7 = { xc + y, yc - x, xc + y + (yc - x) * wid };
            c8 = { xc - y, yc - x, xc - y + (yc - x) * wid };

            std::vector<CirclePoint> circles = { c1, c2, c3, c4, c5, c6, c7, c8 };

            for (CirclePoint c : circles)
            {
                if (valid_point(canvas, c))
                {
                    arr[c.arrayval] = HEXPEN;
                    circ_points[c.yval].emplace_back(c.xval);
                }
                else if (c.xval > wid)
                    circ_points[c.yval].emplace_back(wid);
                else if (c.xval <= 0)
                    circ_points[c.yval].emplace_back(0);
            }

        };

        int x = 0, y = r, d = 3 - (2 * r);
        draw_circle(xc, yc, x, y, r);

        while (y >= x)
        {
            ++x;
            if (d > 0)
            {
                y--;
                d = d + 4 * (x - y);
            }

            else
                d = d + 4 * x + 6;

            draw_circle(xc, yc, x, y, r);

        }

        for (auto& [y, values] : circ_points)
        {
            int max = *std::max_element(values.begin(), values.end());
            int min = *std::min_element(values.begin(), values.end());

            for (int i = min; i < max; i++)
            {
                if (valid_point(canvas, CirclePoint{i, NULL, i + y * canvas.rect().w}))
                    arr[i + y * wid] = HEXPEN;
            }
        }
    }

}
