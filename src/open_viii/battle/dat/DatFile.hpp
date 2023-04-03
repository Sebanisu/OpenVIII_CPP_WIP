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
#include "Section2_Model_geometry.hpp"
#include "Section7_Information_and_Stats.hpp"
#include <cstdint>
#include <filesystem>
#include <span>
#include <vector>
namespace open_viii::battle {
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
  std::vector<char>       m_buffer{};

  /**
   * @brief Path to the DAT file.
   */
  std::filesystem::path   m_path{};

  DatHeader               m_header{};///< The header data of the .dat file.

  Section2_Model_geometry m_section2{};

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
    if (m_header.m_count == 2) {
      m_section7 = open_viii::tools::read_val<Section7_Information_and_Stats>(
        std::span(m_buffer).subspan(m_header.m_offsets.front()));
      return;
    }
    if (m_header.m_count == 11)
    {
        m_section7 = open_viii::tools::read_val<Section7_Information_and_Stats>(
            std::span(m_buffer).subspan(m_header.m_offsets[6]));
        m_section11
            = Section11_Textures(m_buffer.data(), std::span(m_buffer).subspan(m_header.m_offsets[10]));
        return;
    }
    if(m_header.m_count == 7)
    {
        //index 0 = 1;
        //index 1 = 2;
        //index 2 = 3;
        //index 5 = 11;
        m_section11
            = Section11_Textures(m_buffer.data(), std::span(m_buffer).subspan(m_header.m_offsets[5]));
        return;
    }
    if (m_header.m_count == 8) //weapon
    {
        m_section11
            = Section11_Textures(m_buffer.data(), std::span(m_buffer).subspan(m_header.m_offsets[6]));
        return;
    }
    if (m_header.m_count == 5) //weapon 2
    {
        m_section11
            = Section11_Textures(m_buffer.data(), std::span(m_buffer).subspan(m_header.m_offsets[4]));
        return;
    }
    if (m_header.m_count == 10) //edna
    {
        m_section11
            = Section11_Textures(m_buffer.data(), std::span(m_buffer).subspan(m_header.m_offsets[8]));
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

  [[nodiscard]] Section2_Model_geometry &
    section_2()
  {
    return m_section2;
  }

  [[nodiscard]] const Section2_Model_geometry &
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
