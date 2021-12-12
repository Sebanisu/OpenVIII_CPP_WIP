//
// Created by pcvii on 10/30/2020.
//
#ifndef VIIIARCHIVE_FILESECTIONTYPET_HPP
#define VIIIARCHIVE_FILESECTIONTYPET_HPP
#include "open_viii/tools/Tools.hpp"
namespace open_viii::pak::FileSectionTypeT {
/**
 * None; no type defined.
 */
constexpr static auto NONE = std::string_view("");
/**
 * Cam file
 * @remark F8P
 */
constexpr static auto CAM  = std::string_view("F8P");
/**
 * Bink Video
 */
constexpr static auto BIK  = std::string_view("BIK");
/**
 * Bink Video Version 2.
 */
constexpr static auto KB2  = std::string_view("KB2");
/**
 * get_type 3 character array from stream.
 * @param is = input stream
 * @return array.
 */
template<bool rewind = false>
static auto
  get_type(std::istream &is)
{
  static constexpr auto sz           = 3U;
  const auto            get_type_int = [&is]() {
    return tools::read_val<std::array<char, sz>>(is);
  };
  if constexpr (rewind) {
    const auto ret = get_type_int();
    is.seekg(-static_cast<signed>(sz), std::ios::cur);
    return ret;
  }
  else {
    return get_type_int();
  }
}
/**
 * Compare needle with haystack and return true if valid.
 * @tparam needleT type of needle
 * @tparam haystackT type of haystack
 * @param needle range compatible with haystack used with equal algorithm
 * @param haystack range compatible with needle used with equal algorithm
 * @return true or false
 */
template<typename needleT, typename... haystackT>
constexpr static bool
  valid_type(const needleT &needle, const haystackT &...haystack)
{
  if constexpr (sizeof...(haystack) == 0) {
    return valid_type(needle, CAM, BIK, KB2);
  }
  else if constexpr (
    std::ranges::range<needleT> && (std::ranges::range<haystackT> && ...)) {
    return (std::ranges::equal(needle, haystack) || ...);
  }
  else {
    return ((needle == haystack) || ...);
  }
}
/**
 * Compare needle with haystack and return found value
 * @tparam needleT needle type
 * @tparam first_haystackT first haystack type
 * @tparam haystackT rest of haystack type
 * @param needle range compatible with haystack used with equal algorithm
 * @param first_haystack range compatible with needle used with equal algorithm
 * @param haystack range compatible with needle used with equal algorithm
 * @return returns the found value.
 */
template<typename needleT, typename first_haystackT, typename... haystackT>
constexpr static auto
  find_type(
    const needleT         &needle,
    const first_haystackT &first_haystack,
    const haystackT &...haystack) noexcept
{
  if (std::ranges::equal(needle, first_haystack)) {
    return first_haystack;
  }
  if constexpr (sizeof...(haystack) != 0) {
    return find_type(needle, haystack...);
  }
  else {
    return decltype(first_haystack){};
  }
}
/**
 * Compare needle with default values and return found value
 * @tparam needleT
 * @param needle range compatible with haystack used with equal algorithm
 * @return returns found value
 */
template<typename needleT>
constexpr static auto
  find_type(const needleT &needle) noexcept
{
  return find_type(needle, CAM, BIK, KB2);
}
static_assert(valid_type(CAM));
static_assert(valid_type(BIK));
static_assert(valid_type(KB2));
static_assert(valid_type(find_type(CAM)));
static_assert(valid_type(find_type(BIK)));
static_assert(valid_type(find_type(KB2)));
}// namespace open_viii::pak::FileSectionTypeT
#endif// VIIIARCHIVE_FILESECTIONTYPET_HPP
