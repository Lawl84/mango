#pragma once
#include <SDL.h>
#include <thread>
#include "gui/canvas.h"

class Mango
{
    SDL_Window* m_window;
    SDL_Renderer* m_rend;
    uint32_t* texbuf;
public:
    Mango();
    ~Mango();

    void mainloop();
    std::string save_image_dialogue(int w, int h);
};
