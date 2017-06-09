#ifndef CROSSPRODUCT_HPP
#define CROSSPRODUCT_HPP

template <typename... T> struct cat2;

template <template<typename...> class R, typename... As, typename... Bs>
struct cat2 <R<As...>, R<Bs...> > {
        using type = R <As..., Bs...>;
};

template <typename... Ts> struct product_helper;

template <template<typename...> class R, typename... Ts>
struct product_helper < R<Ts...> > { // stop condition
        using type = R< Ts...>;
};

template <template<typename...> class R, typename... Ts>
struct product_helper < R<R<> >, Ts... > { // catches first empty tuple
        using type = R<>;
};

template <template<typename...> class R, typename... Ts, typename... Rests>
struct product_helper < R<Ts...>, R<>, Rests... > { // catches any empty tuple except first
        using type = R<>;
};

template <template<typename...> class R, typename... X, typename H, typename... Rests>
struct product_helper < R<X...>, R<H>, Rests... > {
        using type1 = R <typename cat2<X,R<H> >::type...>;
        using type  = typename product_helper<type1, Rests...>::type;
};

template <template<typename...> class R, typename... X, template<typename...> class Head, typename T, typename... Ts, typename... Rests>
struct product_helper < R<X...>, Head<T, Ts...>, Rests... > {
        using type1 = R <typename cat2<X,R<T> >::type...>;
        using type2 = typename product_helper<R<X...> , R<Ts...> >::type;
        using type3 = typename cat2<type1,type2>::type;
        using type  = typename product_helper<type3, Rests...>::type;
};

template <template<typename...> class R, typename... Ts> struct product;

template <template<typename...> class R>
struct product < R > { // no input, R specifies the return type
    using type = R<>;
};

template <template<typename...> class R, template<typename...> class Head, typename... Ts, typename... Tail>
struct product <R, Head<Ts...>, Tail... > { // R is the return type, Head<A...> is the first input list
    using type = typename product_helper< R<R<Ts>...>, Tail... >::type;
};

#endif