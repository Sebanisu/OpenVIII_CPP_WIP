#include "UniquifyPupu.hpp"
namespace open_viii::graphics::background
{
PupuID UniquifyPupu::operator()(const is_tile auto &tile_const)
{
     const auto t           = 16;
     const auto x           = static_cast<int16_t>(tile_const.x() / t);
     const auto y           = static_cast<int16_t>(tile_const.y() / t);
     auto       input_value = PupuKey{ PupuID(tile_const), x, y };
     auto       insert_key  = [&](PupuKey key) -> PupuID
     {
          if (m_pupu_map.contains(key))
          {
               ++(m_pupu_map.at(key));
               return key.pupu_id + m_pupu_map.at(key);
          }
          else
          {
               m_pupu_map.emplace(key, std::uint8_t{});
               return key.pupu_id + m_pupu_map.at(std::move(key));
          }
     };
     return insert_key(input_value);
}
template PupuID UniquifyPupu::operator()(const Tile1 &);
template PupuID UniquifyPupu::operator()(const Tile2 &);
template PupuID UniquifyPupu::operator()(const Tile3 &);
//  operator UniqueValues<PupuID>() const
//  {
//    std::vector<PupuID> values{};
//    std::ranges::transform(m_pupu_map,
//    std::back_insert_iterator(values),[](auto && key_value)->PupuID{
//      const auto & [key,value] = key_value;
//      return key.pupu_id;
//    });
//    UniqueValues<PupuID>{std::move(values)};
//  }
}// namespace open_viii::graphics::background