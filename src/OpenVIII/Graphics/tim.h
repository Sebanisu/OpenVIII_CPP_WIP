//
// Created by pcvii on 8/20/2020.
//

#ifndef VIIIARCHIVE_TIM_H
#define VIIIARCHIVE_TIM_H
#include <cstdint>
enum class BPPT : std::uint8_t {
  // 4 BPP
  // This is 0 so it will show as unset.
  _4bpp = 0b0U,

  // 8 BPP
  // if _8bpp and _16bpp are set then it's 24 bit
  _8bpp = 0b1U,

  // 16 BPP
  // if _8bpp and _16bpp are set then it's 24 bit
  _16bpp = 0b10U,

  // 24 BPP / not used in fields
  // Both flags must be set for this to be right

  _24bpp = _8bpp | _16bpp,


  // Color Lookup table Present / not used in fields. Fields have clut even if they aren't used.
  CLP = 0b1000,

};
// template<typename dimT>
// struct point
//{
//  dimT X{};
//  dimT Y{};
//};
template<typename dimT> struct rectangle
{
  dimT X{};
  dimT Y{};
  dimT Width{};
  dimT Height{};
};
struct timHeader
{
private:
  static constexpr auto tagVal_ = 0x10;
  static constexpr auto versionVal_ = 0x0;
  std::uint8_t tag_{};
  std::uint8_t version_{};
  std::uint8_t none0_{};
  std::uint8_t none1_{};
  BPPT bpp_{};
  std::uint8_t none2_{};
  std::uint8_t none3_{};
  std::uint8_t none4_{};

public:
  [[nodiscard]] auto BPP() const { return bpp_; }
  [[nodiscard]] bool Check() const
  {
    return none0_ == none1_ && none0_ == none2_ && none0_ == none3_ && none0_ == none4_ && none0_ == 0
             && tag_ == tagVal_ && version_ == versionVal_
             && (((static_cast<size_t>(bpp_) == (static_cast<size_t>(BPPT::_8bpp) | static_cast<size_t>(BPPT::CLP)))
                 || (bpp_ == BPPT::CLP))
           || ((static_cast<size_t>(bpp_) & static_cast<size_t>(BPPT::_24bpp)) > 0));
  }
};
struct timClutHeader
{
  static constexpr auto XdivisableBy_{ 16U };
  static constexpr auto MaxY_{ 511U };
  std::uint32_t ClutSize_{};
  rectangle<std::uint16_t> rectangle_{};
  // typically the width = number of colors,
  // and height = number of cluts
};
struct timImageHeader
{
  std::uint32_t ImageSize{};
  rectangle<std::uint16_t> rectangle_{};
};
struct tim
{
  timHeader timHeader_{};
  timClutHeader timClutHeader_{};
  timImageHeader timImageHeader_{};
  Width() { static_cast<std::size_t>(timHeader_.BPP()) | static_cast<std::size_t>() }
};

#endif// VIIIARCHIVE_TIM_H
