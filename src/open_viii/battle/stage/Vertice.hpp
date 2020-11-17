//
// Created by pcvii on 11/17/2020.
//

#ifndef VIIIARCHIVE_VERTICE_HPP
#define VIIIARCHIVE_VERTICE_HPP
#include "open_viii/Concepts.hpp"
#include <cstdint>
#include <ostream>
namespace open_viii::battle::stage {
template<Number mainNumberT = std::int16_t> struct Vertice
{
private:
  mainNumberT m_x{};
  mainNumberT m_y{};
  mainNumberT m_z{};

public:
  Vertice() = default;
  Vertice(mainNumberT in_x, mainNumberT in_y, mainNumberT in_z) : m_x(in_x), m_y(in_y), m_z(in_z) {}

  template<Number numberT>
  explicit Vertice(Vertice<numberT> lhs)
    : m_x(static_cast<mainNumberT>(lhs.x())),
      m_y(static_cast<mainNumberT>(lhs.y())),
      m_z(static_cast<mainNumberT>(lhs.z()))
  {}

  template<Number numberT> auto operator/(numberT lhs)
  {
    return Vertice<numberT>{
      static_cast<numberT>(m_x) / lhs, static_cast<numberT>(m_y) / lhs, static_cast<numberT>(m_z) / lhs
    };
  }

  template<Number numberT> auto operator*(numberT lhs)
  {
    return Vertice<numberT>{
      static_cast<numberT>(m_x) * lhs, static_cast<numberT>(m_y) * lhs, static_cast<numberT>(m_z) * lhs
    };
  }

  template<Number numberT> auto operator-(numberT lhs)
  {
    return Vertice<numberT>{
      static_cast<numberT>(m_x) - lhs, static_cast<numberT>(m_y) - lhs, static_cast<numberT>(m_z) - lhs
    };
  }

  template<Number numberT> auto operator+(numberT lhs)
  {
    return Vertice<numberT>{
      static_cast<numberT>(m_x) + lhs, static_cast<numberT>(m_y) + lhs, static_cast<numberT>(m_z) + lhs
    };
  }

  [[nodiscard]] auto x() const noexcept
  {
    return m_x;
  }
  [[nodiscard]] auto y() const noexcept
  {
    return m_y;
  }
  [[nodiscard]] auto z() const noexcept
  {
    return m_z;
  }

  void x(mainNumberT in_x) noexcept
  {
    m_x = in_x;
  }
  void y(mainNumberT in_y) noexcept
  {
    m_y = in_y;
  }
  void z(mainNumberT in_z) noexcept
  {
    m_z = in_z;
  }

  friend std::ostream &operator<<(std::ostream &os, const Vertice<mainNumberT> &lhs)
  {
    return os << '(' << lhs.x() << ", " << lhs.y() << ", " << lhs.z() << ')';
  }
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_VERTICE_HPP
