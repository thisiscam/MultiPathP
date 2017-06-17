#ifndef PMAP_HPP
#define PMAP_HPP

#include <vector>
#include <stdexcept>

#include "PTypePtr.h"
#include "DefaultArray.hpp"

using namespace std;

namespace basic_cpp_runtime {

/* TODO: modeling map. */
template<typename K, typename V>
class PMap final : public PTypePtr
{

public:
    PMap() = default;

    template<typename Kp, typename Vp>
    operator PMap<Kp, Vp>() {
        return PMap<Kp, Vp>(static_cast<PList<PTuple<Kp, Vp>>>(data));
    }

    inline int size() const {
        return data.size();
    }

    inline void insert(const K& k, const V& v) {
        for (int i=0; i < data.size(); i++) {
            if (k == data.get(i).v0()) {
                throw runtime_error("Reinsertion of key into PMap");
            }
        }
        data.add(PTuple<K, V>(k, v));
    }

    inline bool containsKey(const K& k) const {
        for (int i=0; i < data.size(); i++) {
            if (k == data.get(i).v0()) {
                return true;
            }
        }
        return false;
    }

    inline const V& get(const K& k) const {
        for (int i=0; i < data.size(); i++) {
            if (k == data.get(i).v0()) {
                return data.get(i).v1;
            }
        }
        throw runtime_error("Key does not exist in dictionary");
    }

    inline V& get(const K& k) {
        for (int i=0; i < data.size(); i++) {
            if (k == data.get(i).v0()) {
                return data.get(i).v1;
            }
        }
        throw runtime_error("Key does not exist in dictionary");
    }

    inline void set(const K& k, const V& v) {
        for (int i=0; i < data.size(); i++) {
            if (k == data.get(i).v0()) {
                data.get(i).v1 = v;
                return;
            }
        }
        insert(k, v);
    }


    inline PList<K> keys() 
    {
        PList<K> ret;
        for(int i = 0; i < data.size(); i++)
        {
            ret.add(data[i].v0());
        }
        return ret;
    }

    inline PList<V> values() 
    {
        PList<K> ret;
        for(int i = 0; i < data.size(); i++)
        {
            ret.add(data[i].v1);
        }
        return ret;
    }

    inline bool operator == (const PMap<K, V>& other) const {
        if(size() == other.size()) {
            for(int i=0; i < size(); i++) {
                if(other.containsEntry(data.get(i).v0(), data.get(i).v1)) {
                    return true;
                }
            }
        }
        return false;
    }

    inline bool operator != (const PMap<K, V>& other) const {
        return !(*this == other);
    }


private:
    PMap(const PList<PTuple<K, V>> data):data(data) { }

    inline bool containsEntry(const K& k, const V& v) const {
        for(int i=0; i < size(); i++) {
            if (k == data.get(i).v0()) {
                return v == data.get(i).v1;
            }
        }
        return false;
    }

    PList<PTuple<K, V>> data;
};

};

#endif