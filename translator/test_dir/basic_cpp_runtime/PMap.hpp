#ifndef PMAP_HPP
#define PMAP_HPP

#include <vector>
#include <stdexcept>

#include "PTypePtr.h"
#include "DefaultArray.hpp"

using namespace std;

namespace basic_cpp_runtime {

template<typename K, typename V>
class PMap final : public PTypePtr
{

public:

	int size() {
		return data.size();
	}

	void insert(const K& k, const V& v) {
		for (int i=0; i < data.size(); i++) {
			if (k.equals(data[i].v0)) {
				throw runtime_error("Reinsertion of key into PMap");
			}
		}
		data.add(PTuple<K, V>(k, v));
	}

	bool containsKey(const K& k) {
		for (int i=0; i < data.size(); i++) {
			if (k.equals(data[i].v0)) {
				return true;
			}
		}
		return false;
	}

	V get(const K& k) {
		for (int i=0; i < data.size(); i++) {
			if (k.equals(data[i].v0)) {
				return data[i].v1;
			}
		}
		throw runtime_error("Key does not exist in dictionary");
	}

	void set(const K& k, const V& v) {
		for (int i=0; i < data.size(); i++) {
			if (k.equals(data[i].v0)) {
				data[i].v1 = v;
				return;
			}
		}
		insert(k, v);
	}

	// bool equals(const PAny& other) {
	// 	return other.equals(*this);
	// }

	bool equals(const PMap& other) {
		for(int i=0; i < size(); i++) {
			if(containsEntry(data[i].v0, data[i].v1)) {
				return true;
			}
		}
		return false;
	}

private:
	bool containsEntry(const K& k, const V& v) {
		for(int i=0; i < size(); i++) {
			if (k.equals(data[i].v0)) {
				return v.equals(data[i].v1);
			}
		}
		return false;
	}

	PList<PTuple<K, V>> data;
};

};

#endif