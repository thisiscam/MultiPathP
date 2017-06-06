#ifndef PINTEGER_HPP
#define PINTEGER_HPP

#include <memory>
#include <stdexcept>

#include "IPType.hpp"

namespace basic_cpp_runtime {

class PBoxedInt;
using PBoxedIntPtr = std::unique_ptr<PBoxedInt>;

class PBoxedInt final : public IPType
{

public:
	struct PBoxedIntKind : public IPType::Kind {
		const char* tag = "PBoxedIntKind";
	};
	static constexpr const PBoxedIntKind* kind() {
		return &_kind;
	}

	PBoxedInt(int value):value(value) {

	}

	static PBoxedIntPtr create(int value) {
		return PBoxedIntPtr(new PBoxedInt(value));
	}

	IPTypePtr dynamicCopy() override {
		return std::move(copy());
	}

	PBoxedIntPtr copy() {
		return create(value);
	}

	bool equals(const IPType *other) const override {
		const PBoxedInt* o = dynamic_cast<const PBoxedInt*>(other);
		return o != NULL && value == o->value;
	}

	const int value;

	void checkType(const IPType::Kind* targetKind) override {
		if(targetKind != (IPType::Kind*)kind() && targetKind != IPType::kind()) {
			throw std::bad_cast();
		}
	}

	static const PBoxedIntKind _kind;
};

};

#endif