#include "boost/mp11/list.hpp"
#include "boost/mp11/algorithm.hpp"

template<typename T> struct remap;
template<std::size_t ...Is> struct remap<std::index_sequence<Is...>> {
  template<typename P> using apply_to_mp_list = type_list<boost::mp11::mp_at_c<P, Is>...>;
};

template<typename List, typename Shuffle> using ApplyShuffle =
  typename remap<Shuffle>::template apply_to_mp_list<typename List::template apply<boost::mp11::mp_list>>;
