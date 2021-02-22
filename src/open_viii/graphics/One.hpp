//
// Created by pcvii on 2/18/2021.
//
#ifndef VIIIARCHIVE_ONE_HPP
#define VIIIARCHIVE_ONE_HPP
#include "Mch.hpp"
#include "one/OneModel.hpp"
namespace open_viii::graphics {
/**
 * @note world\esk\chara.one has 16 tim files
 */
struct One
{
private:
  std::vector<char>     m_buffer{};
  std::uint32_t         m_count{};
  static constexpr auto SIZE_TIM_START = 5U;
  /**
   * 4 bit color tim.
   */
  static constexpr std::string_view TIM4_START{ "\x10\x00\x00\x00\x08",
                                                SIZE_TIM_START };
  /**
   * 8 bit color tim.
   */
  static constexpr std::string_view TIM8_START{ "\x10\x00\x00\x00\x09",
                                                SIZE_TIM_START };
  /**
   * 16 bit color tim.
   */
  static constexpr std::string_view TIM16_START{ "\x10\x00\x00\x00\x09",
                                                 SIZE_TIM_START };
  static_assert(std::ranges::size(TIM4_START) == SIZE_TIM_START);
  static_assert(std::ranges::size(TIM8_START) == SIZE_TIM_START);
  static_assert(std::ranges::size(TIM16_START) == SIZE_TIM_START);
  static constexpr std::array TIM_STARTS{ TIM4_START, TIM8_START, TIM16_START };
public:
  One(std::vector<char> &&buffer)
    : m_buffer(std::move(buffer)),
      m_count(tools::read_val<std::uint32_t>(m_buffer))
  {}
  void save([[maybe_unused]] const std::string &path) const
  {
    size_t i{};
    for (const auto &start : TIM_STARTS) {
      auto buffer = std::span<const char>(m_buffer);
      while (true) {
        const auto offset = tools::search(buffer, start);
        if (offset == std::ranges::end(buffer)) {
          break;
        }
        buffer = std::span<const char>(offset,
                                       std::ranges::end(buffer));
        const auto tim = Tim(buffer);
        buffer = buffer.subspan(std::ranges::size(start)); //TODO figure out why this isn't moving forward past the previous tim.
        if (tim.check()) {
          const auto out_path = path + std::to_string(i++);
          tim.save(out_path);
        }
      }
    }
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_ONE_HPP
