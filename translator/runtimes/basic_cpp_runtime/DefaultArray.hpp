#ifndef DEFAULT_ARRAY_HPP
#define DEFAULT_ARRAY_HPP

#include <vector>

using namespace std;

namespace basic_cpp_runtime {

template<typename T>
class DefaultArray 
{
public:
    const T& get(const int index) const
    {
        if(index > data.size()) {
            return get_default();
        } else {
            return data[index];
        }
    }

    T& get(const int index)
    {
        resizeIfNeeded(index + 1);
        return data[index];
    }

    void set(const int index, const T& value) {
        resizeIfNeeded(index + 1);
        data[index] = value;
    }

private:
    static const T& get_default() {
        static T d;
        return d;
    }
    vector<T> data;
    void resizeIfNeeded(int length) {
        if(length >= data.size()) {
            data.resize(length + 1);
        }
    }
};

};

#endif