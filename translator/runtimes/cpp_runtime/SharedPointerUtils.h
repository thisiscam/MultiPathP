#ifndef SHARED_POINTER_UTILS
#define SHARED_POINTER_UTILS

#include <memory>

template<typename FROM, typename T>
FROM get(const std::shared_ptr<T>& ptr) {
    return *(FROM*)ptr.get();
}

template<typename FROM, typename TO, typename T>
TO getCast(const std::shared_ptr<T>& ptr) {
    return static_cast<TO>(get<FROM>(ptr));
}

#ifdef USE_VALUE_SUMMARY
template<typename FROM, typename T>
RUNTIME_NAMESPACE::ValueSummary<FROM> 
get(const RUNTIME_NAMESPACE::ValueSummary<std::shared_ptr<T>>& ptr) {
    return RUNTIME_NAMESPACE::unaryOp<FROM>(ptr, [](const std::shared_ptr<T>& ptr) { return get<FROM>(ptr); });
}

template<typename FROM, typename TO, typename T>
RUNTIME_NAMESPACE::ValueSummary<TO> 
getCast(const RUNTIME_NAMESPACE::ValueSummary<std::shared_ptr<T>>& ptr) {
    return RUNTIME_NAMESPACE::unaryOp<TO>(ptr, [](const std::shared_ptr<T>& ptr) { return getCast<FROM, TO>(ptr); });
}
#endif

#endif