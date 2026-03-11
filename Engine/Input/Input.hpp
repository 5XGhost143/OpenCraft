#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>

class Input {
public:
    void beginFrame() {
        prevKeys = curKeys;
        mouseRelX = mouseRelY = 0;
        leftClicked = rightClicked = false;
    }

    void processEvent(const SDL_Event& e) {
        if (e.type == SDL_KEYDOWN) curKeys[e.key.keysym.sym] = true;
        if (e.type == SDL_KEYUP)   curKeys[e.key.keysym.sym] = false;
        if (e.type == SDL_MOUSEMOTION) {
            mouseRelX += e.motion.xrel;
            mouseRelY += e.motion.yrel;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT)  leftClicked  = true;
            if (e.button.button == SDL_BUTTON_RIGHT) rightClicked = true;
        }
    }

    bool held(SDL_Keycode k)    const { auto it=curKeys.find(k);  return it!=curKeys.end()&&it->second; }
    bool pressed(SDL_Keycode k) const {
        auto c=curKeys.find(k); auto p=prevKeys.find(k);
        bool cn = c!=curKeys.end()&&c->second;
        bool pn = p!=prevKeys.end()&&p->second;
        return cn && !pn;
    }

    int mouseRelX = 0, mouseRelY = 0;
    bool leftClicked = false, rightClicked = false;

private:
    std::unordered_map<SDL_Keycode,bool> curKeys, prevKeys;
};