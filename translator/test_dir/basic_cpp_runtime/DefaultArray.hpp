#ifndef DEFAULT_ARRAY_HPP
#define DEFAULT_ARRAY_HPP

#include <vector>

using namespace std;

namespace basic_cpp_runtime {

template<typename T>
class DefaultArray 
{
public:
	struct Indexer {
		const int idx;
		DefaultArray& array;
		Indexer(DefaultArray& array, const int idx):array(array),idx(idx) {}
		Indexer& operator= (const T& value) {
			array.data[idx] = value;
			return *this;
		}
		operator T() {
			return array.data[idx];
		}
	};

	Indexer operator[] (const int idx) {
		resizeIfNeeded(idx + 1);
		return Indexer(*this, idx);
	}

	T operator[] (const int index) const
    {
        return data[index];
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