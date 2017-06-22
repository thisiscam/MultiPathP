#ifndef DEFAULT_ARRAY_HPP
#define DEFAULT_ARRAY_HPP

#include <vector>

#include "Ref.hpp"

using namespace std;

namespace RUNTIME_NAMESPACE {

template<typename T>
class DefaultArray 
{
public:
    DefaultArray():allocator([]() { return T(); }) { }

    DefaultArray(std::function<T(void)> allocator):allocator(allocator) 
    { }

    T get(const Int& index) const
    {
#ifdef USE_VALUE_SUMMARY
        return unaryOp<T>(index, [&](int index) {
            resizeIfNeeded(index);
            return data[index];
        });
#else
        resizeIfNeeded(index);
        return data[index];
#endif
    }

    Ref<T> getl(const Int& index)
    {
#ifdef USE_VALUE_SUMMARY
        return unaryOp<Ref<T>>(index, [&](int index) {
            resizeIfNeeded(index);
            return Ref<T>(data[index]);
        });
#else
        resizeIfNeeded(index);
        return Ref<T>(data[index]);
#endif
    }

private:

    vector<T> data;
    void resizeIfNeeded(int length) const {
        if(length >= data.size()) {
            const_cast<vector<T>*>(&data)->resize(length + 1, allocator());
        }
    }

    std::function<T(void)> allocator;
};

};

#endif