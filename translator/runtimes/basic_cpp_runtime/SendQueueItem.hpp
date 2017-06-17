#ifndef SEND_QUEUE_ITEM
#define SEND_QUEUE_ITEM

namespace basic_cpp_runtime {

class PMachine;

struct SendQueueItem {
public:
    SendQueueItem() = default;

    SendQueueItem(PMachine*& target, const int& e, const PAny& payload):target(target),e(e),payload(payload) { }

    PMachine* target;
    int e;
    PAny payload;
};

};

#endif