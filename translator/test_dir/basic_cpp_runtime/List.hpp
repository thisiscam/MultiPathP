#ifndef PLIST_HPP
#define PLIST_HPP

#include <vector>
#include <stdexcept>

#include "DefaultArray.hpp"

using namespace std;

namespace basic_cpp_runtime {

template<typename T> class List;
template<typename T> using ListPtr = unique_ptr<List<T>>;

template<typename T>
class List final
{
	using TPtr = unique_ptr<T>;

public:

	static ListPtr<T> newEmpty() {
		return ListPtr<T>(new List<T>());
	}

	void add(TPtr item) {
		data[_size] = std::move(item);
		_size++;
	}

	void insert(int idx, TPtr item) {
		for(int i = _size; i > idx; i--) {
			data[i] = data[i - 1];
		}
		data[idx] = std::move(item);
		_size++;
	}

	void removeAt(int idx) {
		for(int i = idx + 1; i < _size; i++) {
			data[i - 1] = data[i];
		}
		_size--;
	}

	void removeRange(int start, int count) {
		for (int i = start + count; i < _size; i++) {
			data[i - count] = data[i];
		}
		_size = _size - count;
	}

	void removeRange(int start) {
		if(_size < start) {
			throw new out_of_range("List::removeRange");
		}
		_size = start;
	}

	TPtr get(int idx) {
		if(idx >= _size) {
			throw out_of_range("List::get");
		}
		return data[idx]->copy();
	}

	void set(int idx, TPtr val) {
		if(idx >= _size) {
			throw out_of_range("List::set");
		}
		data[idx] = std::move(val);
	}

	int size() {
		return _size;
	}

private:

	int _size = 0;
	DefaultArray<TPtr> data;
};

};

#endif