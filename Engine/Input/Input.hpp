#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>

class Input
{
public:
    int  mouseRelX   = 0;
    int  mouseRelY   = 0;
    bool leftClicked  = false;
    bool rightClicked = false;

    void beginFrame();
    void processEvent(const SDL_Event& e);
    bool held(SDL_Keycode k) const;
    bool pressed(SDL_Keycode k) const;

private:
    std::unordered_map<SDL_Keycode, bool> curKeys;
    std::unordered_map<SDL_Keycode, bool> prevKeys;
};