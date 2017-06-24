#ifndef SEND_QUEUE_ITEM
#define SEND_QUEUE_ITEM

namespace RUNTIME_NAMESPACE {

class PMachine;

struct SendQueueItem {
public:
    SendQueueItem() = default;

    SendQueueItem(const Ptr<PMachine>& target, const Int& e, const PAny& payload):target(target),e(e),payload(payload) { }

    Ptr<PMachine> target;
    Int e;
    PAny payload;
};

inline std::ostream& operator<<(std::ostream& os, const SendQueueItem& item) {
	os << "{" << item.target << "," << item.e << "}";
	return os; 
}

};

#endif