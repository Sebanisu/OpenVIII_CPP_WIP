//
// Created by pcvii on 8/20/2020.
//

#ifndef VIIIARCHIVE_TIM_H
#define VIIIARCHIVE_TIM_H
#include <cstdint>
#include <array>
#include <string_view>
#include "OpenVIII/Tools/Tools.h"
namespace OpenVIII::Graphics {

/**
 * @enum OpenVIII::Graphics::BPPT
 * \ingroup Graphics
 * @brief Contains 4 to 24 bits per pixel flags. Also a CLP flag for color lookup table.
 */
enum class BPPT : std::uint8_t {
  _4bpp [[maybe_unused]] = 0b0U, /**< is coded as std::uint8_t of value 0. 4 bits per per pixel */
  _8bpp = 0b1U, /**< is coded as std::uint8_t of value 1. 8 bits per per pixel */
  _16bpp = 0b10U, /**< is coded as std::uint8_t of value 2. 16 bits per per pixel */
  _24bpp = _8bpp | _16bpp, /**< is coded as std::uint8_t of value 3. 24 bits per per pixel */
  CLP = 0b1000, /**< is coded as std::uint8_t of value 8. CLP flag for color lookup table present. This is required for
                   4bpp and 8bpp. */

};
template<typename dimT> struct point
{
private:
  mutable dimT X_{};
  mutable dimT Y_{};

public:
  [[nodiscard]] auto X() const { return X_; }
  [[nodiscard]] auto Y() const { return Y_; }

  /**
   *
   * @param x is new X coordinate.
   * @return
   */
  [[nodiscard]] auto X(const dimT &x) const { return X_ = x; }

  /**
   *
   * @param y is new Y coordinate.
   * @return
   */
  [[nodiscard]] auto Y(const dimT &y) const { return Y_ = y; }
};
/**
 * @struct OpenVIII::Graphics::rectangle
 * @tparam dimT Number type.
 * @brief 2D Container that holds top left height and width.
 */
template<typename dimT> struct rectangle
{
private:
  mutable point<dimT> TopLeft_{};
  mutable point<dimT> WidthHeight_{};

public:
  /**
   * @return Left coordinate.
   */
  [[nodiscard]] auto Left() const { return TopLeft_.X(); }
  /**
   * @return Left coordinate.
   */
  [[nodiscard]] auto X() const { return TopLeft_.X(); }

  /**
   *
   * @param x is new Left coordinate.
   * @return Left coordinate.
   */
  [[nodiscard]] auto X(const dimT &x) const { return TopLeft_.X(x); }
  /**
   *
   * @param x is new Left coordinate.
   * @return Left coordinate.
   */
  [[nodiscard]] auto Left(const dimT &x) const { return TopLeft_.X(x); }
  /**
   * @return Top coordinate.
   */
  [[nodiscard]] auto Top() const { return TopLeft_.Y(); }
  /**
   * @return Top coordinate.
   */
  [[nodiscard]] auto Y() const { return TopLeft_.Y(); }
  /**
   *
   * @param y is new Top coordinate.
   * @return Top coordinate.
   */
  [[nodiscard]] auto Y(const dimT &y) const { return TopLeft_.Y(y); }

  /**
   * @return Right coordinate.
   */
  [[nodiscard]] auto Right() const { return TopLeft_.X() + WidthHeight_.X(); }
  /**
   * @return Bottom coordinate.
   */
  [[nodiscard]] auto Bottom() const { return TopLeft_.Y() + WidthHeight_.Y(); }
  /**
   * @return Width.
   */
  [[nodiscard]] auto Width() const { return WidthHeight_.X(); }

  /**
   *
   * @param width is new Width value.
   * @return Width.
   */
  [[nodiscard]] auto Width(const dimT &width) const { return WidthHeight_.X(width); }
  /**
   * @return Height.
   */
  [[nodiscard]] auto Height() const { return WidthHeight_.Y(); }
  /**
   *
   * @param height is new Height value.
   * @return Height.
   */
  [[nodiscard]] auto Height(const dimT &height) const { return WidthHeight_.Y(height); }
};


/**
 * @struct OpenVIII::Graphics::timHeader
 * @ingroup Graphics
 * @brief Color Lookup Table Header
 */
struct timHeader
{
private:
  /**
   * @brief expected value of tag_ is 0x10.
   */
  static constexpr auto tagVal_ = 0x10;
  /**
   * @brief expected value of version_ is 0x0.
   */
  static constexpr auto versionVal_ = 0x0;
  /**
   * @brief tag_ is first byte of a TIM file.
   */
  std::uint8_t tag_{};
  /**
   * @brief version_ is second byte of a TIM file.
   */
  std::uint8_t version_{};

