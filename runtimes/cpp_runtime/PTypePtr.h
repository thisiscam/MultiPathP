#ifndef PTYPEPTR_H
#define PTYPEPTR_H

#include <utility>

namespace RUNTIME_NAMESPACE {

class PTypePtr {
public:
	virtual PTypePtr& assign(const PTypePtr& other) = 0; // don't mess around with operator=
	virtual PTypePtr* clone() const = 0;
    virtual ~PTypePtr() = default;
};

template<typename SELF_TYPE>
class AbstractPTypePtr : public PTypePtr {
public:
	inline PTypePtr& assign(const PTypePtr& other) final {
		static_cast<SELF_TYPE*>(this)->operator=(dynamic_cast<const SELF_TYPE&>(other));
		return *this;
	}

	inline PTypePtr* clone() const final {
		return new SELF_TYPE(*static_cast<const SELF_TYPE*>(this));
	}
};

};

#endif