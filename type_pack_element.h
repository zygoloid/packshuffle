template<typename T> struct remap;
template<std::size_t ...Is> struct remap<std::index_sequence<Is...>> {
  template<typename ...Ts> using apply = type_list<__type_pack_element<Is, Ts...>...>;
};

template<typename List, typename Shuffle> using ApplyShuffle =
  typename List::template apply<remap<Shuffle>::template apply>;
