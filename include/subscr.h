#ifndef GOOSE_SUBSCRIBER_H
#define GOOSE_SUBSCRIBER_H

#include <string>
#include <vector>
#include <functional>
#include <cstdint>
#include <memory>

// Предварительное объявление C-структур
struct sGooseSubscriber;
struct sGooseReceiver;


class GooseSubscriber {
public:
    using DataCallback = std::function<void(const std::vector<bool>&, uint64_t, uint32_t, uint32_t)>;
    
    explicit GooseSubscriber(const std::string& goCbRef);
    ~GooseSubscriber();
    
    void SetDstMac(const uint8_t mac[6]);
    void SetAppId(uint32_t appId);
    void SetInterface(const std::string& interface);
    void SetListener(DataCallback callback);
    
    std::vector<bool> GetDataSetValues() const; // получение текущих значений данных
    uint32_t GetStNum() const;  // получение номера состояния
    uint32_t GetSqNum() const; // получение порядкового номера
    uint64_t GetTimestamp() const; // получение временной метки
    uint32_t GetTimeAllowedToLive() const; // получение времени жизни сообщения
    bool IsValid() const; // проверка валидности текущего сообщения
    
    // Внутренние методы для интеграции с C-библиотекой
    sGooseSubscriber* GetCSSubscriber() const { return c_subscriber_; }

private:
    sGooseSubscriber* c_subscriber_{nullptr};
    DataCallback data_callback_;
    std::vector<bool> current_values_;
    uint32_t st_num_{0};
    uint32_t sq_num_{0};
    uint64_t timestamp_{0};
    bool is_valid_{false};
    
    // Статический метод-обертка для C callback
    static void GooseListener(sGooseSubscriber* subscriber, void* parameter);
};

class GooseReceiver {
public:
    explicit GooseReceiver(const std::string& interface = "");
    ~GooseReceiver();
    
    void Start();
    void Stop();
    bool IsRunning() const;
    
    void AddSubscriber(GooseSubscriber& subscriber);
    void RemoveSubscriber(GooseSubscriber& subscriber);
    
    // Статические методы для глобального доступа
    static std::unique_ptr<GooseReceiver> Create();
    static std::unique_ptr<GooseReceiver> CreateRemote(const std::string& remoteAddr, uint16_t port = 102);

private:
    sGooseReceiver* c_receiver_{nullptr};
    std::string interface_;
};

#endif // GOOSE_SUBSCRIBER_H