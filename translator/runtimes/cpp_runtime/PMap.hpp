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
        FOR(Int i = 0, i < data.size(), ++i, {
            IF_ONLY(k == data.get(i).v0()) {
                throw runtime_error("Reinsertion of key into PMap");
            }
        })
        ENDFOR_NC()
        data.add(PTuple<K, V>(k, v));
    }

    inline FUNCTION_DECL(Bool, containsKey, (const K& k) const, {
        FOR(Int i = 0, i < data.size(), ++i, {
            IF(k == data.get(i).v0()) 
            THEN({
                RETURN(true);
            })
            ENDIF()
        })
        ENDFOR()
        RETURN(false);
    })

    inline FUNCTION_DECL(V, get, (const K& k) const, {
        FOR(Int i = 0, i < data.size(), ++i, {
            IF(k == data.get(i).v0()) 
            THEN({
                RETURN(data.get(i).v1());
            })
            ENDIF()
        })
        ENDFOR()
        throw runtime_error("Key does not exist in dictionary");
    })

    inline FUNCTION_DECL(Ref<V>, getl, (const K& k), {
        FOR(Int i = 0, i < data.size(), ++i, {
            if (k == data.get(i).v0()) {
                RETURN(data.getl(i).v1l());
            }
        })
        ENDFOR()
        throw runtime_error("Key does not exist in dictionary");
    })

    inline FUNCTION_DECL(void, set, (const K& k, const V& v), {
        FOR(Int i = 0, i < data.size(), ++i, {
            IF(k == data.get(i).v0()) 
            THEN({
                data.getl(i).v1l() = v;
                RETURN();
            })
            ENDIF()
        })
        ENDFOR()
        insert(k, v);
    })


    inline PList<K> keys() {
        PList<K> ret;
        FOR(Int i = 0, i < data.size(), ++i, {
            ret.add(data.get(i).v0());
        })
        ENDFOR_NC()
        return ret;
    }

    inline PList<V> values() {
        PList<K> ret;
        FOR(Int i = 0, i < data.size(), ++i, {
            ret.add(data.get(i).v1());
        })
        ENDFOR_NC()
        return ret;
    }

    inline FUNCTION_DECL(Bool, operator ==, (const PMap<K, V>& other) const, {
        IF(size() == other.size()) 
        THEN({
            FOR(Int i = 0, i < size(), ++i, {
                IF(other.containsEntry(data.get(i).v0(), data.get(i).v1())) 
                THEN({
                    RETURN(true);
                })
                ENDIF()
            })
            ENDFOR()
        })
        ENDIF()
        RETURN(false);
    })

    inline Bool operator != (const PMap<K, V>& other) const {
        return !(*this == other);
    }


private:
    PMap(const PList<PTuple<K, V>> data):data(data) { }

    inline FUNCTION_DECL(Bool, containsEntry, (const K& k, const V& v) const, {
        FOR(Int i = 0, i < size(), ++i, {
            IF(k == data.get(i).v0()) 
            THEN({
                RETURN(v == data.get(i).v1());
            })
            ENDIF()
        })
        ENDFOR()
        RETURN(false);
    })

    PList<PTuple<K, V>> data;
};

};

#endif