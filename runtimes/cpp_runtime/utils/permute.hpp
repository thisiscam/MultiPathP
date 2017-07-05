#ifndef PERMUTE_HPP
#define PERMUTE_HPP

#include <iostream>
#include <type_traits>

template <typename, typename> struct Merge;

template <template <typename...> class P, typename... Ts, typename... Us>
struct Merge<P<Ts...>, P<Us...>> {
    using type = P<Ts..., Us...>;
};

template <std::size_t N, typename Pack, typename Previous, typename... Output> struct PermutationNHelper;

template <std::size_t N, template <typename...> class P, typename First, typename... Rest, typename... Prev, typename... Output>
struct PermutationNHelper<N, P<First, Rest...>, P<Prev...>, Output...> : Merge<
    typename PermutationNHelper<N-1, P<Prev..., Rest...>, P<>, Output..., First>::type,  // P<Prev..., Rest...> are the remaining elements, thus ensuring that the next element chosen will not be First.  The new Prev... is empty since we now start at the first element of P<Prev..., Rest...>.
    typename PermutationNHelper<N, P<Rest...>, P<Prev..., First>, Output...>::type  // Using P<Rest...> ensures that the next set of permutations will begin with the type after First, and thus the new Prev... is Prev..., First.
> {};

template <std::size_t N, template <typename...> class P, typename Previous, typename... Output>
struct PermutationNHelper<N, P<>, Previous, Output...> {
    using type = P<>;
};

template <template <typename...> class P, typename First, typename... Rest, typename... Prev, typename... Output>
struct PermutationNHelper<0, P<First, Rest...>, P<Prev...>, Output...> {  // P<Prev...> must be used (instead of simply 'Previous'), else there will be ambiguity.
    using type = P<P<Output...>>;  // Note that it must be P<P<Output...>> instead of P<Output...> for the merging to work as intended.
};

template <template <typename...> class P, typename Previous, typename... Output>
struct PermutationNHelper<0, P<>, Previous, Output...> {
    using type = P<P<Output...>>;
};

template <typename Pack> struct EmptyPack;

template <template <typename...> class P, typename... Ts>
struct EmptyPack<P<Ts...>> { using type = P<>; };

template <std::size_t N, typename Pack>
using PermutationN = typename PermutationNHelper<N, Pack, typename EmptyPack<Pack>::type>::type;

template <typename PackOfPacks> struct PackSize;

template <template <typename...> class P, typename... Ts>
struct PackSize<P<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

template <typename Pack>
using Permutation = PermutationN<PackSize<Pack>::value, Pack>;

#endif