//
// Created by pcvii on 3/30/2023.
//

#ifndef OPENVIII_CPP_WIP_DATFILE_HPP
#define OPENVIII_CPP_WIP_DATFILE_HPP
#include "DatHeader.hpp"
#include "open_viii/CardID.hpp"
#include "open_viii/kernel/GFID.hpp"
#include "open_viii/kernel/MagicSpell.hpp"
#include "open_viii/strings/FF8String.hpp"
#include "open_viii/tools/Read.hpp"
#include "Section11_Textures.hpp"

#include "Section1_Skeleton.hpp"
#include "Section2_Model_Geometry.hpp"
#include "Section3_Model_Animation.hpp"
#include "Section5_Animation_Sequences.hpp"
#include "Section7_Information_and_Stats.hpp"
#include <cstdint>
#include <filesystem>
#include <span>
#include <vector>
namespace open_viii::battle::dat {
/**
 * @brief Represents a DAT file containing battle-related data.
 *
 * @see https://wiki.ffrtt.ru/index.php/FF8/FileFormat_DAT
 * @see https://forums.qhimm.com/index.php?topic=11137.msg154834
 * @see
 * https://github.com/MaKiPL/OpenVIII-monogame/blob/master/Core/Battle/Dat/DatFile.cs
 * @see https://sourceforge.net/projects/ifrit/
 */
class DatFile
{
private:
  /**
   * @brief Buffer to store the data read from the DAT file.
   */
  std::vector<char>            m_buffer{};

  /**
   * @brief Path to the DAT file.
   */
  std::filesystem::path        m_path{};

  DatHeader                    m_header{};///< The header data of the .dat file.

  Section1_Skeleton            m_section1{};
  Section2_Model_Geometry      m_section2{};
  Section3_Model_Animation     m_section3{};
  Section5_Animation_Sequences m_section5{};

  /**
   * @brief Contains information and statistics about a character in the game.
   */
  Section7_Information_and_Stats m_section7{};

  Section11_Textures             m_section11{};

public:
  /**
   * @brief Default constructor for DatFile.
   */
  DatFile() = default;

  [[nodiscard]] auto
    get_span(std::size_t index) const
  {
    if (index + 1 == m_header.m_count) {
      return std::span(m_buffer).subspan(m_header.m_offsets[index]);
    }
    return std::span(m_buffer).subspan(
      m_header.m_offsets[index],
      m_header.m_offsets[index + 1] - m_header.m_offsets[index]);
  }
  /**
   * @brief Constructs a DatFile with the given buffer and file path.
   *
   * @param in_buffer Input buffer containing the file data.
   * @param path File path of the .dat file.
   */
  DatFile(std::vector<char> in_buffer, std::filesystem::path path)
    : m_buffer(std::move(in_buffer)), m_path(std::move(path)),
      m_header(m_buffer)
  {
    if (m_header.m_count == 2)// monster 127
    {
      m_section7 = open_viii::tools::read_val<Section7_Information_and_Stats>(
        std::span(m_buffer).subspan(m_header.m_offsets.front()));
      return;
    }
    if (m_header.m_count == 11)// monster
    {
      m_section1 = Section1_Skeleton(m_buffer.data(), get_span(0));
      m_section3 = Section3_Model_Animation(m_buffer.data(), get_span(2));
      m_section2 = Section2_Model_Geometry(m_buffer.data(), get_span(1));
      m_section5 = Section5_Animation_Sequences(
        m_buffer.data(),
        get_span(4),
        get_span(5));
      m_section7 = open_viii::tools::read_val<Section7_Information_and_Stats>(
        get_span(6));
      m_section11 = Section11_Textures(m_buffer.data(), get_span(10));
      return;
    }
    if (m_header.m_count == 7)// character
    {
      m_section1  = Section1_Skeleton(m_buffer.data(), get_span(0));
      m_section3  = Section3_Model_Animation(m_buffer.data(), get_span(2));
      m_section2  = Section2_Model_Geometry(m_buffer.data(), get_span(1));
      m_section11 = Section11_Textures(m_buffer.data(), get_span(5));
      return;
    }
    if (m_header.m_count == 8)// weapon rest
    {

      m_section1 = Section1_Skeleton(m_buffer.data(), get_span(0));
      m_section3 = Section3_Model_Animation(m_buffer.data(), get_span(2));
      m_section2 = Section2_Model_Geometry(m_buffer.data(), get_span(1));
      m_section5 = Section5_Animation_Sequences(
        m_buffer.data(),
        get_span(3),
        get_span(4));
      m_section11 = Section11_Textures(m_buffer.data(), get_span(6));
      return;
    }
    if (m_header.m_count == 5)// weapon fists
    {
      // Skeleton and Animations from character.
      m_section2 = Section2_Model_Geometry(m_buffer.data(), get_span(0));
      m_section5 = Section5_Animation_Sequences(
        m_buffer.data(),
        get_span(1),
        get_span(2));
      m_section11 = Section11_Textures(m_buffer.data(), get_span(4));
      return;
    }
    if (m_header.m_count == 10)// edna
    {
      m_section1 = Section1_Skeleton(m_buffer.data(), get_span(0));
      m_section3 = Section3_Model_Animation(m_buffer.data(), get_span(2));
      m_section2 = Section2_Model_Geometry(m_buffer.data(), get_span(1));
      m_section5 = Section5_Animation_Sequences(
        m_buffer.data(),
        get_span(5),
        get_span(6));
      m_section11 = Section11_Textures(m_buffer.data(), get_span(8));
      return;
    }
    std::cout << m_header.m_count << std::endl;
  }

  /**
   * @brief Get a const reference to the Section7_Information_and_Stats object.
   *
   * @return Const reference to the Section7_Information_and_Stats object.
   */
  [[nodiscard]] const Section7_Information_and_Stats &
    section_7() const
  {
    return m_section7;
  }

  /**
   * @brief Get a non-const reference to the Section7_Information_and_Stats
   * object.
   *
   * @return Non-const reference to the Section7_Information_and_Stats object.
   */
  Section7_Information_and_Stats &
    section_7()
  {
    return m_section7;
  }

  [[nodiscard]] Section2_Model_Geometry &
    section_2()
  {
    return m_section2;
  }

  [[nodiscard]] const Section2_Model_Geometry &
    section_2() const
  {
    return m_section2;
  }

  [[nodiscard]] const Section11_Textures &
    section_11() const
  {
    return m_section11;
  }
};
}// namespace open_viii::battle
#endif// OPENVIII_CPP_WIP_DATFILE_HPP
