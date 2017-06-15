#ifndef PTYPES_H
#define PTYPES_H

#include <type_traits>

namespace basic_cpp_runtime {

class PAny;
class PMachine;
template<typename T> class PList;
template<typename K, typename V> class PMap;
template<typename ...Ts> class PTuple;

/* Region Declare Types */
template<typename ...> struct List { };

};

#include "PAny.hpp"
#include "PList.hpp"
#include "PTuple.hpp"
#include "PMap.hpp"
#include "PMachine.hpp"

#endif