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

template<is_trivially_copyable_and_default_constructible trivialType,
  auto sizeOfType>
[[nodiscard]] static trivialType read_val(std::istream &fp)
{
  trivialType item{};
  std::array<char, sizeOfType> tmp{};
  fp.read(tmp.data(), tmp.size());
  if constexpr (requires { std::ranges::data(item); }) {

    memcpy(std::ranges::data(item),
      std::ranges::data(tmp),
      sizeof(*std::ranges::data(item)) * std::ranges::size(item));
  } else {
    memcpy(&item, std::ranges::data(tmp), sizeOfType);
  }
  return item;
}

template<is_trivially_copyable_and_default_constructible trivialType>
[[nodiscard]] static trivialType read_val(std::istream &fp)
{
  return read_val<trivialType, sizeof(trivialType)>(fp);
}

template<is_trivially_copyable trivialType>
static void read_val(std::istream &fp, trivialType &item)
{
  std::array<char, sizeof(item)> tmp{};
  fp.read(tmp.data(), tmp.size());

  memcpy(&item, std::ranges::data(tmp), sizeof(item));
}

template<typename trivialType>
requires(requires(trivialType item) {
  std::ranges::data(item);
}) static void read_val(std::istream &fp, trivialType &item)
{
  //  if constexpr( requires(trivialType
  //  t){std::is_same_v<char,decltype(*std::ranges::data(t))>;})
  //  {}
  // if container is type char could just write directly to it.
  const auto size = sizeof(*std::ranges::data(item)) * std::ranges::size(item);
  std::vector<char> tmp(size);

  fp.read(std::ranges::data(tmp), static_cast<long>(size));

  memcpy(std::ranges::data(item), std::ranges::data(tmp), size);
}

template<is_trivially_copyable trivialType>
static void read_val(const std::span<const char> &span, trivialType &item)
{
  memcpy(&item, std::ranges::data(span), sizeof(trivialType));
}
template<is_trivially_copyable_and_default_constructible trivialType>
[[nodiscard]] static trivialType read_val(
  const std::span<const char> &span, std::size_t offset = 0U)
{
  trivialType item{};
  read_val<trivialType>(span.subspan(offset), item);
  return item;
}


template<has_data_and_size trivialType>
requires std::ranges::contiguous_range<trivialType> &&
  has_resize<trivialType> static void
  read_val(
    const std::span<const char> &span, trivialType &item, std::size_t size)
{
  const auto element_size = sizeof(*std::ranges::data(item));
  // std::ranges::size(span) / element_size
  item.resize(size);
  if (size != 0) {
    memcpy(std::ranges::data(item),
      std::ranges::data(span),
      element_size * std::ranges::size(item));
  }
}

template<is_default_constructible_has_data_and_size trivialType>
[[nodiscard]] static trivialType read_val(
  const std::span<const char> &span, std::size_t size)
{
  trivialType item{};
  read_val(span, item, size);
  return item;
}
template<is_default_constructible_has_data_and_size trivialType>
[[nodiscard]] static trivialType read_val(const std::span<const char> &span)
{
  trivialType item();
  const auto element_size = sizeof(*std::ranges::data(item));
  read_val(span, item, std::ranges::size(span) / element_size);
  return item;
}
template<std::ranges::contiguous_range dstT = std::vector<char>,
  std::integral sizeT>
requires has_resize<dstT> [[maybe_unused]] static auto read_val(
  std::istream &fp, const sizeT &s)
{
  dstT buf{};
  if (s <= 0) {
    return buf;
  }
  buf.resize(static_cast<std::size_t>(s));
  fp.read(std::ranges::data(buf), s);
  return buf;
}


template<typename dstT = std::vector<char>>
[[maybe_unused]] static auto read_entire_stream(std::istream &fp)
{
  fp.seekg(0, std::ios::end);// seek to end;
  const auto size_of_file = fp.tellg();// read filesize
  fp.seekg(0, std::ios::beg);// seek to beginning
  return read_val<dstT>(fp, static_cast<long>(size_of_file));// read entire file
}

static std::optional<std::ifstream> open_file(const std::filesystem::path &path)
{
  std::optional<std::ifstream> ofp{};
  std::error_code ec{};
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
template<typename lambdaT>
requires(std::invocable<lambdaT, std::istream &>)
  [[maybe_unused]] static bool read_from_file(
    const lambdaT &lambda, const std::filesystem::path &path)
{
  auto ofp = open_file(path);
  if (ofp.has_value() && ofp->is_open()) {// check might be redundant.
    lambda(*ofp);
    ofp->close();
    return true;
  }
  return false;
}

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
 */
template<is_trivially_copyable_and_default_constructible valueT>
[[nodiscard]] static valueT read_value_from_file(
  const std::filesystem::path &path, const std::size_t &offset)
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
