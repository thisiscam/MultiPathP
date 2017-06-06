#ifndef PINTEGER_HPP
#define PINTEGER_HPP

#include <memory>
#include <stdexcept>

#include "IPType.hpp"

namespace basic_cpp_runtime {

class PBoxedBool;
using PBoxedBoolPtr = std::unique_ptr<PBoxedBool>;

class PBoxedBool final : public IPType
{

public:
	struct PBoxedBoolKind : public IPType::Kind {
		const char* tag = "PBoxedBoolKind";
	};
	static constexpr const PBoxedBoolKind* kind() {
		return &_kind;
	}

	PBoxedBool(bool value):value(value) {

	}

	static PBoxedBoolPtr create(bool value) {
		return PBoxedBoolPtr(new PBoxedBool(value));
	}

	IPTypePtr dynamicCopy() override {
		return std::move(copy());
	}

	PBoxedBoolPtr copy() {
		return create(value);
	}

	bool equals(const IPType* other) const override {
		PBoxedBool* o = dynamic_cast<const PBoxedBool*>(other);
		return o != NULL && value == o->value;
	}

	const bool value;

	void checkType(const IPType::Kind* targetKind) override {
		if(targetKind != (IPType::Kind*)kind() && targetKind != IPType::kind()) {
			throw std::bad_cast();
		}
	}

	static const PBoxedBoolKind _kind;
};

};

#endif