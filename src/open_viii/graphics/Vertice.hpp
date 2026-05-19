//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_VERTICE_HPP
#define VIIIARCHIVE_VERTICE_HPP
#include "open_viii/Concepts.hpp"
#include <cstdint>
#include <fmt/format.h>
#include <numeric>
#include <utility>

namespace open_viii::graphics {

template<Number mainNumberT = std::int16_t>
struct Vertice
{
  // Public data
  mainNumberT x{};
  mainNumberT y{};
  mainNumberT z{};

  constexpr Vertice() = default;

  constexpr Vertice(mainNumberT in_x, mainNumberT in_y, mainNumberT in_z)
    : x(in_x), y(in_y), z(in_z)
  {}

  template<Number numberT>
  constexpr explicit Vertice(const Vertice<numberT> &lhs)
    : x(static_cast<mainNumberT>(lhs.x)), y(static_cast<mainNumberT>(lhs.y)),
      z(static_cast<mainNumberT>(lhs.z))
  {}

  template<Number numberT>
  [[nodiscard]] constexpr auto
    operator/(numberT lhs) const
  {
    return Vertice<numberT>{ static_cast<numberT>(x) / lhs,
                             static_cast<numberT>(y) / lhs,
                             static_cast<numberT>(z) / lhs };
  }

  template<Number numberT>
  [[nodiscard]] constexpr auto
    operator*(numberT lhs) const
  {
    return Vertice<numberT>{ static_cast<numberT>(x) * lhs,
                             static_cast<numberT>(y) * lhs,
                             static_cast<numberT>(z) * lhs };
  }

  template<Number numberT>
  [[nodiscard]] constexpr auto
    operator-(numberT lhs) const
  {
    return Vertice<numberT>{ static_cast<numberT>(x) - lhs,
                             static_cast<numberT>(y) - lhs,
                             static_cast<numberT>(z) - lhs };
  }

  template<Number numberT>
  [[nodiscard]] constexpr auto
    operator+(numberT lhs) const
  {
    return Vertice<numberT>{ static_cast<numberT>(x) + lhs,
                             static_cast<numberT>(y) + lhs,
                             static_cast<numberT>(z) + lhs };
  }

  [[nodiscard]] constexpr auto
    operator+(const Vertice &lhs) const
  {
    return Vertice{ static_cast<mainNumberT>(x + lhs.x),
                    static_cast<mainNumberT>(y + lhs.y),
                    static_cast<mainNumberT>(z + lhs.z) };
  }

  [[nodiscard]] constexpr Vertice
    with_x(mainNumberT v) const noexcept
  {
    auto ret = *this;
    ret.x    = v;
    return ret;
  }

  [[nodiscard]] constexpr Vertice
    with_y(mainNumberT v) const noexcept
  {
    auto ret = *this;
    ret.y    = v;
    return ret;
  }

  [[nodiscard]] constexpr Vertice
    with_z(mainNumberT v) const noexcept
  {
    auto ret = *this;
    ret.z    = v;
    return ret;
  }

  template<typename T>
  [[nodiscard]] constexpr auto
    midpoint(const Vertice<T> &other) const noexcept
  {
    return Vertice{ std::midpoint(x, other.x),
                    std::midpoint(y, other.y),
                    std::midpoint(z, other.z) };
  }
};

}// namespace open_viii::graphics

template<typename T>
struct fmt::formatter<open_viii::graphics::Vertice<T>>
{
  constexpr auto
    parse(format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const open_viii::graphics::Vertice<T> &v, FormatContext &ctx) const
  {
    return fmt::format_to(ctx.out(), "({}, {}, {})", v.x, v.y, v.z);
  }
};
#endif// VIIIARCHIVE_VERTICE_HPP
