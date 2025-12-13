#ifndef GOOSE_SUBSCRIBER_H
#define GOOSE_SUBSCRIBER_H

#include <string>
#include <vector>
#include <functional>
#include <cstdint>
#include <memory>
#include "goose_receiver.h"
#include "goose_subscriber.h"


namespace TOR{
class GooseSubscriber {
public:
    using DataCallback = std::function<void(const std::vector<bool>&, uint64_t, uint32_t, uint32_t)>;
    
    explicit GooseSubscriber(const std::string& goCbRef);
    ~GooseSubscriber();
    
private:
   GooseReceiver receiver;
    
    // Статический метод-обертка для C callback
    static void GooseListener(sGooseSubscriber* subscriber, void* parameter);
};
}
#endif // GOOSE_SUBSCRIBER_H