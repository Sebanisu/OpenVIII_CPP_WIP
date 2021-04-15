//
// Created by pcvii on 2/18/2021.
//
#ifndef VIIIARCHIVE_ONE_HPP
#define VIIIARCHIVE_ONE_HPP
#include "BPPT.hpp"
#include "Mch.hpp"
#include "one/OneModel.hpp"
namespace open_viii::graphics {
constexpr auto
  get_tim_header_start(const BPPT bpp)
{
  return std::array<char, 5U>{ 0x10, 0x0, 0x0, 0x0, static_cast<char>(bpp) };
}
constexpr auto get_time_header_starts()
{
  /**
   * 4 bit color tim.
   */
  constexpr auto TIM4_START = get_tim_header_start(BPPT::as_bpp4());
  /**
   * 8 bit color tim.
   */
  constexpr auto TIM8_START = get_tim_header_start(BPPT::as_bpp8());
  /**
   * 16 bit color tim.
   */
  constexpr auto TIM16_START = get_tim_header_start(BPPT::as_bpp16());
  /**
   * 24 bit color tim.
   */
  constexpr auto TIM24_START = get_tim_header_start(BPPT::as_bpp24());
  return std::array{
    TIM4_START, TIM8_START, TIM16_START, TIM24_START
  };
}
/**
 * @note world\esk\chara.one has 16 tim files
 */
struct One
{
private:
  std::vector<char>     m_buffer{};
  std::uint32_t         m_count{};
  static constexpr auto TIM_STARTS = get_time_header_starts();
  auto
    get_next_offset(const std::span<const char> &buffer) const
  {
    // could be a coroutine using std::generator
    // because these tim can be in any order.
    // todo. maybe have a way to pop items from the TIM_STARTS that don't
    // have at least 1 match.
    return std::ranges::min(
      TIM_STARTS | std::ranges::views::transform([&buffer](const auto &start) {
        return tools::search(buffer, start);
      }));
  }
  Tim
    get_tim(auto offset, std::span<const char> &buffer) const
  {
    buffer        = std::span<const char>(offset, std::ranges::end(buffer));
    const Tim tim = Tim(buffer);
    buffer        = buffer.subspan(std::ranges::size(TIM_STARTS.front()));
    return tim;
  }

public:
  One(std::vector<char> &&buffer)
    : m_buffer(std::move(buffer)),
      m_count(tools::read_val<std::uint32_t>(m_buffer))
  {}
  bool
    save_tim(const std::string &path, const size_t i, const Tim &tim) const
  {
    if (tim) {
      auto       path_parts = std::filesystem::path(path);
      const auto out_path   = (path_parts.parent_path()
                             / (path_parts.stem().string() + std::to_string(i)
                                + path_parts.extension().string()))
                              .string();
      tim.save(out_path);
      return true;
    }
    return false;
  }
  void
    save([[maybe_unused]] const std::string &path) const
  {
    // todo make this generic, extract tim file code to a free function.
    size_t i{};
    auto   buffer = std::span<const char>(m_buffer);
    while (true) {
      const auto offset = get_next_offset(buffer);
      if (offset == std::ranges::end(buffer)) {
        break;
      }
      if (save_tim(path, i, get_tim(offset, buffer))) {
        ++i;
      }
    }
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_ONE_HPP
