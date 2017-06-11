#ifndef PLIST_HPP
#define PLIST_HPP

#include <vector>
#include <stdexcept>

#include "IPType.hpp"
#include "DefaultArray.hpp"

using namespace std;

namespace basic_cpp_runtime {

template<typename T> class PList;
template<typename T> using PListPtr = unique_ptr<PList<T>>;

template<typename T>
class PList final : public IPType
{
	using TPtr = unique_ptr<T>;
private:
	struct PListKind {
		const char* tag = "PListKind";
		PListKind():t(T::kind()) {}
		const IPType::Kind* t;
	};
	const static PListKind _kind;

public:
	static constexpr const PListKind* kind() {
		return &_kind;
	}

	void add(TPtr item) {
		data[size] = std::move(item);
		size++;
	}

	void insert(int idx, TPtr item) {
		for(int i = size; i > idx; i--) {
			data[i] = data[i - 1];
		}
		data[idx] = std::move(item);
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

	TPtr get(int idx) {
		if(idx >= size) {
			throw out_of_range("PList::get");
		}
		return data[idx]->copy();
	}

	void set(int idx, TPtr val) {
		if(idx >= size) {
			throw out_of_range("PList::set");
		}
		data[idx] = std::move(val);
	}

	IPTypePtr dynamicCopy() override {
		return std::move(copy());
	}

	PListPtr<T> copy() {
		PListPtr<T> ret(new PList<T>());
		for(int i=0; i < size; i++) {
			ret->add(get(i));
		}
		return ret;
	}

	bool equals(const IPType* other) const override {
		const PList<T>* o1 = dynamic_cast<const PList<T>*>(other);
		if(o1 != NULL) {
			for(int i=0; i < size; i++) {
				if(!data[i].equals(o1->data[i].get())) {
					return false;
				}
			}
		} else {
			const PList<IPType>* o2 = dynamic_cast<const PList<IPType>*>(other);
			if(o2 != NULL) {
				for(int i=0; i < size; i++) {
					if(!data[i].equals(o2->data[i].get())) {
						return false;
					}
				}
			} else {
				return false;
			}
		}
		return true;
	}

	void checkType(const IPType::Kind* targetKind) override {
		if(targetKind != (IPType::Kind*)kind() && targetKind != IPType::kind()) { // list<T> or any
			if(targetKind->tag != _kind.tag) {
				throw new bad_cast();
			}
			PListKind* targetListKind = (PListKind*)targetKind;
			if(targetListKind->t != IPType::kind()) { // list<any> 
				for(int i=0; i < size; i++) {
					data[i]->checkType(targetListKind->t);
				}
			}
		}
	}

private:
	int size = 0;
	DefaultArray<TPtr> data;
};

template<typename T>
const typename PList<T>::PListKind PList<T>::_kind = PList<T>::PListKind();

};

#endif