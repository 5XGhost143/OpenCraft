#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Audio
{
public:
    void init();
    void playBreak();
    void playPlace();
    void destroy();

private:
    struct Tone
    {
        float freq;
        float amp;
        float dur;
        int   pos;
    };

    SDL_AudioDeviceID dev = 0;
    SDL_AudioSpec     spec{};
    std::vector<Tone> tones;

    void addTone(float freq, float amp, float dur);

    static void audioCallback(void* userdata, Uint8* stream, int len);
};