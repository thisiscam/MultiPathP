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
    unaryOp<t>((ptr), [&](typename ExtractVSParam<decltype(ptr)>::type p) {	 \
    	if(p == NULL) {														         \
    		throw runtime_error("null pointer exception");					         \
    	}																	         \
    	return p->method args;												         \
    })

#define INVOKE_PTR(fPtr, t, args) \
    unaryOp<t>((fPtr), [&](typename ExtractVSParam<decltype(fPtr)>::type fPtr) {     \
        return (*fPtr) args;                                                         \
    })

template<typename T, size_t d1>
ValueSummary<typename std::remove_const<T>::type> getIndex1D(T (&array)[d1], const Int& i1) {
    return unaryOp<ValueSummary<typename std::remove_const<T>::type>>(i1, [&](int i1) { 
        return array[i1]; 
    });
}

template<typename T, size_t d1, size_t d2>
ValueSummary<typename std::remove_const<T>::type> 
getIndex2D(T (&array)[d1][d2], const Int& i1, const Int& i2) {
    return binaryOp<ValueSummary<typename std::remove_const<T>::type>>(i1, i2, [&](int i1, int i2) { 
        return array[i1][i2]; 
    });
}

#else

#define INVOKE(ptr, t, method, args) \
    (ptr->method args)

#define INVOKE_PTR(fPtr, t, args) \
    ((*fPtr) args)

template<typename T, size_t d1>
typename std::remove_const<T>::type getIndex1D(T (&array)[d1], const Int& i1) {
	return array[i1];
}

template<typename T, size_t d1, size_t d2>
typename std::remove_const<T>::type getIndex2D(T (&array)[d1][d2], const Int& i1, const Int& i2) {
    return array[i1][i2];
}

#endif

}
#endif