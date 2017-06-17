#ifndef PLIST_HPP
#define PLIST_HPP

#include <vector>
#include <stdexcept>

#include "PTypePtr.h"
#include "DefaultArray.hpp"

using namespace std;

namespace basic_cpp_runtime {

template<typename T>
class PList final : public PTypePtr
{
    friend class PMachine;
public:

    PList() = default;

    PList(PList&&) = default;

    PList(const PList& other):_size(other.size()) {
        for(int i=0; i < other.size(); i++) {
            data.set(i, other.data.get(i));
        }
    }

    inline int size() const {
        return _size;
    }

    inline const PList& operator=(const PList& other)
    {
        for(int i=0; i < other.size(); i++) {
            data.set(i, other.data.get(i));
        }
        _size = other.size();
        return *this;
    }

    template<typename U>
    operator PList<U>() const {
        PList<U> ret;
        for(int i = 0; i < size(); i++) {
            ret.add(static_cast<U>(data.get(i)));
        }
        return ret;
    }


    inline void add(const T& item) {
        data.set(size(), item);
        _size++;
    }

    inline void insert(int idx, const T& item) {
        for(int i = size(); i > idx; i--) {
            data.set(i, data.get(i - 1));
        }
        data.set(idx, item);
        _size++;
    }

    inline void removeAt(int idx) {
        for(int i = idx + 1; i < size(); i++) {
            data.set(i - 1, data.get(i));
        }
        _size--;
    }

    inline void removeRange(int start, int count) {
        for (int i = start + count; i < size(); i++) {
            data.set(i - count, data.get(i));
        }
        _size = _size - count;
    }

    inline void removeRange(int start) {
        if(size() < start) {
            throw new out_of_range("PList::removeRange");
        }
        _size = start;
    }

    inline const T& get(const int idx) const {
        if(idx >= size()) {
            throw out_of_range("PList::get");
        }
        return data.get(idx);
    }

    inline T& get(const int idx) {
        if(idx >= size()) {
            throw out_of_range("PList::get");
        }
        return data.get(idx);
    }

    inline void set(const int idx, const T& value) {
        if(idx >= size()) {
            throw out_of_range("PList::get");
        }
        data.set(idx, value);
    }

    inline void setTop(const T& value) {
        data.set(size() - 1, value);
    }

    inline bool operator == (const PList<T>& other) const {
        if(size() == other.size()) {
            for(int i=0; i < size(); i++) {
                if(data.get(i) != other.data.get(i)) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }

    inline bool operator != (const PList<T>& other) const {
        return !(*this == other);
    }

private:
    int _size = 0;
    DefaultArray<T> data;
};

};

#endif