  /**
   * @brief value should be 0.
   */
  std::uint8_t none0_{};
  /**
   * @brief value should be 0.
   */
  std::uint8_t none1_{};

  /**
   * @brief bits per pixel flags and CLP flag
   */
  BPPT bpp_{};
  /**
   * @brief value should be 0.
   */
  std::uint8_t none2_{};
  /**
   * @brief value should be 0.
   */
  std::uint8_t none3_{};
  /**
   * @brief value should be 0.
   */
  std::uint8_t none4_{};

public:
  /**
   * @return bits per pixel flags and CLP flag
   */
  [[nodiscard]] auto BPP() const { return bpp_; }
  /**
   * All none values should be 0. version_ should be versionVal_. tag_ should be tagVal_. Only valid values for bpp are
   * 4bpp with CLP, 8bpp with CLP, 16bpp, or 24 bpp.
   * @brief Test member variables for valid values.
   * @return true if Check test passes.
   */
  [[nodiscard]] bool Check() const
  {
    return none0_ == 0 && none1_ == 0 && none2_ == 0 && none3_ == 0 && none4_ == 0 && tag_ == tagVal_
           && version_ == versionVal_
           && (((static_cast<size_t>(bpp_) == static_cast<size_t>(BPPT::_8bpp) + static_cast<size_t>(BPPT::CLP)))
               || (bpp_ == BPPT::CLP) || (bpp_ == BPPT::_24bpp) || (bpp_ == BPPT::_16bpp));
  }
};

struct timImageHeader
{
private:
  std::uint32_t size_{};
  rectangle<std::uint16_t> rectangle_{};

public:
  /**
   * Total Header and Data size
   * @brief size in bytes
   */
  auto size() const { return size_; }
  /**
   * Dimensions of data.
   */
  auto Rectangle() const { return rectangle_; }


  /**
   * Total size of Color Lookup Table data without header.
   * @brief Size in bytes.
   */
  [[nodiscard]] auto dataSize() const { return size_ - sizeof(timImageHeader); };
};
/**
 * @struct OpenVIII::Graphics::timClutHeader
 * @ingroup Graphics
 * @brief Color Lookup Table Header
 */
struct timClutHeader
{
private:
  /**
   * @brief X value must be divisible by 16
   */
  static constexpr auto XdivisableBy_{ 16U };
  /**
   * @brief Y max value is 511
   */
  static constexpr auto MaxY_{ 511U };

  /**
   * @brief Valid width values
   */
  static constexpr std::array ValidWidth_ = { 16U, 256U };

  timImageHeader imageHeader_{};

public:
  /**
   * Typically the width = number of colors, and height = number of color lookup tables. Sometimes if there is only 16
   * colors (4bpp) there is multiple groups of 16 in the table.
   * @brief Dimensions of the color lookup table.
   */
  [[nodiscard]] auto Rectangle() const { return imageHeader_.Rectangle(); }
  /**
   * Total size of Color Lookup Table including header.
   * @brief Size in bytes.
   */
  [[nodiscard]] auto size() const { return imageHeader_.size(); };

