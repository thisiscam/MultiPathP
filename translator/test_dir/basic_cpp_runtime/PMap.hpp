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
	PMap() = default;

    template<typename Kp, typename Vp>
    explicit operator PMap<Kp, Vp>() {
    	return PMap<Kp, Vp>(static_cast<PList<PTuple<Kp, Vp>>>(data));
    }

	int size() const {
		return data.size();
	}

	void insert(const K& k, const V& v) {
		for (int i=0; i < data.size(); i++) {
			if (k == data.get(i).v0) {
				throw runtime_error("Reinsertion of key into PMap");
			}
		}
		data.add(PTuple<K, V>(k, v));
	}

	bool containsKey(const K& k) const {
		for (int i=0; i < data.size(); i++) {
			if (k == data.get(i).v0) {
				return true;
			}
		}
		return false;
	}

	const V& get(const K& k) const {
		for (int i=0; i < data.size(); i++) {
			if (k == data.get(i).v0) {
				return data.get(i).v1;
			}
		}
		throw runtime_error("Key does not exist in dictionary");
	}

	V& get(const K& k) {
		for (int i=0; i < data.size(); i++) {
			if (k == data.get(i).v0) {
				return data.get(i).v1;
			}
		}
		throw runtime_error("Key does not exist in dictionary");
	}

	void set(const K& k, const V& v) {
		for (int i=0; i < data.size(); i++) {
			if (k == data.get(i).v0) {
				data.get(i).v1 = v;
				return;
			}
		}
		insert(k, v);
	}


	bool operator == (const PMap<K, V>& other) const {
		if(size() == other.size()) {
			for(int i=0; i < size(); i++) {
				if(other.containsEntry(data.get(i).v0, data.get(i).v1)) {
					return true;
				}
			}
		}
		return false;
	}

	bool operator != (const PMap<K, V>& other) const {
		return !(*this == other);
	}


private:
	PMap(const PList<PTuple<K, V>> data):data(data) { }

	bool containsEntry(const K& k, const V& v) const {
		for(int i=0; i < size(); i++) {
			if (k == data.get(i).v0) {
				return v == data.get(i).v1;
			}
		}
		return false;
	}

	PList<PTuple<K, V>> data;
};

};

#endif