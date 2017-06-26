#ifndef DEFAULT_ARRAY_HPP
#define DEFAULT_ARRAY_HPP

using namespace std;

namespace RUNTIME_NAMESPACE {

template<typename T>
class DefaultArray 
{
public:
    static const size_t INITIAL_SIZE = 8;

    DefaultArray():
        DefaultArray([]() { return T(); }) { }

    ~DefaultArray() {
        for(int i = 0; i < capacity; i++) {
            if(data[i] != NULL) {
                delete data[i];
            }
        }
        free(data);
    }

    DefaultArray(const DefaultArray& other):
        allocator(other.allocator),
        capacity(other.capacity),
        data((T**)calloc(other.capacity, sizeof(T*)))
    {
        for(int i = 0; i < capacity; i++) {
            if(other.data[i] != NULL) {
                data[i] = new T(*other.data[i]);
            }
        }
    }

    DefaultArray(std::function<T(void)> allocator):
        allocator(allocator),
        capacity(INITIAL_SIZE),
        data((T**)calloc(INITIAL_SIZE, sizeof(T*))) 
    {

    }

    T get(const Int& index) const
    {
#ifdef USE_VALUE_SUMMARY
        return unaryOp<T>(index, [&](int index) {
#endif
            checkIndex(index);
            if(data[index] == NULL) {
                data[index] = new T(allocator());
            }
            return *data[index];
#ifdef USE_VALUE_SUMMARY
        });
#endif
    }

    void set(const T& val, const Int& index)
    {
#ifdef USE_VALUE_SUMMARY
        unaryOp<void>(index, [&](int index) {
#endif
            checkIndex(index);
            if(data[index] == NULL) {
                data[index] = new T(val);
            } else {
                *data[index] = val;
            }
#ifdef USE_VALUE_SUMMARY
        });
#endif
    }

    template<typename V, typename Head, typename ...Rest>
    void set(const V& val, const Int& index, Head head, Rest... rest)
    {
#ifdef USE_VALUE_SUMMARY
        unaryOp<void>(index, [&](int index) {
#endif
            checkIndex(index);
            if(data[index] == NULL) {
                data[index] = new T(allocator());
            }
            data[index]->set(val, head, rest...);
#ifdef USE_VALUE_SUMMARY
        });
#endif
    }

private:

    T** data;
    size_t capacity;
    void checkIndex(int index) const {
        DefaultArray* self = const_cast<DefaultArray<T>*>(this);
        if(index + 1 >= capacity) {
            size_t old_capacity = capacity;
            self->capacity *= 2;
            self->data = (T**)realloc(data, sizeof(T*) * self->capacity);
            memset(self->data + old_capacity, 0, sizeof(T*) * (capacity - old_capacity));
        }
    }

    std::function<T(void)> allocator;
};

};

#endif