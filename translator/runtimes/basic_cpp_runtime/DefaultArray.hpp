#ifndef DEFAULT_ARRAY_HPP
#define DEFAULT_ARRAY_HPP

#include <vector>

#include "Ref.hpp"

using namespace std;

namespace basic_cpp_runtime {

template<typename T>
class DefaultArray 
{
public:
    T get(const int index) const
    {
        if(index > data.size()) {
            return T();
        } else {
            return data[index];
        }
    }

    Ref<T> getl(const int index)
    {
        resizeIfNeeded(index + 1);
        return Ref<T>(data[index]);
    }

private:

    vector<T> data;
    void resizeIfNeeded(int length) {
        if(length >= data.size()) {
            data.resize(length + 1);
        }
    }
};

};

#endif