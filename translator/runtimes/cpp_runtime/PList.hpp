#ifndef PLIST_HPP
#define PLIST_HPP

#include <vector>
#include <stdexcept>

#include "PTypePtr.h"
#include "DefaultArray.hpp"

using namespace std;

namespace RUNTIME_NAMESPACE {

template<typename T>
class PList final : public PTypePtr
{
    friend class PMachine;
public:

    PList() = default;

    PList(PList&&) = default;

    PList(const PList& other):_size(other.size()) {
        FOR(Int i = 0, i < other.size(), ++i, {
            data.set(other.data.get(i), i);
        })
        ENDFOR_NC()
    }

    inline const Int& size() const {
        return _size;
    }

    inline const PList& operator=(const PList& other) {
        FOR(Int i = 0, i < other.size(), ++i, {
            data.set(other.data.get(i), i);
        })
        ENDFOR_NC()
        _size = other.size();
        return *this;
    }

    template<typename U>
    operator PList<U>() const {
        PList<U> ret;
        FOR(Int i = 0, i < size(), ++i, {
            ret.add(static_cast<U>(data.get(i)));
        })
        ENDFOR_NC()
        return ret;
    }

    inline void add(const T& item) {
        data.set(item, size());
        ++_size;
    }

    inline void insert(const Int& idx, const T& item) {
        FOR(Int i = size(), i > idx, --i, {
            data.set(data.get(i - 1), i);
        })
        ENDFOR_NC()
        data.set(item, idx);
        ++_size;
    }

    inline void insert(const PTuple<Int, T>& item) {
        insert(item.v0(), item.v1());
    }

    inline void removeAt(const Int& idx) {
        FOR(Int i = idx + 1, i < size(), ++i, {
            data.set(data.get(i), i - 1);
        })
        ENDFOR_NC()
        --_size;
    }

    inline void removeRange(const Int& start, const Int& count) {
        FOR(Int i = start + count, i < size(), ++i, {
            data.set(data.get(i), i - count);
        })
        ENDFOR_NC()
        _size = _size - count;
    }

    inline void removeRange(const Int& start) {
        IF_ONLY(start >= size()) {
            throw out_of_range("PList::removeRange");
        }
        _size = start;
    }

    inline T get(const Int& idx) const {
        IF_ONLY(idx >= size()) {
            throw out_of_range("PList::get");
        }
        return data.get(idx);
    }

    template<typename V, typename ...Rest>
    inline void set(const V& value, const Int& idx, Rest... rest) {
        IF_ONLY(idx >= size()) {
            throw out_of_range("PList::get");
        }
        return data.set(value, idx, rest...);
    }

    inline void setTop(const T& value) {
        data.set(value, size() - 1);
    }

    inline FUNCTION_DECL(Bool, operator==, (const PList<T>& other) const) {
        IF(size() == other.size())
        THEN() {
            FOR(Int i = 0, i < size(), ++i, {
                IF(data.get(i) != other.data.get(i))
                THEN() {
                    RETURN(false);
                }
                ENDIF()
            })
            ENDFOR()
            RETURN(true);
        }
        ELSE() {
            RETURN(false);
        }
        ENDIF()
    }
    END_FUNCTION()

    inline Bool operator != (const PList<T>& other) const {
        return !(*this == other);
    }

    template<typename P>
    friend void operator<<(std::ostream&, const PList<P>&);

private:
    Int _size = 0;
    DefaultArray<T> data;
};

template<typename T>
void operator<<(std::ostream& os, const PList<T>& list) {
#ifdef USE_VALUE_SUMMARY
    Bdd oldPc = PathConstraint::pc();
    PathConstraint::pc() = Bdd::bddOne();
    os << "PList[";
    for(int i = 0; i < list.size().maxValue(); ++i) {
        if(i != 0) {
            os << ",";
        }
        os << list.get(i);
    }
    os << "]";
    PathConstraint::pc() = oldPc;
#else
    for(int i = 0; i < list.size(); ++i, {
        if(i != 0) {
            os << ",";
        }
        os << list.get(i);
    }
#endif
}


};

#endif