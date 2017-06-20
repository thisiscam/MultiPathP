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
    T get(const Int& index) const
    {
        if(index > data.size()) {
            return T();
        } else {
            return data[index];
        }
    }

    Ref<T> getl(const Int& index)
    {
        resizeIfNeeded(index + 1);
        return Ref<T>(data[index]);
    }

private:

    vector<T> data;
    void resizeIfNeeded(const Int& length) {
        if(length >= data.size()) {
            data.resize(length + 1);
        }
    }
};

};

#endif