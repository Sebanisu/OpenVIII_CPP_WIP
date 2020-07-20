// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
// todo This is an expermental file will go into it's own project later.
#ifndef VIIIARCHIVE_TESTCOLOR_H
#define VIIIARCHIVE_TESTCOLOR_H
#include <cstddef>
#include <iostream>
#include <bitset>
#include <ratio>
namespace OpenVIII::Color {

[[maybe_unused]] constexpr const static auto bit1 = 1U;
[[maybe_unused]] constexpr const static auto bit4 = 4U;
constexpr const static auto bit5 = 5U;
[[maybe_unused]] constexpr const static auto bit6 = 6U;
constexpr const static auto bit8 = 8U;
constexpr const static auto bit16 = 16U;

template<unsigned int base, unsigned int exponent> struct get_power
{// https://stackoverflow.com/a/27270738
  constexpr static const auto value = base * get_power<base, exponent - 1U>::value;
};
template<unsigned int base> struct get_power<base, 0>
{
  constexpr static const auto value = 1U;
};
// template<signed int number> struct get_abs
//{
//  constexpr static const auto value = static_cast<unsigned>(number < 0 ? 0 - number : number);
//};
template<size_t bits = bit8, size_t lshift_ = 0> struct colorPart
{
private:
  std::bitset<bits> value_{};

  template<intmax_t numAssignT, intmax_t denAssignT>
  constexpr void assign([[maybe_unused]] const std::ratio<numAssignT, denAssignT> c) noexcept
  {
    using t1 = std::ratio<MaxValue, 1>;
    using t2 = decltype(c);
    using v = std::ratio_multiply<t1, t2>;
    assign(static_cast<double>(v::num) / static_cast<double>(v::den));
  }
  template<size_t bitsAssignT, size_t lShiftAssignT>
  void assign(const colorPart<bitsAssignT, lShiftAssignT> &c) noexcept
  {
    return assign(
      c.dataF());// using floating point value which is value/maxvalue which is used to cast colors to other formats
  }
  constexpr colorPart &operator+(const colorPart &c)
  {
    constexpr auto comb = dataF() + c.dataF();
    if constexpr (comb > 1.0) {
      return 1.0;
    } else {
      return comb;
    }
  }
  constexpr colorPart &operator-(const colorPart &c)
  {
    constexpr auto comb = dataF() - c.dataF();
    if constexpr (comb < 0.0) {
      return 0.0;
    } else {
      return comb;
    }
  }
  constexpr colorPart &operator*(const colorPart &c) { return dataF() * c.dataF(); }
  constexpr colorPart &operator/(const colorPart &c)
  {
    constexpr auto v = c.dataF();
    if constexpr (v != 0.0) {
      return dataF() / c.dataF();
    }
    return 0.0;
  }
  template<typename T, typename = std::enable_if<std::is_integral_v<T> || std::is_floating_point_v<T>>>
  void assign(T c) noexcept
  {
    {
      if constexpr (std::is_integral_v<T>) {
        value_ = (static_cast<unsigned>(c) >> rshift) & MaxValue;// precision loss here
        // TODO fix this. I think it's not correct.
      } else if constexpr (std::is_floating_point_v<T>) {

        if (c > 1.0) {
          c = 1.0;
        } else if (c < 0) {
          c = 0;
        }
        value_ = static_cast<unsigned long long>(c * MaxValue);
      }
    }
  }

public:
  constexpr const static auto lshift = lshift_;
  constexpr const static size_t rshift{ (bits < bit8)                   ? bit8 - bits
                                        : (bits > bit8 && bits < bit16) ? bit16 - bits
                                                                        : 0U };
  constexpr colorPart() = default;

  constexpr const static auto MaxValue = get_power<2, bits>::value - 1;
  template<typename T, typename = std::enable_if<std::is_integral_v<T> || std::is_floating_point_v<T>>>
  colorPart(T c) noexcept
  {
    assign(c);
  }

  template<size_t bitsAssignT, size_t lShiftAssignT> colorPart(const colorPart<bitsAssignT, lShiftAssignT> &c) noexcept
  {
    assign(c);
  }
  template<intmax_t numAssignT, intmax_t denAssignT>
  constexpr colorPart(const std::ratio<numAssignT, denAssignT> c) noexcept
  {
    assign(c);
  }


  constexpr const static auto topEnd = bits - rshift;

  [[nodiscard]] constexpr auto dataF() const noexcept
  {
    // ratio wouldn't take to_ulong() because it is not constexpr :(
    return static_cast<double>(value_.to_ulong()) / MaxValue;
  }
  [[nodiscard]] constexpr auto data() const noexcept
  {
    {
      if constexpr (topEnd > 0) {
        return value_.to_ulong() << rshift | (value_ >> topEnd).to_ulong();
        // shift count will be greater than width.
        // adding the extra bits expands the color range
      }
    }
    return value_.to_ulong();
  }
  [[nodiscard]] constexpr auto value() const noexcept
  {
    // static_assert(bits / bit8 <= sizeof(baseT), "baseType too small for number of bits");

    return value_.to_ulong();
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto shift() const noexcept
  {
    if constexpr (lshift != 0) {
      return value_.to_ulong() << lshift;
    }

    return value_.to_ulong();
  }
};
template<typename red = colorPart<bit8, bit8 * 3>,
  typename green = colorPart<bit8, bit8 * 2>,
  typename blue = colorPart<bit8, bit8>,
  typename alpha = colorPart<bit8, 0>>
struct color
{
private:
  red r_{};
  green g_{};
  blue b_{};
  alpha a_{};

public:
  [[maybe_unused]] [[nodiscard]] auto constexpr R() const noexcept { return r_; }
  [[maybe_unused]] auto constexpr R(const red r) noexcept { return r_ = r; }
  [[maybe_unused]] [[nodiscard]] auto constexpr G() const noexcept { return g_; }
  [[maybe_unused]] auto constexpr G(const green g) noexcept { return g_ = g; }
  [[maybe_unused]] [[nodiscard]] constexpr auto B() const noexcept { return b_; }
  [[maybe_unused]] auto constexpr B(const blue b) noexcept { return b_ = b; }
  [[maybe_unused]] [[nodiscard]] constexpr auto A() const noexcept { return a_; }
  [[maybe_unused]] constexpr auto A(const alpha a) noexcept { return g_ = a; }
  // operator= ();
  color() = default;
  color(color &) noexcept = default;
  color(color &&) noexcept = default;
  ~color() noexcept = default;
  color &operator=(const color &) noexcept = default;
  color &operator=(color &&) noexcept = default;
  template<typename cT = color> color &operator=(const cT &v) noexcept
  {
    r_ = v.R();
    g_ = v.G();
    b_ = v.B();
    a_ = v.A();
    return *this;
  }
  color(const red &r, const green &g, const blue &b, const alpha &a) noexcept
  {
    r_ = r;
    g_ = g;
    b_ = b;
    if constexpr (!std::is_null_pointer_v<alpha>) {
      a_ = a;
    }
  }
  color(const red &r, const green &g, const blue &b) noexcept
  {
    r_ = r;
    g_ = g;
    b_ = b;
    if constexpr (!std::is_null_pointer_v<alpha>) {
      a_ = 1.0;
    }
  }
  [[nodiscard]] constexpr auto value() const noexcept
  {
    if constexpr (!std::is_null_pointer_v<alpha>) {
      return r_.shift() | g_.shift() | b_.shift() | a_.shift();
    }
    return r_.shift() | g_.shift() | b_.shift();
  }
};

using ABGR1555 = color<colorPart<bit5, 0>, colorPart<bit5, bit5>, colorPart<bit5, bit5 * 2>, colorPart<1, bit5 * 3>>;
using RGB24bit = color<colorPart<bit8, bit8 * 2>, colorPart<bit8, bit8>, colorPart<bit8, 0>, std::nullptr_t>;
using RGBA60bit =
  color<colorPart<bit16, bit16 * 3>, colorPart<bit16, bit16 * 2>, colorPart<bit16, bit16>, colorPart<bit16, 0>>;
// int main()
//{
//
//  // auto c = color();
//
//  auto c = RGB24bit(1.0,1.0,1.0);
//  auto output = [](const auto &c) {
//         std::cout << "\nred\n";
//         std::cout << static_cast<int>(c.R().lshift) << std::endl;
//         std::cout << static_cast<int>(c.R().rshift) << std::endl;
//         std::cout << static_cast<int>(c.R().value()) << std::endl;
//         std::cout << static_cast<int>(c.R().data()) << std::endl;
//         std::cout << std::hex << static_cast<size_t>(c.R().shift()) << std::dec << std::endl;
//
//         std::cout << "\ngreen\n";
//         std::cout << static_cast<int>(c.G().lshift) << std::endl;
//         std::cout << static_cast<int>(c.G().rshift) << std::endl;
//         std::cout << static_cast<int>(c.G().value()) << std::endl;
//         std::cout << static_cast<int>(c.G().data()) << std::endl;
//         std::cout << std::hex << static_cast<size_t>(c.G().shift()) << std::dec << std::endl;
//
//         std::cout << "\nblue\n";
//         std::cout << static_cast<int>(c.B().lshift) << std::endl;
//         std::cout << static_cast<int>(c.B().rshift) << std::endl;
//         std::cout << static_cast<int>(c.B().value()) << std::endl;
//         std::cout << static_cast<int>(c.B().data()) << std::endl;
//         std::cout << std::hex << static_cast<size_t>(c.B().shift()) << std::dec << std::endl;
//
//         if constexpr (!std::is_null_pointer_v<decltype(c.A())>)
//         {
//           std::cout << "\nalpha\n";
//           std::cout << static_cast<int>(c.A().lshift) << std::endl;
//           std::cout << static_cast<int>(c.A().rshift) << std::endl;
//           std::cout << static_cast<int>(c.A().value()) << std::endl;
//           std::cout << static_cast<int>(c.A().data()) << std::endl;
//           std::cout << std::hex << static_cast<size_t>(c.A().shift()) << std::dec << std::endl;
//         }
//         std::cout << "\nhex of color\n";
//         std::cout << std::hex << static_cast<size_t>(c.value()) << std::dec << std::endl;
//
//  };
//  output(c);
//  auto d = color();
//  auto h = RGBA60bit();
//  d = c;
//  h = c;
//  // d.r=1.0f;
//  // c = d;
//  std::cout << std::endl;
//  output(d);
//  std::cout << std::endl;
//  output(h);
//}
}// namespace OpenVIII::Color
#endif// VIIIARCHIVE_TESTCOLOR_H