  /**
   * Total size of Color Lookup Table data without header.
   * @brief Size in bytes.
   */
  [[nodiscard]] auto dataSize() const { return imageHeader_.dataSize(); }
  /**
   * Test X and Y of rectangle to see if they are valid values.
   * Width is usually number of colors should be 16 or 256.
   * @return returns true if valid
   */
  [[nodiscard]] bool Check() const
  {
    return imageHeader_.Rectangle().X() % XdivisableBy_ == 0 && imageHeader_.Rectangle().Y() <= MaxY_
           && Tools::any_of(imageHeader_.Rectangle().Width(), ValidWidth_);
  }
};
struct tim
{
private:
  timHeader timHeader_{};
  timClutHeader timClutHeader_{};
  std::string_view timClutData_{};
  timImageHeader timImageHeader_{};
  std::string_view timImageData_{};

public:
  explicit tim(const std::string_view &buffer)
  {
    if (std::size(buffer) == 0) {
      return;
    }
    auto ptr_ = buffer.data();
    auto size_ = buffer.size();
    const auto setData = [&ptr_, &size_, &buffer](std::string_view &item, const auto &bytes) {
      item = std::string_view{ ptr_, bytes };
      ptr_ += bytes;
      size_ -= bytes;
    };
    const auto getValue = [&ptr_, &size_, &buffer](auto &item) {
      memcpy(&item, ptr_, sizeof(item));
      ptr_ += sizeof(item);
      size_ -= sizeof(item);
    };
    getValue(timHeader_);
    if ((static_cast<std::size_t>(timHeader_.BPP()) & static_cast<std::size_t>(BPPT::CLP)) > 0U) {
      getValue(timClutHeader_);
      setData(timClutData_, timClutHeader_.dataSize());
    }
    getValue(timImageHeader_);
    setData(timImageData_, timImageHeader_.dataSize());
  }
  [[nodiscard]] bool Check() const
  {
    return timHeader_.Check() && ((static_cast<size_t>(timHeader_.BPP()) & static_cast<size_t>(BPPT::CLP)) == 0U||timClutHeader_.Check())
  }
  [[nodiscard]] auto Width() const
  {
    static constexpr auto _4bpp_step{ 4 };
    static constexpr auto _8bpp_step{ 2 };
    static constexpr auto _24bpp_step{ 1.5 };
    if (timHeader_.BPP() == BPPT::CLP) {
      return static_cast<decltype(timImageHeader_.Rectangle().Width())>(
        timImageHeader_.Rectangle().Width() * _4bpp_step);// 4pp
    }
    if (((static_cast<size_t>(timHeader_.BPP())
          == static_cast<size_t>(BPPT::_8bpp) + static_cast<size_t>(BPPT::CLP)))) {
      return static_cast<decltype(timImageHeader_.Rectangle().Width())>(
        timImageHeader_.Rectangle().Width() * _8bpp_step);// 8pp
    }
    if (timHeader_.BPP() == BPPT::_24bpp) {
      return static_cast<decltype(timImageHeader_.Rectangle().Width())>(
        timImageHeader_.Rectangle().Width() / _24bpp_step);// 24 bpp
    }
    if (timHeader_.BPP() == BPPT::_16bpp) {
      return timImageHeader_.Rectangle().Width();// 16bpp
    }
    return static_cast<decltype(timImageHeader_.Rectangle().Width())>(0);// invalid value
  }
  [[nodiscard]] auto Height() const { return timImageHeader_.Rectangle().Height(); }
  [[nodiscard]] auto X() const { return timImageHeader_.Rectangle().X(); }
  [[nodiscard]] auto Y() const { return timImageHeader_.Rectangle().Y(); }

  [[nodiscard]] auto ClutX() const { return timClutHeader_.Rectangle().X(); }
  [[nodiscard]] auto ClutY() const { return timClutHeader_.Rectangle().Y(); }
};

#endif// VIIIARCHIVE_TIM_H
}