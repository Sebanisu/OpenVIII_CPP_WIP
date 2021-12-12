//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_VERTICE_HPP
#define VIIIARCHIVE_VERTICE_HPP
#include "open_viii/Concepts.hpp"
namespace open_viii::graphics {
template<Number mainNumberT = std::int16_t>
struct Vertice
{
private:
  mainNumberT m_x{};
  mainNumberT m_y{};
  mainNumberT m_z{};

public:
  constexpr Vertice() = default;
  constexpr Vertice(mainNumberT in_x, mainNumberT in_y, mainNumberT in_z)
    : m_x(in_x), m_y(in_y), m_z(in_z)
  {}
  template<Number numberT>
  constexpr explicit Vertice(Vertice<numberT> lhs)
    : m_x(static_cast<mainNumberT>(lhs.x())),
      m_y(static_cast<mainNumberT>(lhs.y())),
      m_z(static_cast<mainNumberT>(lhs.z()))
  {}
  template<Number numberT>
  auto
    operator/(numberT lhs)
  {
    return Vertice<numberT>{ static_cast<numberT>(m_x) / lhs,
                             static_cast<numberT>(m_y) / lhs,
                             static_cast<numberT>(m_z) / lhs };
  }
  template<Number numberT>
  auto
    operator*(numberT lhs)
  {
    return Vertice<numberT>{ static_cast<numberT>(m_x) * lhs,
                             static_cast<numberT>(m_y) * lhs,
                             static_cast<numberT>(m_z) * lhs };
  }
  template<Number numberT>
  auto
    operator-(numberT lhs)
  {
    return Vertice<numberT>{ static_cast<numberT>(m_x) - lhs,
                             static_cast<numberT>(m_y) - lhs,
                             static_cast<numberT>(m_z) - lhs };
  }
  template<Number numberT>
  auto
    operator+(numberT lhs)
  {
    return Vertice<numberT>{ static_cast<numberT>(m_x) + lhs,
                             static_cast<numberT>(m_y) + lhs,
                             static_cast<numberT>(m_z) + lhs };
  }
  [[nodiscard]] constexpr auto
    x() const noexcept
  {
    return m_x;
  }
  [[nodiscard]] constexpr auto
    y() const noexcept
  {
    return m_y;
  }
  [[nodiscard]] constexpr auto
    z() const noexcept
  {
    return m_z;
  }
  [[nodiscard]] constexpr auto
    with_x(mainNumberT in_x) const noexcept
  {
    auto ret = *this;
    ret.m_x  = in_x;
    return ret;
  }
  [[nodiscard]] constexpr auto
    with_y(mainNumberT in_y) const noexcept
  {
    auto ret = *this;
    ret.m_y  = in_y;
    return ret;
  }
  [[nodiscard]] constexpr auto
    with_z(mainNumberT in_z) const noexcept
  {
    auto ret = *this;
    ret.m_z  = in_z;
    return ret;
  }
  template<typename T>
  [[nodiscard]] constexpr auto
    midpoint(const Vertice<T> other) const noexcept
  {
    return Vertice(
      std::midpoint(m_x, other.x()),
      std::midpoint(m_y, other.y()),
      std::midpoint(m_z, other.z()));
  }
};
template<typename mainNumberT>
inline std::ostream &
  operator<<(std::ostream &os, const Vertice<mainNumberT> &lhs)
{
  return os << '(' << lhs.x() << ", " << lhs.y() << ", " << lhs.z() << ')';
}
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_VERTICE_HPP
