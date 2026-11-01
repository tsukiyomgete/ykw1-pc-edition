#include <iostream>
#include "nn/os.h"
using namespace std;

void SayHello(void* argument)
{
    (void)argument;
    cout << "Hello from the thread NINTENDO" << endl;
}


int main()
{
    cout << "Booting up YKW1 PC EDITION" << endl;
    nn::os::ThreadType testThread;
    nn::os::CreateThread(&testThread, SayHello, nullptr, nullptr, 0, 0);
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

    return 0;
}