
#include "subscr.h"
#include <iostream>
#include <cstring>
#include <memory>

extern "C" {
    #include "goose_receiver.h"
    #include "goose_subscriber.h"
    #include "mms_value.h"
    #include "linked_list.h"
}

// C callback wrapper
void GooseSubscriber::GooseListener(sGooseSubscriber* subscriber, void* parameter) {
    GooseSubscriber* self = static_cast<GooseSubscriber*>(parameter);
    if (!self || !subscriber) return;
    
    // Обновляем состояние
    self->st_num_ = GooseSubscriber_getStNum(subscriber);
    self->sq_num_ = GooseSubscriber_getSqNum(subscriber);
    self->timestamp_ = GooseSubscriber_getTimestamp(subscriber);
    self->is_valid_ = GooseSubscriber_isValid(subscriber) != 0;
    
    // Получаем данные
    MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);
    if (values && MmsValue_getType(values) == MMS_ARRAY) {
        int size = MmsValue_getArraySize(values);
        self->current_values_.clear();
        
        for (int i = 0; i < size; ++i) {
            MmsValue* element = MmsValue_getElement(values, i);
            if (element && MmsValue_getType(element) == MMS_BOOLEAN) {
                self->current_values_.push_back(MmsValue_getBoolean(element) != 0);
            }
        }
    }
    
    // Вызываем пользовательский callback
    if (self->data_callback_) {
        self->data_callback_(self->current_values_, self->timestamp_, 
                           self->st_num_, self->sq_num_);
    }
    
    // Выводим информацию в консоль
    std::cout << "\n=== GOOSE Event ===" << std::endl;
    std::cout << "  stNum: " << self->st_num_ 
              << ", sqNum: " << self->sq_num_ << std::endl;
    std::cout << "  timeToLive: " << GooseSubscriber_getTimeAllowedToLive(subscriber) << std::endl;
    
    uint64_t ts = self->timestamp_;
    std::cout << "  timestamp: " << (ts / 1000) << "." << (ts % 1000) << std::endl;
    std::cout << "  message is " << (self->is_valid_ ? "valid" : "INVALID") << std::endl;
    
    if (values) {
        char buffer[1024];
        MmsValue_printToBuffer(values, buffer, sizeof(buffer));
        std::cout << "  allData: " << buffer << std::endl;
    }
    
    std::cout << "  Data values: ";
    for (bool val : self->current_values_) {
        std::cout << (val ? "1" : "0") << " ";
    }
    std::cout << "\n===================" << std::endl;
}

// GooseSubscriber implementation
GooseSubscriber::GooseSubscriber(const std::string& goCbRef) {
    c_subscriber_ = GooseSubscriber_create(goCbRef.c_str(), nullptr);
    if (c_subscriber_) {
        std::cout << "GOOSE Subscriber created for: " << goCbRef << std::endl;
    } else {
        std::cerr << "Failed to create GOOSE subscriber for: " << goCbRef << std::endl;
    }
}

GooseSubscriber::~GooseSubscriber() {
    if (c_subscriber_) {
        // Note: Деструкция управляется GooseReceiver
        // GooseSubscriber_destroy(c_subscriber_);
    }
}

void GooseSubscriber::SetDstMac(const uint8_t mac[6]) {
    if (c_subscriber_) {
        GooseSubscriber_setDstMac(c_subscriber_, const_cast<uint8_t*>(mac));
    }
}

void GooseSubscriber::SetAppId(uint32_t appId) {
    if (c_subscriber_) {
        GooseSubscriber_setAppId(c_subscriber_, appId);
    }
}

void GooseSubscriber::SetInterface(const std::string& interface) {
    // Интерфейс настраивается на уровне GooseReceiver
}

void GooseSubscriber::SetListener(DataCallback callback) {
    data_callback_ = callback;
    if (c_subscriber_) {
        GooseSubscriber_setListener(c_subscriber_, GooseSubscriber::GooseListener, this);
    }
}

std::vector<bool> GooseSubscriber::GetDataSetValues() const {
    return current_values_;
}

uint32_t GooseSubscriber::GetStNum() const {
    return st_num_;
}

uint32_t GooseSubscriber::GetSqNum() const {
    return sq_num_;
}

uint64_t GooseSubscriber::GetTimestamp() const {
    return timestamp_;
}

uint32_t GooseSubscriber::GetTimeAllowedToLive() const {
    if (c_subscriber_) {
        return GooseSubscriber_getTimeAllowedToLive(c_subscriber_);
    }
    return 0;
}

bool GooseSubscriber::IsValid() const {
    return is_valid_;
}

// GooseReceiver implementation
GooseReceiver::GooseReceiver(const std::string& interface) 
    : interface_(interface) {
    if (interface_.empty()) {
        c_receiver_ = GooseReceiver_create();
    } else {
        c_receiver_ = GooseReceiver_create();
        GooseReceiver_setInterfaceId(c_receiver_, interface_.c_str());
    }
    
    if (c_receiver_) {
        std::cout << "GOOSE Receiver created" 
                  << (interface_.empty() ? "" : " on interface: " + interface_) 
                  << std::endl;
    }
}

GooseReceiver::~GooseReceiver() {
    Stop();
    if (c_receiver_) {
        GooseReceiver_destroy(c_receiver_);
    }
}

void GooseReceiver::Start() {
    if (c_receiver_) {
        GooseReceiver_start(c_receiver_);
    }
}

void GooseReceiver::Stop() {
    if (c_receiver_) {
        GooseReceiver_stop(c_receiver_);
    }
}

bool GooseReceiver::IsRunning() const {
    if (c_receiver_) {
        return GooseReceiver_isRunning(c_receiver_) != 0;
    }
    return false;
}

void GooseReceiver::AddSubscriber(GooseSubscriber& subscriber) {
    if (c_receiver_ && subscriber.GetCSSubscriber()) {
        GooseReceiver_addSubscriber(c_receiver_, subscriber.GetCSSubscriber());
    }
}

void GooseReceiver::RemoveSubscriber(GooseSubscriber& subscriber) {
    if (c_receiver_ && subscriber.GetCSSubscriber()) {
        GooseReceiver_removeSubscriber(c_receiver_, subscriber.GetCSSubscriber());
    }
}

std::unique_ptr<GooseReceiver> GooseReceiver::Create() {
    return std::make_unique<GooseReceiver>();
}

std::unique_ptr<GooseReceiver> GooseReceiver::CreateRemote(const std::string& remoteAddr, uint16_t port) {
    // Для удалённого режима требуется RSession (как в примере)
    // Эта реализация - упрощённая версия
    auto receiver = std::make_unique<GooseReceiver>();
    // Здесь должна быть настройка RSession для multicast
    return receiver;
}