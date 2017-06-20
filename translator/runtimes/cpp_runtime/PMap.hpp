#ifndef PMAP_HPP
#define PMAP_HPP

#include <vector>
#include <stdexcept>

#include "PTypePtr.h"
#include "DefaultArray.hpp"

using namespace std;

namespace RUNTIME_NAMESPACE {

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

    inline const Int& size() const {
        return data.size();
    }

    inline void insert(const K& k, const V& v) {
        for (Int i = 0; i < data.size(); ++i) {
            if (k == data.get(i).v0()) {
                throw runtime_error("Reinsertion of key into PMap");
            }
        }
        data.add(PTuple<K, V>(k, v));
    }

    inline Bool containsKey(const K& k) const {
        for (Int i = 0; i < data.size(); ++i) {
            if (k == data.get(i).v0()) {
                return true;
            }
        }
        return false;
    }

    inline V get(const K& k) const {
        for (Int i = 0; i < data.size(); ++i) {
            if (k == data.get(i).v0()) {
                return data.get(i).v1();
            }
        }
        throw runtime_error("Key does not exist in dictionary");
    }

    inline Ref<V> getl(const K& k) {
        for (Int i = 0; i < data.size(); ++i) {
            if (k == data.get(i).v0()) {
                return data.getl(i).v1l();
            }
        }
        throw runtime_error("Key does not exist in dictionary");
    }

    inline void set(const K& k, const V& v) {
        for (Int i = 0; i < data.size(); ++i) {
            if (k == data.get(i).v0()) {
                data.getl(i).v1l() = v;
                return;
            }
        }
        insert(k, v);
    }


    inline PList<K> keys() 
    {
        PList<K> ret;
        for(Int i = 0; i < data.size(); ++i)
        {
            ret.add(data.get(i).v0());
        }
        return ret;
    }

    inline PList<V> values() 
    {
        PList<K> ret;
        for(Int i = 0; i < data.size(); ++i)
        {
            ret.add(data.get(i).v1());
        }
        return ret;
    }

    inline Bool operator == (const PMap<K, V>& other) const {
        if(size() == other.size()) {
            for(Int i = 0; i < size(); ++i) {
                if(other.containsEntry(data.get(i).v0(), data.get(i).v1())) {
                    return true;
                }
            }
        }
        return false;
    }

    inline Bool operator != (const PMap<K, V>& other) const {
        return !(*this == other);
    }


private:
    PMap(const PList<PTuple<K, V>> data):data(data) { }

    inline Bool containsEntry(const K& k, const V& v) const {
        for(Int i = 0; i < size(); ++i) {
            if (k == data.get(i).v0()) {
                return v == data.get(i).v1();
            }
        }
        return false;
    }

    PList<PTuple<K, V>> data;
};

};

#endif