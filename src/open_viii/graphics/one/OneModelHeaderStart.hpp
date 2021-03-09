//
// Created by pcvii on 2/18/2021.
//
#ifndef VIIIARCHIVE_ONEMODELHEADERSTART_HPP
#define VIIIARCHIVE_ONEMODELHEADERSTART_HPP
namespace open_viii::graphics {
struct OneModelHeaderStart
{
private:
  std::uint32_t m_offset{};
  std::uint32_t m_size{};
  std::uint32_t m_size2{};
  std::uint32_t m_flag_dword{};

public:
  constexpr OneModelHeaderStart() = default;
  constexpr OneModelHeaderStart(std::uint32_t offset,
                                std::uint32_t size,
                                std::uint32_t size2,
                                std::uint32_t flag_dword)
    : m_offset(offset), m_size(size), m_size2(size2), m_flag_dword(flag_dword)
  {}
  constexpr OneModelHeaderStart(std::uint32_t offset,
                                std::uint32_t size,
                                std::uint32_t flag_dword)
    : OneModelHeaderStart(offset, size, size, flag_dword)
  {}
  constexpr std::uint32_t offset() const noexcept
  {
    return m_offset;
  }
  constexpr std::uint32_t size() const noexcept
  {
    return m_size;
  }
  constexpr std::uint32_t size2() const noexcept
  {
    return m_size2;
  }
  constexpr std::uint32_t flag_dword() const noexcept
  {
    return m_flag_dword;
  }
  constexpr std::uint32_t get_flag_from_dword() const noexcept
  {
    return ((m_flag_dword >> 24) & 0xFF);
  }
  constexpr bool is_main_chr() const noexcept
  {
    constexpr auto main_chr_flag = 0xD0U;
    return get_flag_from_dword() == main_chr_flag;
  }
  constexpr bool is_unknown_object() const noexcept
  {
    constexpr auto main_chr_flag = 0xA0U;
    return get_flag_from_dword() == main_chr_flag;
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_ONEMODELHEADERSTART_HPP
