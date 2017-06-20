#ifndef REF_HPP
#define REF_HPP

namespace RUNTIME_NAMESPACE {

#define REF_BODY(T...)                                  \
                                                        \
public:                                                 \
                                                        \
    Ref(T& value):value(&value) { }                     \
                                                        \
    inline const Ref& operator=(const T& other)         \
    {                                                   \
        *value = other;                                 \
        return *this;                                   \
    }                                                   \
                                                        \
private:                                                \
    T* value;                                           \
public:

#define END_REF() };

template<typename T>
class Ref final {
REF_BODY(T)
END_REF()

};

#endif