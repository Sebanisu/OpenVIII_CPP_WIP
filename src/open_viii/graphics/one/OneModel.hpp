//
// Created by pcvii on 2/18/2021.
//
#ifndef VIIIARCHIVE_ONEMODEL_HPP
#define VIIIARCHIVE_ONEMODEL_HPP
#include "OneModelHeaderStart.hpp"
namespace open_viii::graphics {
struct OneModel
{
  OneModelHeaderStart        m_model_header_start{};
  std::vector<std::uint32_t> m_tim_offsets{};
  OneModelHeaderStart
    get_model_header_start(std::span<const char> &buffer) const
  {
    const auto temp = tools::read_val<OneModelHeaderStart>(buffer);
    if (temp.size() != temp.size2()) {
      buffer = buffer.subspan(sizeof(temp) - sizeof(temp.size2()));
      return OneModelHeaderStart(temp.offset(), temp.size(), temp.size2());
    }
    buffer = buffer.subspan(sizeof(temp));
    return temp;
  }
  //todo check if this function can be void.
  std::vector<std::uint32_t>
    get_tim_offsets(std::span<const char> &buffer) const
  {
    if (m_model_header_start.is_main_chr()) {
      buffer = buffer.subspan(4);
      return {};
    }
    else if (m_model_header_start.is_unknown_object()) {
      buffer = buffer.subspan(8);
      return {};
    }
    std::vector<std::uint32_t> ret{};
    ret.push_back(m_model_header_start.flag_dword() << 8);
    Mch::read_tim_offsets(ret, buffer);
    return ret;
  }

public:
  OneModel(std::span<const char> buffer)
    : m_model_header_start(get_model_header_start(buffer)),
      m_tim_offsets(get_tim_offsets(buffer))
  {}
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_ONEMODEL_HPP
