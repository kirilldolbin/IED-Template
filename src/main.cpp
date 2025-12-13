
#include "subscr.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>
#include <cstring>
#include "GPIO.h"

using namespace std::chrono_literals;

std::atomic<bool> running{true};

void signalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
    running = false;
}

void customGooseHandler(const std::vector<bool>& values, uint64_t timestamp,
                        uint32_t stNum, uint32_t sqNum) {
    std::cout << "\n[Custom Handler] GOOSE Event:" << std::endl;
    std::cout << "  Sequence: stNum=" << stNum << ", sqNum=" << sqNum << std::endl;
    std::cout << "  Timestamp: " << timestamp << std::endl;
    std::cout << "  Data: ";
    for (size_t i = 0; i < values.size(); ++i) {
        std::cout << "[" << i << "]=" << (values[i] ? "1" : "0") << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{

    GPIO gpio;
    TOR::GooseSubscriber Minevich("simpleIOGenericIO/LLN0$GO$gcbAnalogValues");
    // bool value = false;
    int i = 0;
    while (true)
    {
        
        gpio.SetValue(false, i);
        i++;
        if (i > 3)
        {
            i = 0;
        }
        gpio.SetValue(true, i);
        std::this_thread::sleep_for(100ms);
    }
    

}