#ifndef PLIST_HPP
#define PLIST_HPP

#include <vector>
#include <stdexcept>

#include "PTypePtr.h"
#include "DefaultArray.hpp"

using namespace std;

namespace basic_cpp_runtime {

template<typename T>
class PList final : public PTypePtr
{
public:

	PList() = default;

	PList(const PList& other):size(other.size) {
		for(int i=0; i < other.size; i++) {
			data[i] = other.data[i];
		}
	}

	const PList& operator=(const PList& other)
    {
        for(int i=0; i < other.size; i++) {
			data[i] = other.data[i];
		}
		size = other.size;
		return *this;
    }

	void add(const T& item) {
		data[size] = item;
		size++;
	}

	void insert(int idx, const T& item) {
		for(int i = size; i > idx; i--) {
			data[i] = data[i - 1];
		}
		data[idx] = item;
		size++;
	}

	void removeAt(int idx) {
		for(int i = idx + 1; i < size; i++) {
			data[i - 1] = data[i];
		}
		size--;
	}

	void removeRange(int start, int count) {
		for (int i = start + count; i < size; i++) {
			data[i - count] = data[i];
		}
		size = size - count;
	}

	void removeRange(int start) {
		if(size < start) {
			throw new out_of_range("PList::removeRange");
		}
		size = start;
	}

	T get(const int idx) const {
		if(idx >= size) {
			throw out_of_range("PList::get");
		}
		return data[idx];
	}

	void set(const int idx, const T& value) {
		if(idx >= size) {
			throw out_of_range("PList::get");
		}
		data[idx] = value;
	}

	int size = 0;

private:
	DefaultArray<T> data;
};

};

#endif