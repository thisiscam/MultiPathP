#ifndef PTYPE_BOXED_HPP
#define PTYPE_BOXED_HPP

namespace RUNTIME_NAMESPACE {

template<typename T>
struct PTypeBoxed : public AbstractPTypePtr<PTypeBoxed<T>> {

	PTypeBoxed(const PTypeBoxed<T>&) = default;

	PTypeBoxed(PTypeBoxed<T>&&) = default;

	PTypeBoxed& operator=(const PTypeBoxed<T>&) = default;

	PTypeBoxed& operator=(PTypeBoxed<T>&&) = default;

	PTypeBoxed(const T& value):
		value(value)
	{ }

	explicit inline operator T() const {
		return value;
	}

	inline Bool operator==(const PTypeBoxed& other) const {
		return value == other.value;
	}

	inline Bool operator!=(const PTypeBoxed& other) const {
		return value != other.value;
	}

	~PTypeBoxed() override = default;

	T value;
};

}

#endif