// No-op "shuffle" implementation.
template<typename List, typename Shuffle> using ApplyShuffle = typename List::template apply<type_list>;
