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
        for(Int i = 0; i < other.size(); ++i) {
            data.getl(i) = other.data.get(i);
        }
    }

    inline const Int& size() const {
        return _size;
    }

    inline const PList& operator=(const PList& other)
    {
        for(Int i = 0; i < other.size(); ++i) {
            data.getl(i) = other.data.get(i);
        }
        _size = other.size();
        return *this;
    }

    template<typename U>
    operator PList<U>() const {
        PList<U> ret;
        for(Int i = 0; i < size(); ++i) {
            ret.add(static_cast<U>(data.get(i)));
        }
        return ret;
    }

    inline void add(const T& item) {
        data.getl(size()) = item;
        _size++;
    }

    inline void insert(const Int& idx, const T& item) {
        for(Int i = size(); i > idx; i--) {
            data.getl(i) = data.get(i - 1);
        }
        data.getl(idx) = item;
        _size++;
    }

    inline void insert(const PTuple<Int, T>& item) {
        insert(item.v0(), item.v1());
    }

    inline void removeAt(const Int& idx) {
        for(Int i = idx + 1; i < size(); ++i) {
            data.getl(i - 1) = data.get(i);
        }
        _size--;
    }

    inline void removeRange(const Int& start, const Int& count) {
        for (Int i = start + count; i < size(); ++i) {
            data.getl(i - count) = data.get(i);
        }
        _size = _size - count;
    }

    inline void removeRange(const Int& start) {
        if(size() < start) {
            throw new out_of_range("PList::removeRange");
        }
        _size = start;
    }

    inline T get(const Int& idx) const {
        if(idx >= size()) {
            throw out_of_range("PList::get");
        }
        return data.get(idx);
    }

    inline Ref<T> getl(const Int& idx) {
        if(idx >= size()) {
            throw out_of_range("PList::get");
        }
        return data.getl(idx);
    }

    inline void setTop(const T& value) {
        data.getl(size() - 1) = value;
    }

    inline Bool operator == (const PList<T>& other) const {
        if(size() == other.size()) {
            for(Int i = 0; i < size(); ++i) {
                if(data.get(i) != other.data.get(i)) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }

    inline Bool operator != (const PList<T>& other) const {
        return !(*this == other);
    }

private:
    Int _size = 0;
    DefaultArray<T> data;
};

template<typename T>
class Ref<PList<T>> final {
    REF_BODY(PList<T>)
    Ref<T> getl(const Int& idx) { return value->getl(idx); }
};

};

#endif