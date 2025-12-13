/*
 * goose_subscriber_example.c
 *
 * This is an example for a standalone GOOSE subscriber
 *
 * Has to be started as root in Linux.
 */

#include "goose_receiver.h"
#include "goose_subscriber.h"
#include "hal_thread.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "subscr.h"

static void
gooseListener(GooseSubscriber subscriber, void* parameter)
{
    GPIO* gpio=static_cast<GPIO*>(parameter);
    printf("GOOSE event:\n");
    printf("  stNum: %u sqNum: %u\n", GooseSubscriber_getStNum(subscriber),
            GooseSubscriber_getSqNum(subscriber));
    printf("  timeToLive: %u\n", GooseSubscriber_getTimeAllowedToLive(subscriber));

    uint64_t timestamp = GooseSubscriber_getTimestamp(subscriber);

    printf("  timestamp: %u.%u\n", (uint32_t) (timestamp / 1000), (uint32_t) (timestamp % 1000));
    printf("  message is %s\n", GooseSubscriber_isValid(subscriber) ? "valid" : "INVALID");

    MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);

    char buffer[1024];

    MmsValue_printToBuffer(values, buffer, 1024);


    printf("  allData: %s\n", buffer);
    int arraySize = MmsValue_getArraySize(values);
            int i;
            for (i = 0; i < arraySize; i++) {
                MmsValue* pin_value=MmsValue_getElement(values, i);
                bool pin_state=MmsValue_getBoolean(pin_value);
                gpio->SetValue(pin_state,i);
            }
}
namespace TOR{
GooseSubscriber::GooseSubscriber(GPIO* gpio)
{
    receiver = GooseReceiver_create();
    GooseReceiver_setInterfaceId(receiver, "eth0");
    ::GooseSubscriber subscriber = GooseSubscriber_create("RETOM_GOOSECTRL/LLN0$GO$Control_DataSet1", nullptr);

    uint8_t dstMac[6] = {0x01,0x0c,0xcd,0x01,0x00,0x15};
    GooseSubscriber_setDstMac(subscriber, dstMac);
    GooseSubscriber_setAppId(subscriber, 0x0001);

    GooseSubscriber_setListener(subscriber, gooseListener, gpio);

    GooseReceiver_addSubscriber(receiver, subscriber);

    GooseReceiver_start(receiver);
}

GooseSubscriber::~GooseSubscriber()
{
}
}