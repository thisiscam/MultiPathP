#ifndef DEFAULT_ARRAY_HPP
#define DEFAULT_ARRAY_HPP

#include <vector>

using namespace std;

namespace basic_cpp_runtime {

template<typename T>
class DefaultArray 
{
public:
	T& operator[] (const int idx) {
		resizeIfNeeded(idx + 1);
		return data[idx];
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