
#include "subscr.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>
#include <cstring>
#include "GPIO.h"

using namespace std::chrono_literals;

// std::atomic<bool> running{true};

// void signalHandler(int signal) {
//     std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
//     running = false;
// }

// void customGooseHandler(const std::vector<bool>& values, uint64_t timestamp,
//                         uint32_t stNum, uint32_t sqNum) {
//     std::cout << "\n[Custom Handler] GOOSE Event:" << std::endl;
//     std::cout << "  Sequence: stNum=" << stNum << ", sqNum=" << sqNum << std::endl;
//     std::cout << "  Timestamp: " << timestamp << std::endl;
//     std::cout << "  Data: ";
//     for (size_t i = 0; i < values.size(); ++i) {
//         std::cout << "[" << i << "]=" << (values[i] ? "1" : "0") << " ";
//     }
//     std::cout << std::endl;
// }

int main(int argc, char **argv)
{

    GPIO gpio;
    bool value = false;

    while (true)
    {
        for (int i = 0; i < 4; i++)
        {
            gpio.SetValue(value, i);
            std::this_thread::sleep_for(1s);
            value = !value;
            gpio.SetValue(value, i);
        }
    }

    // // Устанавливаем обработчики сигналов
    // std::signal(SIGINT, signalHandler);
    // std::signal(SIGTERM, signalHandler);

    // std::string interface = "eth0";
    // if (argc > 1) {
    //     interface = argv[1];
    // }

    // std::cout << "Starting GOOSE Receiver on interface: " << interface << std::endl;

    // try {
    //     // Создаём GooseReceiver
    //     auto receiver = GooseReceiver::Create();
    //     if (interface != "") {
    //         receiver = std::make_unique<GooseReceiver>(interface);
    //     }

    //     if (!receiver) {
    //         std::cerr << "Failed to create GOOSE receiver" << std::endl;
    //         return 1;
    //     }

    //     // Создаём GooseSubscriber с тем же GOOSE Control Block Reference, что и у издателя
    //     GooseSubscriber subscriber("SPbPUIO/oolegkaa");

    //     // Настраиваем подписчика в соответствии с издателем
    //     uint8_t dstMac[6] = {0x01, 0x0c, 0xcd, 0x01, 0x00, 0x01};
    //     subscriber.SetDstMac(dstMac);
    //     subscriber.SetAppId(1000);

    //     // Устанавливаем пользовательский обработчик
    //     subscriber.SetListener(customGooseHandler);

    //     // Добавляем подписчика в приёмник
    //     receiver->AddSubscriber(subscriber);

    //     // Запускаем приёмник
    //     receiver->Start();

    //     if (receiver->IsRunning()) {
    //         std::cout << "GOOSE Receiver is running. Press Ctrl+C to stop." << std::endl;
    //         std::cout << "Waiting for GOOSE messages with AppId=1000..." << std::endl;

    //         // Основной цикл
    //         while (running) {
    //             // Периодически выводим текущее состояние
    //             auto values = subscriber.GetDataSetValues();

    //             std::cout << "\rCurrent state - stNum: " << subscriber.GetStNum()
    //                       << ", sqNum: " << subscriber.GetSqNum()
    //                       << ", Valid: " << (subscriber.IsValid() ? "Yes" : "No")
    //                       << ", Values: ";
    //             for (bool val : values) {
    //                 std::cout << (val ? "1" : "0") << " ";
    //             }
    //             std::cout << "       " << std::flush;

    //             std::this_thread::sleep_for(100ms);
    //         }

    //         // Останавливаем приёмник
    //         receiver->Stop();
    //         std::cout << "\nGOOSE Receiver stopped successfully." << std::endl;

    //     } else {
    //         std::cerr << "Failed to start GOOSE receiver." << std::endl;
    //         std::cerr << "Possible reasons:" << std::endl;
    //         std::cerr << "  - Ethernet interface '" << interface << "' doesn't exist" << std::endl;
    //         std::cerr << "  - Root permissions are required" << std::endl;
    //         std::cerr << "  - GOOSE traffic is not arriving on the interface" << std::endl;
    //         return 1;
    //     }

    // } catch (const std::exception& e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    //     return 1;
    // }

    return 0;
}