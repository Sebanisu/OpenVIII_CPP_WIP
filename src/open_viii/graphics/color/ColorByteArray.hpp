//
// Created by pcvii on 7/6/2021.
//

#ifndef OPENVIII_CPP_WIP_COLORBYTEARRAY_HPP
#define OPENVIII_CPP_WIP_COLORBYTEARRAY_HPP
namespace open_viii::graphics {
template<std::size_t sizeT>
requires(sizeT == 3U || sizeT == 4U) struct ColorByteArray
{
protected:
  std::array<std::uint8_t, sizeT> value{};
  constexpr ColorByteArray() = default;
  template<typename... Ts>
  requires(
    sizeof...(Ts) == sizeT
    && (std::integral<std::decay_t<Ts>> && ...)) constexpr ColorByteArray(Ts
                                                                            &&...ts)
    : value{ static_cast<std::uint8_t>(ts)... }
  {}
  constexpr const std::uint8_t* data() const
  {
    return value.data();
  }
  template<typename T>
  constexpr std::uint8_t
    operator[](T &&index) const
  {
    return value[std::forward<T>(index)];
  }
  template<typename T, typename U>
  constexpr auto
    with(T &&index, U &&change) const noexcept
  {
    auto tmp                    = value;
    tmp[std::forward<T>(index)] = std::forward<U>(change);
    return tmp;
  }
  static constexpr bool         has_alpha     = sizeT == 4U ? true : false;
  static constexpr std::uint8_t alpha_size    = 8U;
  static constexpr std::size_t  EXPECTED_SIZE = sizeT;
};
}// namespace open_viii::graphics
#endif// OPENVIII_CPP_WIP_COLORBYTEARRAY_HPP
