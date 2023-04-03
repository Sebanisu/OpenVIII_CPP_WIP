//
// Created by pcvii on 4/3/2023.
//

#ifndef OPENVIII_CPP_WIP_ITEM_HPP
#define OPENVIII_CPP_WIP_ITEM_HPP
#include <cstdint>
namespace open_viii::battle {
/**
   * @brief Represents an item with an ID and quantity.
 */
struct Item
{
  uint8_t ID;      ///< The unique identifier for the item.
  uint8_t quantity;///< The quantity of the item.
};
}
#endif// OPENVIII_CPP_WIP_ITEM_HPP
