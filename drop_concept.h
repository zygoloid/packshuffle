template<typename, std::size_t> concept ignore = true;
template<typename T> struct wrap { using type = T; };

template<typename T> struct meta_at_impl;
template<std::size_t ...Is>
struct meta_at_impl<std::index_sequence<Is...>> {
  // TODO: Clang rejects this valid code.
  template<ignore<Is>..., typename T, typename...> using apply = T;
};

template<std::size_t N, typename ...Pack> using meta_at =
    typename meta_at_impl<std::make_index_sequence<N>>::template apply<Pack...>;

template<typename> struct remap;
template<std::size_t ...Is> struct remap<std::index_sequence<Is...>> {
  template<typename ...Ts> using result = type_list<meta_at<Is, Ts...>...>;
};

template<typename List, typename Shuffle> using ApplyShuffle =
  typename List::template apply<remap<Shuffle>::template result>;
