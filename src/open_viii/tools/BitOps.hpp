//
// Created by pcvii on 2/15/2021.
//
#ifndef VIIIARCHIVE_BITOPS_HPP
#define VIIIARCHIVE_BITOPS_HPP
#include <cstdint>
#include <limits>
namespace open_viii::tools {
static constexpr auto bits_per_byte = 8U;
/**
 * total number of bits for the given type.
 * @tparam measuredT is the type you want the bit count for.
 * @see https://godbolt.org/z/qdqeP3
 */
template<std::integral measuredT = std::uint64_t>
static constexpr std::size_t number_of_bits = bits_per_byte * sizeof(measuredT);
static_assert(number_of_bits<std::uint64_t> == 64U);
static_assert(number_of_bits<std::uint16_t> == 16U);
/**
 * Get max value per bits
 * @tparam retT return type.
 * @tparam lshift how many bits to shift left
 * @return as retT
 */
template<
  std::integral retT   = std::uint64_t,
  std::size_t   lshift = number_of_bits<retT> - 1>
static constexpr retT largest_bit_value = []() -> retT {
  std::size_t i;
  retT        ret{ 1U };
  if constexpr (number_of_bits<retT> < lshift) {
    i = number_of_bits<retT> - 1;
  }
  else {
    i = lshift - 1;
  }
  for (; i > 0; --i) {
    ret = static_cast<retT>(static_cast<retT>(ret << 1U) | 1U);
  }
  return ret;
}();
static_assert(largest_bit_value<std::uint64_t, 5U> == 0b11111U);
/**
 * flip_bits while keeping the type. unsigned char 0b1U becomes 0b1111'1110U
 * @param i
 * @see https://godbolt.org/z/qdqeP3
 */
template<std::integral retT = std::uint64_t>
static consteval retT
  flip_bits(retT i)
{
  if constexpr (std::signed_integral<retT>) {
    // i don't know if the signed version make since.
    using unsigned_retT = typename std::make_unsigned<retT>::type;
    return static_cast<retT>(flip_bits<unsigned_retT>(i));
  }
  else {
    constexpr auto max = (std::numeric_limits<retT>::max)();
    return static_cast<retT>(max - i);
  }
}
/**
 * Get a bitmask with smallest unsigned integral type.
 * @tparam bit_count number of bits in mask
 * @see https://godbolt.org/z/djG1fz
 * @note largest_bit_value is same as get_mask but it get_mask chooses the
 * return type.
 */
template<int bit_count>
static constexpr auto get_mask = []() {
  static_assert(bit_count <= number_of_bits<std::uint64_t> && bit_count > 0);
  constexpr auto bit_count_8  = number_of_bits<std::uint8_t>;
  constexpr auto bit_count_16 = number_of_bits<std::uint16_t>;
  constexpr auto bit_count_32 = number_of_bits<std::uint32_t>;
  if constexpr (bit_count <= 0) {
    return uint8_t(0U);
  }
  else if constexpr (bit_count >= 1 && bit_count <= bit_count_8) {
    return largest_bit_value<std::uint8_t, bit_count>;
  }
  else if constexpr (bit_count > bit_count_8 && bit_count <= bit_count_16) {
    return largest_bit_value<std::uint16_t, bit_count>;
  }
  else if constexpr (bit_count > bit_count_16 && bit_count <= bit_count_32) {
    return largest_bit_value<std::uint32_t, bit_count>;
  }
  else if constexpr (bit_count > bit_count_32 /* && bit_count <= 64*/) {
    return largest_bit_value<std::uint64_t, bit_count>;
  }
}();
static_assert(get_mask<5U> == 0b11111U);
static_assert(get_mask<16U> == 0b1111'1111'1111'1111U);
static_assert(flip_bits(get_mask<5U>) == 0b11100000U);
static_assert(flip_bits(get_mask<16U>) == 0);
}// namespace open_viii::tools
#endif// VIIIARCHIVE_BITOPS_HPP
