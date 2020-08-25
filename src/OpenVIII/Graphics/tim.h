//
// Created by pcvii on 8/20/2020.
//

#ifndef VIIIARCHIVE_TIM_H
#define VIIIARCHIVE_TIM_H
#include <cstdint>
#include <array>
#include <string_view>
#include "OpenVIII/Tools/Tools.h"
#include <iostream>
namespace OpenVIII::Graphics {

/**
 * @enum OpenVIII::Graphics::BPPT
 * \ingroup Graphics
 * @brief Contains 4 to 24 bits per pixel flags. Also a CLP flag for color lookup table.
 * @note change to default initiated cpp20 style.
 */
struct BPPT
{
private:
  bool _8bpp_ : 1;
  bool _16bpp_ : 1;
  bool unused1 : 1;
  bool colorLookupTablePresent_ : 1;

  bool unused2 : 1;
  bool unused3 : 1;
  bool unused4 : 1;
  bool unused5 : 1;

public:
  BPPT()
    : _8bpp_{ false }, _16bpp_{ false }, unused1{ false },
      colorLookupTablePresent_{ false }, unused2{ false }, unused3{ false }, unused4{ false }, unused5{ false }
  {}
  [[nodiscard]] bool unused() const { return unused1 && unused2 && unused2 && unused3 && unused4 && unused5; }
  /**
   * Test bits to check if color lookup table is present and 8bpp and 16bpp are not set;
   * @return true if 4bpp
   */
  [[nodiscard]] bool _4bpp() const { return !unused() && !_8bpp_ && !_16bpp_ && colorLookupTablePresent_; }
  /**
   * Test bits to check if color lookup table is present and 8bpp is set and 16bpp is not set;
   * @return true if 8bpp
   */
  [[nodiscard]] bool _8bpp() const { return !unused() && _8bpp_ && !_16bpp_ && colorLookupTablePresent_; }

  /**
   * Test bits to check if color lookup table is not present and 8bpp is not set and 16bpp is set;
   * @return true if 16bpp
   */
  [[nodiscard]] bool _16bpp() const { return !unused() && !_8bpp_ && _16bpp_ && !colorLookupTablePresent_; }

  /**
   * Test bits to check if color lookup table is not present and 8bpp is set and 16bpp is set;
   * @return true if 24bpp
   */
  [[nodiscard]] bool _24bpp() const { return !unused() && _8bpp_ && _16bpp_ && !colorLookupTablePresent_; }
  /**
   * Test bits to check if color lookup table is present and 8bpp is ignored and 16bpp is not set;
   * @return true if 16bpp
   */
  [[nodiscard]] bool ColorLookupTablePresent() const { return !unused() && !_16bpp_ && colorLookupTablePresent_; }
  /**
   * Test that one of the valid states is true.
   * @return true if is a valid state.
   */
  [[nodiscard]] bool Check() const { return _4bpp() || _8bpp() || _16bpp() || _24bpp(); }
  [[nodiscard]] explicit operator bool() const { return Check(); }
  [[nodiscard]] explicit operator int() const
  {
    //[[maybe_unused]] static constexpr auto size_ = sizeof(BPPT);
    if (_4bpp()) {
      const int retVal = 4;
      return retVal;
    }
    if (_8bpp()) {
      const int retVal = 8;
      return retVal;
    }
    if (_16bpp()) {
      const int retVal = 16;
      return retVal;
    }
    if (_24bpp()) {
      const int retVal = 24;
      return retVal;
    }
    return 0;
  }
  //  _4bpp = 0b0U, /**< is coded as std::uint8_t of value 0. 4 bits per per pixel */
  //  _8bpp = 0b1U, /**< is coded as std::uint8_t of value 1. 8 bits per per pixel */
  //  _16bpp = 0b10U, /**< is coded as std::uint8_t of value 2. 16 bits per per pixel */
  //  _24bpp = _8bpp | _16bpp, /**< is coded as std::uint8_t of value 3. 24 bits per per pixel */
  //  CLP = 0b1000, /**< is coded as std::uint8_t of value 8. CLP flag for color lookup table present. This is required
  //  for
  //                   4bpp and 8bpp. */

