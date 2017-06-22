#ifndef VS_PATCHES_H
#define VS_PATCHES_H

namespace RUNTIME_NAMESPACE {

#ifdef USE_VALUE_SUMMARY

template<typename>
struct ExtractVSParam;

template<typename T>
struct ExtractVSParam<ValueSummary<T>> {
    using type = T;
};

#define INVOKE(ptr, t, method, args) \
    unaryOp<t>((ptr), [&](typename ExtractVSParam<decltype(ptr)>::type p) { return p->method args; })

#define INVOKE_PTR_ON_THIS(fPtr, t, args) \
    unaryOp<t>((fPtr), [&](typename ExtractVSParam<decltype(fPtr)>::type fPtr) { return (this->*fPtr) args; })

template<typename T, size_t d1>
typename std::remove_const<T>::type getIndex1D(T (&array)[d1], Int i1) {
    return unaryOp<typename std::remove_const<T>::type>(i1, [&](int i1) { return array[i1]; });
}

template<typename T, size_t d1, size_t d2>
typename std::remove_const<T>::type getIndex2D(T (&array)[d1][d2], Int i1, Int i2) {
    return binaryOp<typename std::remove_const<T>::type>(i1, i2, [&](int i1, int i2) { return array[i1][i2]; });
}

#else

#define INVOKE(ptr, t, method, args) \
    (ptr->method args)

#define INVOKE_PTR_ON_THIS(fPtr, t, args) \
    ((this->*fPtr) args)

template<typename T, size_t d1>
typename std::remove_const<T>::type getIndex1D(T (&array)[d1], Int i1) {
	return array[i1];
}

template<typename T, size_t d1, size_t d2>
typename std::remove_const<T>::type getIndex2D(T (&array)[d1][d2], Int i1, Int i2) {
    return array[i1][i2];
}

#endif

}
#endif