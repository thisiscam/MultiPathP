/*
* Macro Magic to detect if type is void
* See: https://stackoverflow.com/questions/11632219/c-preprocessor-macro-specialisation-based-on-an-argument
*/

#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define IIF(c) PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t

#define PROBE(x) x, 1 

#define CHECK(...) CHECK_N(__VA_ARGS__, 0)
#define CHECK_N(x, n, ...) n

#define ENABLE_TYPE_void ()

#define TYPE_ENABLED_PROBE(type)            TYPE_ENABLED_PROBE_PROXY( ENABLE_TYPE_##type ) // concatenate prefix with type name
#define TYPE_ENABLED_PROBE_PROXY(...)       TYPE_ENABLED_PROBE_PRIMIVIE(__VA_ARGS__)       // expand arguments
#define TYPE_ENABLED_PROBE_PRIMIVIE(x)      TYPE_ENABLED_PROBE_COMBINE_ x                  // merge
#define TYPE_ENABLED_PROBE_COMBINE_(...)    PROBE(~)                                       // if merge successful, expand to probe

#define IS_TYPE_ENABLED(type) CHECK(TYPE_ENABLED_PROBE(type))


#define IF_TYPE_MATCH_VOID(type, a, b) IIF( IS_TYPE_ENABLED(type) ) (a, b)
