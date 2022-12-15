#include <type_traits>
#include <utility>

template<typename ...T> struct type_list {
  template<template<typename...> typename M> using apply = M<T...>;
};

#ifndef IMPL
#define IMPL "operator_t.h"
#endif

#include IMPL

#ifndef SIZE
#define SIZE 16
#endif
constexpr std::size_t N = 1 << SIZE;

template<typename T> struct indexes;
template<std::size_t ...Is> struct indexes<std::index_sequence<Is...>> {
  template<template<std::size_t...> typename M> using apply = M<Is...>;
};

// Build an initial sequence of elements.
template<std::size_t> struct element {};

template<std::size_t ...Is> struct make_elements {
  template<template<typename...> typename M> using apply = M<element<Is>...>;
};

template<std::size_t N> struct elements {
  template<template<typename ...> typename M> using apply =
    typename indexes<std::make_index_sequence<N>>
             ::template apply<make_elements>
             ::template apply<M>;
};

static_assert(std::is_same_v<elements<3>::apply<std::tuple>,
                             std::tuple<element<0>, element<1>, element<2>>>);

// Build a remapping.
constexpr std::size_t Mask = 0x55555555 & (N - 1);
template<std::size_t ...Is> using shuffled =
  std::index_sequence<Is ^ Mask...>;
template<std::size_t N> using shuffle =
  typename indexes<std::make_index_sequence<N>>
  ::template apply<shuffled>;

// Run the test.
using List = elements<N>;
using Shuffle = shuffle<N>;
using Result = ApplyShuffle<List, Shuffle>;
