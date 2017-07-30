#ifndef VS_PATCHES_H
#define VS_PATCHES_H

namespace std {
template <typename C, typename R, typename ...Args> 
struct hash<R (C::*)(Args...)> {
    size_t operator()(R (C::*x)(Args...)) const {
        return (size_t)reinterpret_cast<const void*>(x);
    }
};
}

namespace RUNTIME_NAMESPACE {

#ifdef USE_VALUE_SUMMARY

template<typename>
struct ExtractVSParam;

template<typename T>
struct ExtractVSParam<ValueSummary<T>> {
    using type = T;
};

template<typename T>
struct ExtractVSParam<const ValueSummary<T>&> {
    using type = T;
};

template<typename T>
struct ExtractVSParam<const ValueSummary<T>&&> {
    using type = T;
};

template<typename T>
struct ExtractVSParam<ValueSummary<T>&&> {
    using type = T;
};

#define INVOKE(ptr, t, method, args) 										         \
    unaryOp<t>((ptr), [&](typename ExtractVSParam<decltype(ptr)>::type p)            \
    {	                                                                             \
    	if(p == NULL) {														         \
    		throw runtime_error("null pointer exception");					         \
    	}																	         \
    	return p->method args;												         \
    })

#define INVOKE_PTR(fPtr, t, args) \
    unaryOp<t>((fPtr), [&](typename ExtractVSParam<decltype(fPtr)>::type fPtr)       \
    {                                                                                \
        if(fPtr == NULL) {                                                           \
            throw runtime_error("null pointer exception");                           \
        }                                                                            \
        return (*fPtr) args;                                                         \
    })


template<typename R, typename A, typename K1, typename K2>
ValueSummary<R> 
getIndex2D(A&& array, K1&& i1, K2&& i2) {
    return binaryOp<ValueSummary<R>>(i1, i2, [&](int i1, int i2) { 
        return array[i1][i2];
    });
}

#else

#define INVOKE(ptr, t, method, args) \
    (ptr->method args)

#define INVOKE_PTR(fPtr, t, args) \
    ((*fPtr) args)

#endif

}
#endif