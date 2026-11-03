#pragma once
#include <cstddef>
#include <cstdint>
#include <deque>
#include <mutex>
#include "nn/types.h"
#include "nn/os/event.h"

//Le son : le jeu remplit des buffers d'echantillons et les donne a la sortie audio.
//Quand un buffer a ete joue, l'event est signale et le jeu le recupere pour le remplir a nouveau.
namespace nn::audio
{
    enum SampleFormat {
        SampleFormat_Invalid = 0,
        SampleFormat_PcmInt8,
        SampleFormat_PcmInt16,
        SampleFormat_PcmInt24,
        SampleFormat_PcmInt32,
        SampleFormat_PcmFloat,
        SampleFormat_Adpcm,
    };

    enum AudioOutState {
        AudioOutState_Started = 0,
        AudioOutState_Stopped = 1,
    };

    struct AudioOutParameter {
        int sampleRate = 0;        //0 = valeur par defaut (48000 Hz)
        uint16_t channelCount = 0; //0 = valeur par defaut (2 = stereo)
        uint16_t reserved = 0;
    };

    //Un morceau de son a jouer
    struct AudioOutBuffer {
        void* buffer = nullptr;
        size_t bufferSize = 0;
        size_t dataSize = 0;   //nombre d'octets a jouer
        size_t played = 0;     //nombre d'octets deja joues (pour nous)
    };

    //La sortie audio
    struct AudioOut {
        uint32_t device = 0;                     //SDL_AudioDeviceID
        int sampleRate = 48000;
        int channelCount = 2;
        AudioOutState state = AudioOutState_Stopped;
        nn::os::SystemEvent* bufferEvent = nullptr;
        std::deque<AudioOutBuffer*> playing;     //buffers en attente d'etre joues
        std::deque<AudioOutBuffer*> released;    //buffers deja joues, a rendre au jeu
        std::mutex lock;                         //le son est joue dans un thread de SDL
    };

    void InitializeAudioOutParameter(AudioOutParameter* outParameter);
    Result OpenDefaultAudioOut(AudioOut* outAudioOut, nn::os::SystemEvent* outEvent, const AudioOutParameter& parameter);
    void   CloseAudioOut(AudioOut* audioOut);
    Result StartAudioOut(AudioOut* audioOut);
    void   StopAudioOut(AudioOut* audioOut);

    const char*  GetAudioOutName(const AudioOut* audioOut);
    int          GetAudioOutSampleRate(const AudioOut* audioOut);
    int          GetAudioOutChannelCount(const AudioOut* audioOut);
    SampleFormat GetAudioOutSampleFormat(const AudioOut* audioOut);

    void   SetAudioOutBufferInfo(AudioOutBuffer* outBuffer, void* buffer, size_t bufferSize, size_t dataSize);
    void*  GetAudioOutBufferDataPointer(const AudioOutBuffer* buffer);
    size_t GetAudioOutBufferDataSize(const AudioOutBuffer* buffer);
    bool   AppendAudioOutBuffer(AudioOut* audioOut, AudioOutBuffer* buffer);
    AudioOutBuffer* GetReleasedAudioOutBuffer(AudioOut* audioOut);
}
