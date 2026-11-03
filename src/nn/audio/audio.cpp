#include "nn/audio.h"
#include <SDL.h>
#include <cstring>
#include <iostream>

namespace nn::audio {

//Appelee par SDL dans son propre thread quand la carte son a besoin de "length" octets
static void AudioCallback(void* userData, Uint8* stream, int length)
{
    AudioOut* audioOut = (AudioOut*)userData;
    bool somethingReleased = false;
    {
        std::lock_guard<std::mutex> guard(audioOut->lock);
        int written = 0;
        while (written < length && !audioOut->playing.empty())
        {
            AudioOutBuffer* buffer = audioOut->playing.front();
            size_t left = buffer->dataSize - buffer->played;
            size_t toCopy = length - written;
            if (left < toCopy)
            {
                toCopy = left;
            }
            std::memcpy(stream + written, (Uint8*)buffer->buffer + buffer->played, toCopy);
            buffer->played = buffer->played + toCopy;
            written = written + (int)toCopy;

            //Buffer fini : on le met de cote pour le rendre au jeu
            if (buffer->played >= buffer->dataSize)
            {
                audioOut->playing.pop_front();
                audioOut->released.push_back(buffer);
                somethingReleased = true;
            }
        }
        //Plus rien a jouer : du silence
        if (written < length)
        {
            std::memset(stream + written, 0, length - written);
        }
    }
    if (somethingReleased && audioOut->bufferEvent != nullptr)
    {
        nn::os::SignalSystemEvent(&audioOut->bufferEvent->base);
    }
}

void InitializeAudioOutParameter(AudioOutParameter* outParameter)
{
    *outParameter = AudioOutParameter{};
}

Result OpenDefaultAudioOut(AudioOut* outAudioOut, nn::os::SystemEvent* outEvent, const AudioOutParameter& parameter)
{
    SDL_InitSubSystem(SDL_INIT_AUDIO);

    outAudioOut->sampleRate = 48000;
    if (parameter.sampleRate != 0)
    {
        outAudioOut->sampleRate = parameter.sampleRate;
    }
    outAudioOut->channelCount = 2;
    if (parameter.channelCount != 0)
    {
        outAudioOut->channelCount = parameter.channelCount;
    }

    //L'event que le jeu attend pour savoir qu'un buffer a ete joue
    nn::os::CreateSystemEvent(&outEvent->base, nn::os::EventClearMode_AutoClear, false);
    outAudioOut->bufferEvent = outEvent;

    //On demande a SDL une sortie au meme format que la Switch : 16 bits signes
    SDL_AudioSpec wanted;
    SDL_zero(wanted);
    wanted.freq = outAudioOut->sampleRate;
    wanted.format = AUDIO_S16SYS;
    wanted.channels = (Uint8)outAudioOut->channelCount;
    wanted.samples = 1024;
    wanted.callback = AudioCallback;
    wanted.userdata = outAudioOut;

    SDL_AudioDeviceID device = SDL_OpenAudioDevice(nullptr, 0, &wanted, nullptr, 0);
    if (device == 0)
    {
        std::cout << "[nn::audio] " << SDL_GetError() << std::endl;
        return Result{1};
    }
    outAudioOut->device = device;
    outAudioOut->state = AudioOutState_Stopped;
    return Result{};
}

void CloseAudioOut(AudioOut* audioOut)
{
    if (audioOut->device != 0)
    {
        SDL_CloseAudioDevice(audioOut->device);
        audioOut->device = 0;
    }
    if (audioOut->bufferEvent != nullptr)
    {
        nn::os::DestroySystemEvent(&audioOut->bufferEvent->base);
        audioOut->bufferEvent = nullptr;
    }
    std::lock_guard<std::mutex> guard(audioOut->lock);
    audioOut->playing.clear();
    audioOut->released.clear();
}

Result StartAudioOut(AudioOut* audioOut)
{
    SDL_PauseAudioDevice(audioOut->device, 0); //0 = on joue
    audioOut->state = AudioOutState_Started;
    return Result{};
}

void StopAudioOut(AudioOut* audioOut)
{
    SDL_PauseAudioDevice(audioOut->device, 1); //1 = pause
    audioOut->state = AudioOutState_Stopped;
}

const char* GetAudioOutName(const AudioOut* audioOut)
{
    (void)audioOut;
    return "DeviceOut";
}

int GetAudioOutSampleRate(const AudioOut* audioOut)
{
    return audioOut->sampleRate;
}

int GetAudioOutChannelCount(const AudioOut* audioOut)
{
    return audioOut->channelCount;
}

SampleFormat GetAudioOutSampleFormat(const AudioOut* audioOut)
{
    (void)audioOut;
    return SampleFormat_PcmInt16;
}

void SetAudioOutBufferInfo(AudioOutBuffer* outBuffer, void* buffer, size_t bufferSize, size_t dataSize)
{
    outBuffer->buffer = buffer;
    outBuffer->bufferSize = bufferSize;
    outBuffer->dataSize = dataSize;
    outBuffer->played = 0;
}

void* GetAudioOutBufferDataPointer(const AudioOutBuffer* buffer)
{
    return buffer->buffer;
}

size_t GetAudioOutBufferDataSize(const AudioOutBuffer* buffer)
{
    return buffer->dataSize;
}

bool AppendAudioOutBuffer(AudioOut* audioOut, AudioOutBuffer* buffer)
{
    std::lock_guard<std::mutex> guard(audioOut->lock);
    buffer->played = 0;
    audioOut->playing.push_back(buffer);
    return true;
}

AudioOutBuffer* GetReleasedAudioOutBuffer(AudioOut* audioOut)
{
    std::lock_guard<std::mutex> guard(audioOut->lock);
    if (audioOut->released.empty())
    {
        return nullptr;
    }
    AudioOutBuffer* buffer = audioOut->released.front();
    audioOut->released.pop_front();
    return buffer;
}

}
