#ifndef PTYPES_H
#define PTYPES_H

#include <type_traits>

namespace RUNTIME_NAMESPACE {

class PAny;
class PMachine;
template<typename T> class PList;
template<typename K, typename V> class PMap;
template<typename ...Ts> class PTuple;

/* Region Declare Types */
template<typename ...> struct List { };

#ifdef USE_VALUE_SUMMARY
template<typename T>
using Ptr = ValueSummary<T*>;
using Int = ValueSummary<int>;
using Bool = ValueSummary<bool>;
#else
template<typename T>
using Ptr = T*;
using Int = int;
using Bool = bool;
#endif

};

#ifndef DECL_TYPES
#error "Must declare all usable before using PTypes! "
#endif

#include "PAny.hpp"
#include "PList.hpp"
#include "PTuple.hpp"
#include "PMap.hpp"
#include "PMachine.hpp"

#endif