struct ignore_t { ignore_t(...); };
template<std::size_t> using ignore = ignore_t;
template<typename T> struct wrap { using type = T; };

template<typename T, typename...> struct meta_at_impl;
template<std::size_t ...Is, typename ...Ts>
struct meta_at_impl<std::index_sequence<Is...>, Ts...> {
    template<typename U, typename ...Us>
    static auto impl(ignore<Is>..., U t, Us ...us) {
        return *t;
    }
    using type = typename decltype(impl(static_cast<wrap<Ts>*>(nullptr)...))::type;
};

template<std::size_t N, typename ...Pack> using meta_at =
    typename meta_at_impl<std::make_index_sequence<N>, Pack...>::type;

template<typename> struct remap;
template<std::size_t ...Is> struct remap<std::index_sequence<Is...>> {
  template<typename ...Ts> using result = type_list<meta_at<Is, Ts...>...>;
};

template<typename List, typename Shuffle> using ApplyShuffle =
  typename List::template apply<remap<Shuffle>::template result>;