  friend std::ostream &operator<<(std::ostream &os, const BPPT &input)
  {
    return os << "{BPP: " << static_cast<int>(input) << ", CLP: " << input.colorLookupTablePresent_ << '}';
  }
};
template<typename dimT> struct point
{
private:
  mutable dimT X_{};
  mutable dimT Y_{};

public:
  /**
   * X coordinate.
   * @return x
   */
  [[nodiscard]] auto X() const { return X_; }

  /**
   * Y coordinate.
   * @return y
   */
  [[nodiscard]] auto Y() const { return Y_; }

  /**
   *
   * @param x is new X coordinate.
   * @return x
   */
  [[nodiscard]] auto X(const dimT &x) const { return X_ = x; }

  /**
   *
   * @param y is new Y coordinate.
   * @return y
   */
  [[nodiscard]] auto Y(const dimT &y) const { return Y_ = y; }


  friend std::ostream &operator<<(std::ostream &os, const point<dimT> &input)
  {
    return os << '{' << input.X_ << ", " << input.Y_ << '}';
  }
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
  [[maybe_unused]] [[nodiscard]] auto Left() const { return TopLeft_.X(); }
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

  friend std::ostream &operator<<(std::ostream &os, const rectangle<dimT> &input)
  {
    return os << "{(X, Y) = " << input.TopLeft_ << ", (Width, Height) = " << input.WidthHeight_ << '}';
  }
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
           && version_ == versionVal_ && bpp_.Check();
  }

  friend std::ostream &operator<<(std::ostream &os, const timHeader &input)
  {
    //[[maybe_unused]]static constexpr auto size_ = sizeof(input);
    return os << "{ Tag: " << static_cast<uint32_t>(input.tag_) << ", Version: " << static_cast<uint32_t>(input.version_) << ", " << input.bpp_ << '}';
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

  friend std::ostream &operator<<(std::ostream &os, const timImageHeader &input)
  {
    //[[maybe_unused]]static constexpr auto sizev_ = sizeof(input);
    return os << '{' << input.size() << " bytes, " << input.rectangle_ << '}';
  }
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

  friend std::ostream &operator<<(std::ostream &os, const timClutHeader &input)
  {
    return os << input.imageHeader_ << " {Colors:" << input.imageHeader_.Rectangle().Width()
              << ", Tables: " << input.imageHeader_.Rectangle().Height() << '}';
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
    if (timHeader_.BPP().ColorLookupTablePresent()) {
      getValue(timClutHeader_);
      setData(timClutData_, timClutHeader_.dataSize());
    }
    getValue(timImageHeader_);
    setData(timImageData_, timImageHeader_.dataSize());
  }
  [[nodiscard]] bool Check() const
  {
    return timHeader_.Check() && (!timHeader_.BPP().Check() || timClutHeader_.Check());
  }
  [[nodiscard]] auto Width() const
  {
    static constexpr auto _4bpp_step{ 4 };
    static constexpr auto _8bpp_step{ 2 };
    static constexpr auto _24bpp_step{ 1.5 };
    if (timHeader_.BPP()._4bpp()) {
      return static_cast<decltype(timImageHeader_.Rectangle().Width())>(
        timImageHeader_.Rectangle().Width() * _4bpp_step);// 4pp
    }
    if (timHeader_.BPP()._8bpp()) {
      return static_cast<decltype(timImageHeader_.Rectangle().Width())>(
        timImageHeader_.Rectangle().Width() * _8bpp_step);// 8pp
    }
    if (timHeader_.BPP()._16bpp()) {
      return timImageHeader_.Rectangle().Width();// 16bpp
    }
    if (timHeader_.BPP()._24bpp()) {
      return static_cast<decltype(timImageHeader_.Rectangle().Width())>(
        timImageHeader_.Rectangle().Width() / _24bpp_step);// 24 bpp
    }
    return static_cast<decltype(timImageHeader_.Rectangle().Width())>(0);// invalid value
  }
  [[nodiscard]] auto Height() const { return timImageHeader_.Rectangle().Height(); }
  [[nodiscard]] auto X() const { return timImageHeader_.Rectangle().X(); }
  [[nodiscard]] auto Y() const { return timImageHeader_.Rectangle().Y(); }

  [[nodiscard]] auto ClutX() const { return timClutHeader_.Rectangle().X(); }
  [[nodiscard]] auto ClutY() const { return timClutHeader_.Rectangle().Y(); }
  [[nodiscard]] auto size() const { return sizeof(timHeader_) + timClutHeader_.size() + timImageHeader_.size(); }
  friend std::ostream &operator<<(std::ostream &os, const tim &input)
  {
    return os << '{' << input.timHeader_ << ", " << input.timClutHeader_ << ", " << input.timImageHeader_ << ", Corrected Width: " << input.Width() <<'}';
  }
};

#endif// VIIIARCHIVE_TIM_H
}