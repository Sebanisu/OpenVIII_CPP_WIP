//
// Created by pcvii on 2/17/2021.
//
#ifndef VIIIARCHIVE_MCH_HPP
#define VIIIARCHIVE_MCH_HPP
#include "Tim.hpp"
namespace open_viii::graphics {
struct Mch
{
private:
  std::vector<char>          m_buffer{};
  std::vector<std::uint32_t> m_tim_offsets{};
  std::uint32_t              m_model_offset{};
  decltype(m_tim_offsets)    get_tim_offsets() const
  {
    decltype(m_tim_offsets) ret{};
    std::span<const char>   buffer = m_buffer;
    do {
      if (std::ranges::size(buffer) < sizeof(std::uint32_t)) {
        break;
      }
      auto val = tools::read_val<std::uint32_t>(buffer);
      if (val > std::ranges::size(buffer)
          || val == std::numeric_limits<std::uint32_t>::max()) {
        break;
      }
      buffer = buffer.subspan(sizeof(std::uint32_t));
      ret.push_back(val);
    } while (true);
    return ret;
  }
public:
  std::uint32_t get_model_offset() const
  {
    std::span<const char> buffer = m_buffer;
    if (std::ranges::size(buffer) < sizeof(std::uint32_t) * 2) {
      return 0;
    }
    return tools::read_val<std::uint32_t>(buffer.subspan(
      sizeof(std::uint32_t) * (std::ranges::size(m_tim_offsets) + 1)));
  }
  Mch(std::vector<char> &&buffer)
    : m_buffer(std::move(buffer)),
      m_tim_offsets(get_tim_offsets()),
      m_model_offset(get_model_offset())
  {}
  void save(const std::string &path)
  {
    for (size_t i{}; const auto tim_offset : m_tim_offsets) {
      const auto tim_span = std::span<const char>(m_buffer).subspan(tim_offset);
      const auto tim      = Tim(tim_span);
      const auto out_path = path + std::to_string(i++);
      tim.save(out_path);
    }
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_MCH_HPP
