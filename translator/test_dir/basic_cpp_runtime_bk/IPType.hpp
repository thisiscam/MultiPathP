#ifndef IPTYPE_HPP
#define IPTYPE_HPP

#include <memory>
#include <stdexcept>

#include "Utils.h"

namespace basic_cpp_runtime {

class IPType;
using IPTypePtr = std::unique_ptr<IPType>;

class IPType {
public:
	struct Kind {
		const char* tag = "IPTypeKind";
	};

	static constexpr const Kind* kind() {
		return &_kind;
	}

	virtual IPTypePtr dynamicCopy() = 0;
	IPTypePtr copy() {
		return dynamicCopy();
	}

	virtual bool equals (const IPType* other) const = 0 ;

	template<typename T>
	std::unique_ptr<T> cast() {
		checkType((Kind*)T::kind());
		return static_unique_ptr_cast<T>(copy());
	}
	
	virtual void checkType(const Kind* kind) = 0;

private:
	const static Kind _kind;
};

};

#endif