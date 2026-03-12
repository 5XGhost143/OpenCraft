#include "Audio.hpp"
#include <cmath>
#include <cstring>
#include <algorithm>

void Audio::init()
{
    SDL_AudioSpec want{};
    want.freq     = 44100;
    want.format   = AUDIO_S16SYS;
    want.channels = 1;
    want.samples  = 512;
    want.callback = audioCallback;
    want.userdata = this;
    dev = SDL_OpenAudioDevice(nullptr, 0, &want, &spec, 0);
    if (dev)
        SDL_PauseAudioDevice(dev, 0);
}

void Audio::playBreak()
{
    SDL_LockAudioDevice(dev);
    addTone(220.f, 0.06f, 0.04f);
    addTone(180.f, 0.05f, 0.03f);
    SDL_UnlockAudioDevice(dev);
}

void Audio::playPlace()
{
    SDL_LockAudioDevice(dev);
    addTone(330.f, 0.04f, 0.03f);
    addTone(440.f, 0.04f, 0.02f);
    SDL_UnlockAudioDevice(dev);
}

void Audio::destroy()
{
    if (dev)
    {
        SDL_CloseAudioDevice(dev);
        dev = 0;
    }
}

void Audio::addTone(float freq, float amp, float dur)
{
    tones.push_back({freq, amp, dur, 0});
}

void Audio::audioCallback(void* userdata, Uint8* stream, int len)
{
    auto*   a       = static_cast<Audio*>(userdata);
    int     samples = len / 2;
    Sint16* buf     = reinterpret_cast<Sint16*>(stream);
    std::memset(stream, 0, len);

    for (auto& t : a->tones)
    {
        int total = static_cast<int>(t.dur * a->spec.freq);
        for (int i = 0; i < samples && t.pos < total; ++i, ++t.pos)
        {
            float env    = 1.f - (float)t.pos / total;
            float sample = std::sin(2.f * 3.14159f * t.freq * t.pos / a->spec.freq) * t.amp * env * 32767.f;
            buf[i] = static_cast<Sint16>(std::max(-32768.f, std::min(32767.f, (float)buf[i] + sample)));
        }
    }

    a->tones.erase(
        std::remove_if(a->tones.begin(), a->tones.end(), [&](const Tone& t)
        {
            return t.pos >= (int)(t.dur * a->spec.freq);
        }),
        a->tones.end()
    );
}