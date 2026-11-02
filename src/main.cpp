#include <iostream>
#include "nn/os.h"
#include "nn/fs.h"
#include "nn/fs/detail.h"
#include "nn/hid.h"
using namespace std;

void SayHello(void* argument)
{
    (void)argument;
    cout << "I am: " << nn::os::GetThreadNamePointer(nn::os::GetCurrentThread()) << endl;
    cout << "Thread id: " << nn::os::GetThreadId(nn::os::GetCurrentThread()) << endl;
}


int main()
{
    cout << "Booting up YKW1 PC EDITION" << endl;
    nn::os::ThreadType testThread;
    nn::os::CreateThread(&testThread, SayHello, nullptr, nullptr, 0, 0);
    nn::os::SetThreadName(&testThread, "TestThread");
    nn::os::SetThreadName(nn::os::GetCurrentThread(), "MainThread");
    cout << "I am: " << nn::os::GetThreadNamePointer(nn::os::GetCurrentThread()) << endl;
    nn::os::StartThread(&testThread);
    cout << "Hello from main" << endl;
    nn::os::WaitThread(&testThread);
    nn::os::DestroyThread(&testThread);
    cout << "Thread finished" << endl;

    nn::os::Tick start = nn::os::GetSystemTick();
    nn::os::SleepThread(nn::TimeSpan::FromMilliSeconds(500));
    nn::os::Tick end = nn::os::GetSystemTick();
    cout << "Ticks: " << (end.value - start.value) << endl;



    nn::os::MutexType testMutex;
    nn::os::InitializeMutex(&testMutex, true, 0);
    nn::os::LockMutex(&testMutex);
    cout << "Mutex locked" << endl;
    nn::os::UnlockMutex(&testMutex);
    cout << "Mutex unlocked" << endl;
    nn::os::FinalizeMutex(&testMutex);

    cout << "Before mount: [" << nn::fs::detail::ResolvePath("rom:/yw1_a.fa") << "]" << endl;
    nn::fs::MountRom("rom", nullptr, 0);
    nn::fs::MountSaveData("save", nn::account::Uid{});
    cout << "rom:/yw1_a.fa -> " << nn::fs::detail::ResolvePath("rom:/yw1_a.fa") << endl;
    cout << "save:/game1.yw -> " << nn::fs::detail::ResolvePath("save:/game1.yw") << endl;
    nn::fs::FileHandle file;
    if (nn::fs::OpenFile(&file, "rom:/yw1_a.fa", nn::fs::OpenMode_Read).IsSuccess())
    {
        char magic[5] = {};
        int64_t size = 0;
        nn::fs::ReadFile(file, 0, magic, 4);
        nn::fs::GetFileSize(&size, file);
        cout << "yw1_a.fa starts with: " << magic << ", size: " << size << endl;
        nn::fs::CloseFile(file);
    }
    else
    {
        cout << "yw1_a.fa not found" << endl;
    }
    nn::fs::Unmount("rom");
    cout << "After unmount: [" << nn::fs::detail::ResolvePath("rom:/yw1_a.fa") << "]" << endl;

    // Gamepad test: press buttons for 10 seconds
    nn::hid::InitializeNpad();
    cout << "Gamepad test (10 s): press buttons, move the sticks" << endl;
    const char* names[] = { "A", "B", "X", "Y", "StickL", "StickR", "L", "R", "ZL", "ZR",
                            "Plus", "Minus", "Left", "Up", "Right", "Down" };
    uint32_t previous = 0;
    nn::os::Tick testStart = nn::os::GetSystemTick();
    while (nn::os::ConvertToTimeSpan(nn::os::Tick{ nn::os::GetSystemTick().value - testStart.value }).nanoSeconds < 10'000'000'000LL)
    {
        nn::hid::NpadFullKeyState pad;
        nn::hid::GetNpadState(&pad, nn::hid::NpadId::No1);
        uint32_t now = pad.buttons.storage[0] & 0xFFFF;
        if (now != previous)
        {
            cout << "Buttons:";
            for (int i = 0; i < 16; i++)
                if (pad.buttons.Test(i))
                    cout << " " << names[i];
            cout << "   stick L (" << pad.analogStickL.x << ", " << pad.analogStickL.y << ")" << endl;
            previous = now;
        }
        nn::os::SleepThread(nn::TimeSpan::FromMilliSeconds(16));
    }

    return 0;
}