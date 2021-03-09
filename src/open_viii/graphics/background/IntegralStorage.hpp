//
// Created by pcvii on 10/12/2020.
//
#ifndef VIIIARCHIVE_INTEGRALSTORAGE_HPP
#define VIIIARCHIVE_INTEGRALSTORAGE_HPP
#include <concepts>
#include <open_viii/graphics/BPPT.hpp>
namespace open_viii::graphics::background::integral_storage {
using namespace literals;
template<std::unsigned_integral intT> struct IntegralStorageCommon
{
private:
  constexpr static std::int8_t BITS_PER_LONG =
    static_cast<std::int8_t>(sizeof(intT) * 8);
  mutable intT        m_pupu_id{};
  mutable std::int8_t m_bits{ BITS_PER_LONG };
public:
  IntegralStorageCommon() = default;
  constexpr explicit IntegralStorageCommon(intT input_pupu_id)
    : m_pupu_id(input_pupu_id)
  {}
  bool seek(std::int8_t offset) const noexcept
  {
    const auto i = m_bits - offset;
    if (i > 0 || i <= BITS_PER_LONG) {
      m_bits = static_cast<std::int8_t>(i);
      return true;
    }
    return false;
  }
  void reset() const noexcept
  {
    m_bits = BITS_PER_LONG;
  }
  void write(const std::unsigned_integral auto &value)
  {
    m_pupu_id |= value << static_cast<intT>(m_bits);
  }
  intT read(const intT &mask) const noexcept
  {
    return (m_pupu_id >> static_cast<intT>(m_bits)) & mask;
  }
  bool empty() const noexcept
  {
    return m_pupu_id == 0U;
  }
  const intT &get_id() const noexcept
  {
    return m_pupu_id;
  }
};
template<std::unsigned_integral intT> struct Writer
{
private:
  mutable IntegralStorageCommon<intT> m_data{};
  void                                add_common(const intT         value,
                                                 const std::int8_t &shift,
                                                 const intT &       mask) const noexcept
  {
    if (m_data.seek(shift)) {
      m_data.write(value & mask);
    }
  }
public:
  Writer() = default;
  constexpr explicit Writer(intT input_pupu_id)
    : m_data(IntegralStorageCommon(input_pupu_id))
  {}
  [[nodiscard]] auto &data() const noexcept
  {
    return m_data;
  }
  [[maybe_unused]] [[nodiscard]] auto
    add_depth(const BPPT &depth) const noexcept
  {
    if (depth.bpp4()) {
      add_uint<2>(0U);
    } else if (depth.bpp8()) {
      add_uint<2>(1U);
    } else if (depth.bpp16()) {
      add_uint<2>(2U);
    } else if (depth.bpp24()) {
      add_uint<2>(3U);
    }
  }
  template<int bit_count, std::unsigned_integral input>
  void add_uint(const input &value) const noexcept
  {
    if constexpr (bit_count <= 0) {
    } else {
      static constexpr auto mask = tools::get_mask<bit_count>;
      add_common(value, bit_count, mask);
    }
  }
  const intT &get_id() const noexcept
  {
    return m_data.get_id();
  }
};
template<std::unsigned_integral intT> struct Reader
{
private:
  mutable IntegralStorageCommon<intT> m_data{};
  intT                                extract_common(const std::int8_t &  shift,
                                                     const std::uint64_t &mask) const noexcept
  {
    if (m_data.seek(shift)) {
      return (m_data.read(mask));
    }
    return 0U;
  }
public:
  Reader() = default;
  constexpr explicit Reader(intT input_pupu_id)
    : m_data(IntegralStorageCommon(input_pupu_id))
  {}
  [[nodiscard]] const auto &data() const noexcept
  {
    return m_data;
  }
  template<int bit_count> auto extract_uint() const noexcept
  {
    if constexpr (bit_count <= 0) {
      return nullptr;
    } else {
      static constexpr auto mask = tools::get_mask<bit_count>;
      if constexpr (bit_count == 1) {
        return extract_common(bit_count, mask) != 0U;
      } else {
        return static_cast<decltype(mask)>(extract_common(bit_count, mask));
      }
    }
  }
  [[maybe_unused]] [[nodiscard]] auto extract_depth() const noexcept
  {
    switch (extract_uint<2>()) {
    default: {
      static constexpr auto bpp = 4_bpp;
      return bpp;
    }
    case 1: {
      static constexpr auto bpp = 8_bpp;
      return bpp;
    }
    case 2: {
      static constexpr auto bpp = 16_bpp;
      return bpp;
    }
    case 3: {
      static constexpr auto bpp = 24_bpp;
      return bpp;
    }
    }
  }
  const intT &get_id() const noexcept
  {
    return m_data.get_id();
  }
};
}// namespace open_viii::graphics::background::integral_storage
#endif// VIIIARCHIVE_INTEGRALSTORAGE_HPP
