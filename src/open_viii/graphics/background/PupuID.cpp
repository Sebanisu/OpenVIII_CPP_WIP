#include "PupuID.hpp"
namespace open_viii::graphics::background
{
[[nodiscard]] std::string PupuID::create_summary() const
{
     return fmt::format(
       "Layer ID: {}\nBlend Mode: {}\nAnimation ID: {}\nAnimation State: "
       "{}\nOffset: {}\nX not aligned: {}\nY not aligned: {}",
       layer_id(),
       blend_mode(),
       animation_id(),
       animation_state(),
       offset(),
       is_x_not_aligned_to_grid(),
       is_y_not_aligned_to_grid());
}

// You are using all 32 bits without overlap.
// The layer field uses 7 bits, which are stored in bits 24-30 of the m_raw
// value. The blend field uses only 3 bits, which are stored in bits 20-22 of
// the m_raw value. The animation id and animation state fields are 8 bits each,
// which are stored in bits 12-19 and 4-11 respectively. The offset field uses 4
// bits, which are stored in bits 0-3 of the m_raw value. Finally, you use two
// more bits (31 and 23) for the x_not_aligned_to_grid and y_not_aligned_to_grid
// flags, respectively. Therefore, all 32 bits are used without any overlap or
// waste of space.


// template PupuID::PupuID(const Tile1 &, std::uint8_t);
// template PupuID::PupuID(const Tile2 &, std::uint8_t);
// template PupuID::PupuID(const Tile3 &, std::uint8_t);


// PupuID = uint32_t(0U + (tile.layer_id() <<
// 24U)+(static_cast<std::uint32_t>(tile.blend_mode()) << 20U) +
// (tile.animation_id() <<12U)+ (tile.animation_state()<<4U))

// std::transform(Sprites.cbegin(), Sprites.cend() - 1, Sprites.cbegin() + 1,
// Sprites.begin()+1, [](const Sprite& first, Sprite second) {
//     static constexpr auto mask = 0xFFFFFFF0U;
//     if ((first.ID & mask) == (second.ID & mask))
//     {
//       second.ID = first.ID + 1;
//     }
//     return second;
//   });

// IDs.reserve(Sprites.size());
// std::transform(Sprites.cbegin(), Sprites.cend(), std::back_inserter(IDs),
// [](const Sprite& sprite) {return sprite.ID; }); std::sort(IDs.begin(),
// IDs.end()); auto it = std::unique(IDs.begin(), IDs.end()); IDs.erase(it,
// IDs.end());
}// namespace ff_8


// struct PupuIDComparator
// {
//      bool operator()(const PupuID &a, const PupuID &b) const
//      {
//           auto key = [](const PupuID &id) {
//                return std::tuple{ // 1. prefer static images (animation_id ==
//                255)
//                                   id.animation_id() == 255 ? 0 : 1,

//                                   // 2. prefer blend mode none
//                                   id.blend_mode() == BlendMode::None ? 0 : 1,

//                                   // 3. prefer animated if still tied
//                                   id.animation_id() != 255 ? 0 : 1,

//                                   // 4. if blend mode != none, group by blend
//                                   mode id.blend_mode() != BlendMode::None ?
//                                   static_cast<int>(id.blend_mode()) : -1,

//                                   // 5. prefer smallest offset
//                                   id.offset(),

//                                   // 6. prefer grid aligned
//                                   (id.is_x_not_aligned_to_grid() ||
//                                   id.is_y_not_aligned_to_grid()) ? 1 : 0
//                };
//           };

//           return key(a) < key(b);
//      }
// };
// struct PupuIDComparator
// {
//      const std::unordered_map<PupuID, size_t> *pupuCount; //not sure what
//      this will do lol. the ids are stored in a vector and we know the count
//      from std::size()

//      bool                                      operator()(const PupuID &a,
//      const PupuID &b) const
//      {
//           auto key = [&](const PupuID &id) {
//                return std::tuple{ pupuCount->at(id) == 1 ? 0 : 1,// prefer
//                unique
//                                   id.animation_id() == 255 ? 0 : 1,
//                                   id.blend_mode() == BlendMode::None ? 0 : 1,
//                                   id.animation_id() != 255 ? 0 : 1,
//                                   id.blend_mode() != BlendMode::None ?
//                                   static_cast<int>(id.blend_mode()) : -1,
//                                   id.offset(),
//                                   (id.is_x_not_aligned_to_grid() ||
//                                   id.is_y_not_aligned_to_grid()) ? 1 : 0 };
//           };
//           return key(a) < key(b);
//      }
// };

// hmm since each file entry will have one or more pupu ids. would we like use
// the comparitor to sort the list of pupu ids and then choose the one that is
// first?  I plan on having like one set of keys and once one gets used up it
// won't be able to be used by another image. then we'll pass those images
// through the maskes to hide the unused pixels.hmm since each file entry will
// have one or more pupu ids. would we like use the comparitor to sort the list
// of pupu ids and then choose the one that is first?  I plan on having like one
// set of keys and once one gets used up it won't be able to be used by another
// image. then we'll pass those images through the maskes to hide the unused
// pixels.