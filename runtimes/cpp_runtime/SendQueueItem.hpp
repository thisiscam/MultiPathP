#ifndef SEND_QUEUE_ITEM
#define SEND_QUEUE_ITEM

namespace RUNTIME_NAMESPACE {

class PMachine;

struct SendQueueItem final {
public:
    SendQueueItem() = default;

    SendQueueItem(const Ptr<PMachine>& target, const Int& e, const PAny& payload) noexcept:
    	target(target),e(e),payload(payload) 
    {
    	 
    }

    SendQueueItem(Ptr<PMachine>&& target, Int&& e, PAny&& payload) noexcept:
    	target(std::move(target)),
    	e(std::move(e)),
    	payload(std::move(payload)) 
    { }

    Ptr<PMachine> target;
    Int e;
    PAny payload;

#ifdef USE_VALUE_SUMMARY
public:
	class Builder {
		Ptr<PMachine>::Builder target;
	    Int::Builder e;
	    PAny::Builder payload;
	public:
	    inline Builder& addValue(const Bdd& pred, const SendQueueItem& rhs) {
	    	target.addValue(pred, rhs.target);
	    	e.addValue(pred, rhs.e);
	    	payload.addValue(pred, rhs.payload);
	    	return *this;
	    }

	    inline SendQueueItem build() {
	    	return SendQueueItem(target.build(), e.build(), payload.build());
	    }
	};
#endif
};

inline std::ostream& operator<<(std::ostream& os, const SendQueueItem& item) {
	os << "{" << item.target << "," << item.e << "}";
	return os; 
}

};

#endif