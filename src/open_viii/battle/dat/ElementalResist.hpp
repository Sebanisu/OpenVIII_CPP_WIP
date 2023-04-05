//
// Created by pcvii on 4/3/2023.
//

#ifndef OPENVIII_CPP_WIP_ELEMENTALRESIST_HPP
#define OPENVIII_CPP_WIP_ELEMENTALRESIST_HPP
#include "open_viii/kernel/ElementT.hpp"
#include <cstdint>
#include <ranges>
#include <stdexcept>
namespace open_viii::battle::dat {
/**
 * @brief Represents elemental resistances for a character.
 */
struct ElementalResist
{
  std::uint8_t Fire;   ///< Fire resistance.
  std::uint8_t Ice;    ///< Ice resistance.
  std::uint8_t Thunder;///< Thunder resistance.
  std::uint8_t Earth;  ///< Earth resistance.
  std::uint8_t Poison; ///< Poison resistance.
  std::uint8_t Wind;   ///< Wind resistance.
  std::uint8_t Water;  ///< Water resistance.
  std::uint8_t Holy;   ///< Holy resistance.

  /**
   * @brief Gets a range of set flags for a given Element value.
   *
   * @param elementFlags The Element value containing the flags.
   * @return A range of Element values corresponding to the set flags.
   */
  constexpr static auto
    getSetElementFlags(kernel::ElementT elementFlags)
  {
    std::uint8_t   minFlagOffset = 0U;
    std::uint8_t   maxFlagOffset = 8U;

    constexpr auto flagValue     = [](std::uint8_t offset) -> kernel::ElementT {
      return static_cast<kernel::ElementT>(1U << offset);
    };

    const auto isSet = [=](kernel::ElementT flag) {
      return (static_cast<std::uint8_t>(flag)
              & static_cast<std::uint8_t>(elementFlags))
          != 0U;
    };

    return std::views::iota(minFlagOffset, maxFlagOffset)
         | std::views::transform(flagValue) | std::views::filter(isSet);
  }

  /**
   * @brief Gets a range of references to the element resistances for a
   * given Element flag.
   *
   * @param element_flag The Element flag value.
   * @return A range of references to the corresponding element
   * resistances.
   */
  [[nodiscard]] auto
    getRefToElementResistances(kernel::ElementT element_flag)
  {
    return getSetElementFlags(element_flag)
         | std::views::transform(
             [this](kernel::ElementT current_flag) -> std::uint8_t & {
               return operator[](current_flag);
             });
  }

  /**
   * @brief Gets a range of constant references to the element resistances
   * for a given Element flag.
   *
   * @param element_flag The Element flag value.
   * @return A range of constant references to the corresponding element
   * resistances.
   */
  [[nodiscard]] auto
    getRefToElementResistances(kernel::ElementT element_flag) const
  {
    return getSetElementFlags(element_flag)
         | std::views::transform(
             [this](kernel::ElementT current_flag) -> const std::uint8_t & {
               return operator[](current_flag);
             });
  }

  /**
   * @brief Index operator to access the resistance value of a specific
   * element.
   *
   * @param element The element whose resistance value is to be accessed.
   * @return A reference to the corresponding resistance value.
   */
  std::uint8_t &
    operator[](kernel::ElementT element)
  {
    switch (element) {
    case kernel::ElementT::fire:
      return Fire;
    case kernel::ElementT::ice:
      return Ice;
    case kernel::ElementT::thunder:
      return Thunder;
    case kernel::ElementT::earth:
      return Earth;
    case kernel::ElementT::poison:
      return Poison;
    case kernel::ElementT::wind:
      return Wind;
    case kernel::ElementT::water:
      return Water;
    case kernel::ElementT::holy:
      return Holy;
    default:
      throw std::out_of_range("Invalid element");
    }
  }
  /**
   * @brief Index operator to access the resistance value of a specific
   * element.
   *
   * @param element The element whose resistance value is to be accessed.
   * @return A reference to the corresponding resistance value.
   */
  const std::uint8_t &
    operator[](kernel::ElementT element) const
  {
    switch (element) {
    case kernel::ElementT::fire:
      return Fire;
    case kernel::ElementT::ice:
      return Ice;
    case kernel::ElementT::thunder:
      return Thunder;
    case kernel::ElementT::earth:
      return Earth;
    case kernel::ElementT::poison:
      return Poison;
    case kernel::ElementT::wind:
      return Wind;
    case kernel::ElementT::water:
      return Water;
    case kernel::ElementT::holy:
      return Holy;
    default:
      throw std::out_of_range("Invalid element");
    }
  }
};
}// namespace open_viii::battle
#endif// OPENVIII_CPP_WIP_ELEMENTALRESIST_HPP
