//
// Created by pcvii on 6/29/2021.
//

#ifndef OPENVIII_CPP_WIP_COLORLAYOUTT_HPP
#define OPENVIII_CPP_WIP_COLORLAYOUTT_HPP
namespace open_viii::graphics {
enum struct ColorLayoutT : std::uint8_t
{
  None  = 0,
  RGB   = 1U,
  BGR   = 1U << 1,
  POSTA = 1U << 2,
  PREA  = 1U << 3,
  RGBA  = POSTA | RGB,
  BGRA  = POSTA | BGR,
  ARGB  = PREA | RGB,
  ABGR  = PREA | BGR,
};

constexpr ColorLayoutT
  operator|(ColorLayoutT a, ColorLayoutT b)
{
  return static_cast<ColorLayoutT>(static_cast<int>(a) | static_cast<int>(b));
}
constexpr ColorLayoutT
  operator&(ColorLayoutT a, ColorLayoutT b)
{
  return static_cast<ColorLayoutT>(static_cast<int>(a) & static_cast<int>(b));
}
template<ColorLayoutT test, ColorLayoutT... flag>
constexpr bool
  has_flag()
{
  if constexpr (sizeof...(flag) != 0) {
    return (((flag & test) != ColorLayoutT{}) || ...);
  }
  else {
    return false;
  }
}
static_assert(has_flag<ColorLayoutT::RGB, ColorLayoutT::RGB>());
static_assert(has_flag<ColorLayoutT::BGR, ColorLayoutT::BGR>());
static_assert(has_flag<ColorLayoutT::PREA, ColorLayoutT::PREA>());
static_assert(has_flag<ColorLayoutT::POSTA, ColorLayoutT::POSTA>());
static_assert(!has_flag<ColorLayoutT::RGB, ColorLayoutT::BGR>());
static_assert(!has_flag<ColorLayoutT::BGR, ColorLayoutT::RGB>());
static_assert(!has_flag<ColorLayoutT::PREA, ColorLayoutT::POSTA>());
static_assert(!has_flag<ColorLayoutT::POSTA, ColorLayoutT::PREA>());

template<ColorLayoutT test, ColorLayoutT... flag>
consteval bool
  has_one_flag()
{
  return std::popcount(
           static_cast<std::underlying_type_t<ColorLayoutT>>(
             test & (flag | ...)))
      == 1;
}
static_assert(has_one_flag<ColorLayoutT::RGB, ColorLayoutT::RGB>());
static_assert(
  has_one_flag<ColorLayoutT::RGB, ColorLayoutT::RGB, ColorLayoutT::BGR>());
static_assert(!has_one_flag<
              ColorLayoutT::RGB | ColorLayoutT::BGR,
              ColorLayoutT::RGB,
              ColorLayoutT::BGR>());
template<
  ColorLayoutT layoutT,
  std::size_t  bgr,
  std::size_t  rgb,
  std::size_t  pre_alpha_offset>
consteval std::size_t
  get_index()
{
  if constexpr (has_flag<layoutT, ColorLayoutT::RGB>()) {
    if constexpr (has_flag<layoutT, ColorLayoutT::PREA>()) {
      return rgb + pre_alpha_offset;
    }
    else {
      return rgb;
    }
  }
  else if constexpr (has_flag<layoutT, ColorLayoutT::BGR>()) {
    if constexpr (has_flag<layoutT, ColorLayoutT::PREA>()) {
      return bgr + pre_alpha_offset;
    }
    else {
      return bgr;
    }
  }
}
template<ColorLayoutT layoutT, typename T, typename U>
constexpr auto
  get_index(T pre_alpha, U post_alpha)
{
  if constexpr (has_flag<layoutT, ColorLayoutT::PREA>()) {
    return pre_alpha;
  }
  else {
    return post_alpha;
  }
}

template<ColorLayoutT layoutT, typename T, typename U, typename V>
constexpr auto
  get_index(const T pre_alpha, const U post_alpha, const V no_alpha)
{
  if constexpr (has_flag<layoutT, ColorLayoutT::PREA, ColorLayoutT::POSTA>()) {
    return get_index<layoutT>(pre_alpha, post_alpha);
  }
  else {
    return no_alpha;
  }
}
}// namespace open_viii::graphics
#endif// OPENVIII_CPP_WIP_COLORLAYOUTT_HPP
