template<typename ...T> struct type_list;
template<typename T> struct wrap;
template<std::size_t I> using index = std::integral_constant<std::size_t, I>;
template<std::size_t ...I> using index_list = type_list<index<I>...>;

#ifdef __clang__
// Slightly more direct implementation is possible if using clang.
template<typename T, T ...N> using make_index_list_impl = index_list<N...>;
template<std::size_t N> using make_index_list = __make_integer_seq<make_index_list_impl, std::size_t, N>;
#else
template<typename T> struct to_index_list_impl;
template<std::size_t ...Is> struct to_index_list_impl<std::index_sequence<Is...>> {
  using type = index_list<Is...>;
};
template<std::size_t N> using make_index_list = typename to_index_list_impl<std::make_index_sequence<N>>::type;
#endif

template<typename Key, typename Value> struct meta_map_element { operator wrap<Key>(); };

template<typename T> struct extract_value;
template<typename Key, typename Value, typename M>
struct extract_value<M meta_map_element<Key, Value>::*> {
  using type = Value;
};

template<typename T, typename U> struct meta_map;
template<typename ...Ts, typename ...Us>
struct meta_map<type_list<Ts...>, type_list<Us...>> : meta_map_element<Ts, Us>... {
  using meta_map_element<Ts, Us>::operator wrap<Ts>...;
  // Extra template parameter works around a GCC bug.
  template<typename Key, typename Self = meta_map> using lookup =
    typename extract_value<decltype(&Self::operator wrap<Key>)>::type;
};

template<typename ...Ts> struct meta_pack {
  using map = meta_map<make_index_list<sizeof...(Ts)>, type_list<Ts...>>;
  template<std::size_t I> using at = typename map::template lookup<index<I>>;
};

template<typename T> struct remap;
template<std::size_t ...Is> struct remap<std::index_sequence<Is...>> {
  template<typename P> using apply_to_meta_pack = type_list<typename P::template at<Is>...>;
};

template<typename List, typename Shuffle> using ApplyShuffle =
  typename remap<Shuffle>::template apply_to_meta_pack<typename List::template apply<meta_pack>>;
