//
// Created by pcvii on 1/15/2021.
//
#ifndef VIIIARCHIVE_MAKE_CONTAINER_HPP
#define VIIIARCHIVE_MAKE_CONTAINER_HPP
#include "open_viii/Concepts.hpp"
namespace open_viii::tools {
template<has_reserve containerT, typename fillT>
  requires requires(containerT c, fillT t) { c.emplace_back(t()); }
containerT
  make_container(const fillT &filler, size_t size)
{
  containerT data{};
  data.reserve(size);
  while (size-- != 0) {
    data.template emplace_back(filler());
  }
  return data;
}
template<has_reserve containerT, typename... restT>
containerT
  make_container(const restT &&...rest)
{
  containerT data{};
  data.reserve(sizeof...(rest));
  (data.template emplace_back(std::forward(rest)), ...);
  return data;
}
}// namespace open_viii::tools
#endif// VIIIARCHIVE_MAKE_CONTAINER_HPP
