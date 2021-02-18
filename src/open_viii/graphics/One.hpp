//
// Created by pcvii on 2/18/2021.
//
#ifndef VIIIARCHIVE_ONE_HPP
#define VIIIARCHIVE_ONE_HPP
#include "Mch.hpp"
#include "one/OneModel.hpp"
namespace open_viii::graphics {
struct One
{
private:
  std::vector<char>          m_buffer{};
  std::uint32_t m_count{};
public:
  One(std::vector<char> &&buffer)
    : m_buffer(std::move(buffer)),
      m_count(tools::read_val<std::uint32_t>(m_buffer))
  {}
  void save([[maybe_unused]] const std::string &path)
  {
//    for (size_t i{}; const auto tim_offset : m_tim_offsets) {
//      const auto tim_span = std::span<const char>(m_buffer).subspan(tim_offset);
//      const auto tim      = Tim(tim_span);
//      const auto out_path = path + std::to_string(i++);
//      tim.save(out_path);
//    }
  }
};
}
#endif// VIIIARCHIVE_ONE_HPP
