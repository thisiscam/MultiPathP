#ifndef PLIST_HPP
#define PLIST_HPP

#include <vector>
#include <stdexcept>

#include "IPType.hpp"
#include "DefaultArray.hpp"

using namespace std;

namespace basic_cpp_runtime {

template<typename T> class PMap;
template<typename T> using PMapPtr = unique_ptr<PMap<T>>;

template<typename K, typename V>
class PMap final : public IPType
{
	using KPtr = unique_ptr<K>;
	using VPtr = unique_ptr<V>;
private:
	struct PMapKind {
		const char* tag = "PMapKind";
		PMapKind():k(K::kind()),v(V::kind()) {}
		const IPType::Kind* k;
		const IPType::Kind* v;
	};
	const static PMapKind _kind;

public:
	static constexpr const PMapKind* kind() {
		return &_kind;
	}

	int size() {
		return data.size();
	}

	void insert(const K* k, VPtr v) {
		for (int i=0; i < data.size(); i++) {
			if (k->equals(data.get(i)->item1.get())) {
				throw exception("Reinsertion of key into PMap");
			}
		}
		data.add(new PTuple<K, V>(k->copy(), std::move(v)));
	}

	VPtr get(const K* k) {
		for (int i=0; i < data.size(); i++) {
			if (k->equals(data.get(i)->item1())) {
				return data[i].Item2;
			}
		}
		throw exception("Key does not exist in dictionary");
	}

	void set(const K* k, VPtr v) {
		for (int i=0; i < data.size(); i++) {
			if (k->equals(data.get(i)->item<1>)) {
				data[i].Item2 = v;
				return;
			}
		}
		Insert(k, v);
	}

	IPTypePtr dynamicCopy() override {
		return std::move(copy());
	}

	PMapPtr<T> copy() {
		PMapPtr<T> ret(new PMap<T>());
		for(int i=0; i < size; i++) {
			ret->add(get(i));
		}
		return ret;
	}

	bool equals(const IPType* other) const override {
		
	}

	void checkType(const IPType::Kind* targetKind) override {
		if(targetKind != (IPType::Kind*)kind() && targetKind != IPType::kind()) { // map<K, V> or any
			if(targetKind->tag != _kind.tag) {
				throw new bad_cast();
			}
			PMapKind* targetMapKind = (PMapKind*)targetKind;
			if(targetMapKind->k != IPType::kind()) { // list<any> 
				
			}
		}
	}

private:
	PList<PTuple<K, V>> data;
};

template<typename K, typename V>
const typename PMap<K, V>::PMapKind PMap<K, V>::_kind = PMap<K, V>::PMapKind();

};

#endif