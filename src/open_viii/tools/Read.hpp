//
// Created by pcvii on 1/11/2021.
//
#ifndef VIIIARCHIVE_READ_HPP
#define VIIIARCHIVE_READ_HPP
#include "open_viii/Concepts.hpp"
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <string>
#include <thread>
#include <vector>
namespace open_viii::tools {
// TODO anything that requires memcpy could maybe be replaced with std::bitcast
// then the functions could be changes to constexpr.
/**
 *
 * @tparam fixed_size_rangeT
 * @tparam sizeOfType
 * @param fp
 * @return
 * @todo test?
 * @todo remove or refactor. Does this function even work in all cases?
 */
template<is_trivially_copyable_and_default_constructible fixed_size_rangeT,
         std::integral auto                              sizeOfType>
requires(sizeOfType > 0
         && (has_data_and_size<fixed_size_rangeT>)&&(
           !has_reserve<fixed_size_rangeT>)&&(!has_resize<fixed_size_rangeT>))
  [[nodiscard]] static fixed_size_rangeT read_val2(std::istream &fp)
{
  fixed_size_rangeT            item{};
  std::array<char, sizeOfType> tmp{};
  fp.read(tmp.data(), tmp.size());
  // todo umm shouldn't i be checking the size of item?
  // this would be okay for std::array because the size is part of the type.
  // but vector or string wouldn't be good because you would need to manually
  // set size.
  assert(std::ranges::size(item) > 0);
  memcpy(std::ranges::data(item),
         std::ranges::data(tmp),
         sizeof(decltype(*std::ranges::data(item))) * std::ranges::size(item));
  return item;
}
/**
 *
 * @tparam trivialType
 * @param fp
 * @param item
 * @todo test?
 */
template<is_trivially_copyable trivialType>
static void read_val(std::istream &fp, trivialType &item)
{
  std::array<char, sizeof(item)> tmp{};
  fp.read(tmp.data(), tmp.size());
  memcpy(&item, std::ranges::data(tmp), sizeof(item));
}
/**
 *
 * @tparam trivialType
 * @param fp
 * @param item
 * @todo test?
 */
template<has_data_and_size trivialType>
static void read_val(std::istream &fp, trivialType &item)
{
  //  if constexpr( requires(trivialType
  //  t){std::is_same_v<char,decltype(*std::ranges::data(t))>;})
  //  {}
  // if container is type char could just write directly to it.
  const auto size =
    sizeof(decltype(*std::ranges::data(item))) * std::ranges::size(item);
  std::vector<char> tmp(size);
  fp.read(std::ranges::data(tmp), static_cast<long>(size));
  memcpy(std::ranges::data(item), std::ranges::data(tmp), size);
}
/**
 *
 * @tparam trivialType
 * @param span
 * @param item
 * @todo test?
 */
template<is_trivially_copyable trivialType>
static void read_val(const std::span<const char> &span, trivialType &item)
{
  memcpy(&item, std::ranges::data(span), sizeof(trivialType));
}
/**
 *
 * @tparam trivialType
 * @param span
 * @return
 * @todo test?
 */
template<is_trivially_copyable_and_default_constructible trivialType>
[[nodiscard]] static trivialType read_val(const std::span<const char> &span)
{
  trivialType item{};
  read_val<trivialType>(span, item);
  return item;
}
/**
 *
 * @tparam trivialT
 * @tparam sizeOfType
 * @param fp
 * @return
 * @todo test?
 */
template<is_trivially_copyable_and_default_constructible trivialT,
         std::integral auto                              sizeOfType>
requires(sizeOfType > 0) [[nodiscard]] static trivialT
  read_val(std::istream &fp)
{
  std::array<char, sizeOfType> tmp{};
  fp.read(tmp.data(), tmp.size());
  return read_val<trivialT>(tmp);
}
/**
 *
 * @tparam trivialType
 * @param fp
 * @return
 * @todo test?
 */
template<is_trivially_copyable_and_default_constructible trivialType>
[[nodiscard]] static trivialType read_val(std::istream &fp)
{
  return read_val<trivialType, sizeof(trivialType)>(fp);
}
/**
 *
 * @tparam trivialT_or_rangeT
 * @tparam sizeOfType
 * @param fp
 * @return
 * @todo test?
 */
template<is_trivially_copyable_and_default_constructible trivialT_or_rangeT,
  std::integral auto                              sizeOfType>
requires(sizeOfType > 0) [[nodiscard]] static trivialT_or_rangeT
safe_read_val(std::istream &fp)
{
  const auto current_pos = fp.tellg();
  fp.seekg(0, std::ios::end);
  const auto end_pos  = fp.tellg();
  const auto max_size = std::abs(end_pos - current_pos);
  fp.seekg(current_pos, std::ios::beg);
  if (sizeOfType >= max_size) {
    return read_val<trivialT_or_rangeT, sizeOfType>();
  }
  return trivialT_or_rangeT();
}
/**
 *
 * @tparam trivialType
 * @param span
 * @param offset
 * @return
 * @todo test?
 */
template<is_trivially_copyable_and_default_constructible trivialType>
[[nodiscard]] static trivialType read_val(const std::span<const char> &span,
                                          std::size_t                  offset)
{
  return read_val<trivialType>(span.subspan(offset));
}
/**
 *
 * @tparam trivialType
 * @param span
 * @param item
 * @param size
 * @todo test?
 */
template<has_data_and_size trivialType>
requires std::ranges::contiguous_range<trivialType> &&
  has_resize<trivialType> static void
  read_val(const std::span<const char> &span,
           trivialType &                item,
           std::size_t                  size)
{
  const auto element_size = sizeof(decltype(*std::ranges::data(item)));
  // std::ranges::size(span) / element_size
  item.resize(size);
  if (size != 0) {
    memcpy(std::ranges::data(item),
           std::ranges::data(span),
           element_size * std::ranges::size(item));
  }
}
/**
 *
 * @tparam trivialType
 * @param span
 * @param size
 * @return
 * @todo test?
 */
template<is_default_constructible_has_data_and_size trivialType>
[[nodiscard]] static trivialType read_val(const std::span<const char> &span,
                                          std::size_t                  size)
{
  trivialType item{};
  read_val(span, item, size);
  return item;
}
/**
 *
 * @tparam rangeT
 * @param span
 * @return
 * @todo test?
 */
template<is_default_constructible_has_data_and_size rangeT>
[[nodiscard]] static rangeT read_val(const std::span<const char> &span)
{
  rangeT     item{};
  constexpr auto element_size = sizeof(decltype(*std::ranges::data(item)));
  read_val(span, item, std::ranges::size(span) / element_size);
  return item;
}
/**
 * Safe version of read_val<rangeT>(span). Check that the span can contain at
 * least 1 element.
 * @tparam rangeT Range value.
 * @param span const char buffer.
 * @return returns empty range if not safe. Else return value from unsafe
 * version.
 * @todo test?
 */
template<is_default_constructible_has_data_and_size rangeT>
[[nodiscard]] static rangeT safe_read_val(const std::span<const char> &span)
{
  if (std::ranges::size(span)
      < sizeof(decltype(*std::ranges::data(rangeT())))) {
    return rangeT();
  }
  return read_val<rangeT>(span);
}
/**
 *
 * @param fp
 * @param s
 * @return
 * @todo test?
 */
template<std::ranges::contiguous_range dstT = std::vector<char>,
         std::integral                 sizeT>
requires has_resize<dstT> [[maybe_unused]] static auto
  read_val(std::istream &fp, const sizeT &s)
{
  dstT buf{};
  if (s <= 0) {
    return buf;
  }
  buf.resize(static_cast<std::size_t>(s));
  fp.read(std::ranges::data(buf), s);
  return buf;
}
/**
 *
 * @tparam dstT
 * @param fp
 * @return
 * @todo test?
 */
template<typename dstT = std::vector<char>>
[[maybe_unused]] static auto read_entire_stream(std::istream &fp)
{
  fp.seekg(0, std::ios::end);          // seek to end;
  const auto size_of_file = fp.tellg();// read filesize
  fp.seekg(0, std::ios::beg);          // seek to beginning
  return read_val<dstT>(fp, static_cast<long>(size_of_file));// read entire file
}
/**
 *
 * @param path
 * @return
 * @todo test?
 */
static std::optional<std::ifstream> open_file(const std::filesystem::path &path)
{
  std::optional<std::ifstream> ofp{};
  std::error_code              ec{};
  if (!std::filesystem::exists(path, ec)) {
    return ofp;
  }
  if (ec) {
    std::cerr << ec.message() << std::endl;
    ec.clear();
    return ofp;
  }
  ofp.emplace(std::ifstream{});
  for (;;) {
    ofp->open(path, std::ios::in | std::ios::binary);
    if (ofp->is_open()) {
      break;
    }
    // TODO might need a better way to open a file than an infinite loop
    // I had issue where it'd fail to open a file sometimes but the
    // next time it'd work fine (╯°□°)╯︵ ┻━┻
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  std::cout << (std::string("Loading: \t\"") + path.string()
                + std::string("\"\n"));
  return ofp;
}
/**
 *
 * @tparam lambdaT
 * @param lambda
 * @param path
 * @return
 * @todo test?
 */
template<typename lambdaT>
requires(std::invocable<lambdaT, std::istream &>)
  [[maybe_unused]] static bool read_from_file(const lambdaT &lambda,
                                              const std::filesystem::path &path)
{
  auto ofp = open_file(path);
  if (ofp.has_value() && ofp->is_open()) {// check might be redundant.
    lambda(*ofp);
    ofp->close();
    return true;
  }
  return false;
}
/**
 *
 * @param path
 * @return
 * @todo test?
 */
template<std::ranges::contiguous_range dstT = std::vector<char>>
[[maybe_unused]] static dstT read_entire_file(const std::filesystem::path &path)
{
  dstT buffer{};
  read_from_file(
    [&buffer](std::istream &fp) {
      buffer = read_entire_stream<dstT>(fp);
    },
    path);
  return buffer;
}
/**
 * Read value from file
 * @tparam valueT type of return value
 * @param path path to file.
 * @param offset byte offset from beginning
 * @return
 * @todo test?
 */
template<is_trivially_copyable_and_default_constructible valueT>
[[nodiscard]] static valueT
  read_value_from_file(const std::filesystem::path &path,
                       const std::size_t &          offset)
{
  valueT item{};
  if (!read_from_file(
        [&offset, &item](std::istream &is) {
          is.seekg(static_cast<long>(offset));
          read_val(is, item);
        },
        path)) {
    // failed to read;
  }
  return item;
}
}// namespace open_viii::tools
#endif// VIIIARCHIVE_READ_HPP
