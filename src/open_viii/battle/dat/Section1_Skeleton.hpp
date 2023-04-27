/**
 * @file section1_skeleton.hpp
 * @author pcvii
 * @date 4/4/2023
 */

#ifndef OPENVIII_CPP_WIP_SECTION1_SKELETON_HPP
#define OPENVIII_CPP_WIP_SECTION1_SKELETON_HPP
#include <span>

/**
 * @brief Namespace for open_viii battle dat related structs and functions
 */
namespace open_viii::battle::dat {

/**
 * @brief Bone struct for Skeleton
 * @see https://wiki.ffrtt.ru/index.php/FF8/FileFormat_DAT#Bone_struct
 * @see https://github.com/MaKiPL/OpenVIII-monogame/blob/master/Core/Battle/Dat/Bone.cs
 */
struct Bone
{
  uint16_t parentId;///< Parent bone ID
  int16_t  boneSize;///< Divide by 4096.0f to get actual float value
  int16_t
    rotX;///< Multiply by 360.0f and divide by 4096.0f to get actual float value
  int16_t
    rotY;///< Multiply by 360.0f and divide by 4096.0f to get actual float value
  int16_t
    rotZ;///< Multiply by 360.0f and divide by 4096.0f to get actual float value
  int16_t                  unk4;///< Divide by 4096.0f to get actual float value
  int16_t                  unk5;///< Divide by 4096.0f to get actual float value
  int16_t                  unk6;///< Divide by 4096.0f to get actual float value
  uint8_t                  unk[28];  ///< UnkSize is 28
  static constexpr uint8_t SIZE = 44;///< Size of Bone struct
};
static_assert(sizeof(Bone) == Bone::SIZE);

/**
 * @brief Header struct for Skeleton
 * @see https://wiki.ffrtt.ru/index.php/FF8/FileFormat_DAT#Section_1:_Skeleton
 * @see https://github.com/MaKiPL/OpenVIII-monogame/blob/master/Core/Battle/Dat/Skeleton.cs
 */
struct SkeletonHeader
{
  uint16_t numBones;///< Number of bones in the skeleton
  uint16_t scale;   ///< Divide by 4096.0f to get actual float value
  uint16_t unk2;    ///< Unknown data field
  uint16_t unk3;    ///< Unknown data field
  uint16_t unk4;    ///< Unknown data field
  uint16_t unk5;    ///< Unknown data field
  uint16_t unk6;    ///< Unknown data field
  uint16_t unk7;    ///< Unknown data field
  // Bones array should follow this struct in memory. Each Bone struct is 48
  // bytes.
  static constexpr uint8_t SIZE = 16;///< Size of SkeletonHeader struct
};
static_assert(sizeof(SkeletonHeader) == SkeletonHeader::SIZE);

/**
 * @brief Skeleton struct containing header and bone data
 * @see https://wiki.ffrtt.ru/index.php/FF8/FileFormat_DAT#Section_1:_Skeleton
 * @see https://github.com/MaKiPL/OpenVIII-monogame/blob/master/Core/Battle/Dat/Skeleton.cs
 */
struct Section1_Skeleton
{
  SkeletonHeader    m_header{};///< Header data for the skeleton
  std::vector<Bone> m_bones{}; ///< Vector containing bone data

  /**
   * @brief Default constructor for Section1_Skeleton
   */
  Section1_Skeleton() = default;

  /**
   * @brief Constructor for Section1_Skeleton
   * @param section_start Pointer to the start of the section data
   * @param span Span containing the entire section data
   */
  Section1_Skeleton(
    [[maybe_unused]] const char *const     section_start,
    [[maybe_unused]] std::span<const char> span)
  {
    m_header = tools::read_val<SkeletonHeader>(span);
    m_bones = tools::read_vals<Bone>(span,m_header.numBones);
  }
};

}// namespace open_viii::battle::dat
#endif// OPENVIII_CPP_WIP_SECTION1_SKELETON_HPP
