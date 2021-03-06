#ifndef DEFAULT_ARRAY_HPP
#define DEFAULT_ARRAY_HPP

#include <math.h>
#include <cstring>

using namespace std;

namespace RUNTIME_NAMESPACE {

static 
size_t next_pow_of_2(size_t x) {
    return pow(2, ceil(log(x)/log(2)));
}

template<typename T>
class DefaultArray 
{
public:
    static const size_t INITIAL_SIZE = 8;

    DefaultArray() noexcept:
        DefaultArray([]() { return T(); }) { }

    ~DefaultArray() noexcept {
        for(int i = 0; i < capacity; i++) {
            delete data[i];
        }
        free(data);
    }

    DefaultArray(const DefaultArray& other) noexcept:
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

    DefaultArray(DefaultArray&& other) noexcept:
        allocator(std::move(other.allocator)),
        capacity(std::move(other.capacity)),
        data(std::move(other.data)) 
    {
        other.capacity = 0;
        other.data = nullptr;
    }

    DefaultArray(std::function<T(void)> allocator) noexcept:
        allocator(allocator),
        capacity(INITIAL_SIZE),
        data((T**)calloc(INITIAL_SIZE, sizeof(T*))) 
    {

    }

    T get(const Int& index) const
    {
#ifdef USE_VALUE_SUMMARY
        return unaryOp<T>(index, [&](int index) -> const T& {
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

#ifdef REMOVE_AT_OPTIMIAZATION
#ifdef USE_VALUE_SUMMARY
    void removeAt(const Int& idx, const Int& end) {
        int minIdx = INT_MAX; 
        int maxIdx = INT_MIN;
        for(const auto& gvIdx : idx.values) {
            minIdx = std::min(gvIdx.first, minIdx);
            maxIdx = std::max(gvIdx.first, maxIdx);
        }
        int minEnd = INT_MAX; 
        int maxEnd = INT_MIN;
        for(const auto& gvEnd : end.values) {
            minEnd = std::min(gvEnd.first, minEnd);
            maxEnd = std::max(gvEnd.first, maxEnd);
        }
        Bdd oldPc = PathConstraint::pc();
        for(int i = minIdx; i < maxEnd - 1; i++) {
            auto entryIdx = idx.values.find(i);
            if(entryIdx != idx.values.end()) {
                Bdd doCopyPc = oldPc & entryIdx->second;
                PathConstraint::pc() |= doCopyPc;
            }
            auto entryEnd = end.values.find(i);
            if(entryEnd != end.values.end()) {
                Bdd endPc = oldPc & entryEnd->second;
                PathConstraint::pc() &= !endPc;
            }
            if(PathConstraint::isNotZero()) {
                *data[i] = *data[i + 1];
            }
        }
        PathConstraint::pc() = oldPc;
    }
#else
    void removeAt(const Int& idx, const Int& end) {
        FOR(Int i = idx + 1, i < end, ++i, {
            set(get(i), i - 1);
        })
        ENDFOR_NC()
    }
#endif
#endif

#ifdef USE_VALUE_SUMMARY
public:
    class Builder {
        typename T::Builder** data = nullptr;
        size_t capacity = 0;

    public:
        ~Builder() noexcept {
            for(int i = 0; i < capacity; i++) {
                delete data[i];
            }
            free(data);
        }

        inline Builder& addValue(const Bdd& pred, const DefaultArray<T>& rhs) {
            if(capacity < rhs.capacity) {
                size_t old_capacity = capacity;
                capacity = rhs.capacity;
                data = (typename T::Builder**)realloc(data, sizeof(typename T::Builder*) * capacity);
                memset(data + old_capacity, 0, sizeof(typename T::Builder*) * (capacity - old_capacity));
            }
            for(int i = 0; i < capacity; i++) {
                if(data[i] == NULL) {
                    if(rhs.data[i] != NULL) {
                        auto builder = new typename T::Builder();
                        builder->addValue(pred, *rhs.data[i]);
                        data[i] = builder;
                    }
                } else { 
                    if(rhs.data[i] != NULL) {
                        data[i]->addValue(pred, *rhs.data[i]);
                    }
                }
            }
            return *this;
        }

        inline DefaultArray<T> build() {
            T** arrayData = (T**)malloc(sizeof(T*) * capacity);
            for(int i = 0; i < capacity; i++) {
                if(data[i] != NULL) {
                    arrayData[i] = new T(data[i]->build());
                } else {
                    arrayData[i] = NULL;
                }
            }
            return DefaultArray<T>(arrayData, capacity);
        }
    };
#endif

private:

    T** data;
    size_t capacity;
    void checkIndex(int index) const {
        DefaultArray* self = const_cast<DefaultArray<T>*>(this);
        if(index + 1 >= capacity) {
            size_t old_capacity = capacity;
            self->capacity = next_pow_of_2(index + 1);
            self->data = (T**)realloc(data, sizeof(T*) * self->capacity);
            memset(self->data + old_capacity, 0, sizeof(T*) * (self->capacity - old_capacity));
        }
    }

    DefaultArray(T** data, size_t capacity):
        data(data),
        capacity(capacity) 
    { }

    std::function<T(void)> allocator;
};

};

#endif