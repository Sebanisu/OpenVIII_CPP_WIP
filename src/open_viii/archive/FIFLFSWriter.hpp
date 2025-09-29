//
// Created by pcvii on 3/21/2023.
//

#ifndef OPENVIII_CPP_WIP_FIFLFSWRITER_HPP
#define OPENVIII_CPP_WIP_FIFLFSWRITER_HPP
#include "FIOutIterator.hpp"
#include "FL.hpp"
#include "FS.hpp"
namespace open_viii::archive {
/**
 * @brief The FIFLFSWriter class writes data to a file system.
 *
 * This class writes data to a file system consisting of three files: a
 * data file (FS), a file information file (FI), and a file list file (FL).
 */
class FIFLFSWriter
{
public:
  /**
   * @brief The iterator category of this output iterator.
   */
  using iterator_category = std::output_iterator_tag;

  /**
   * @brief The type of the values that can be written using this output
   * iterator.
   */
  using value_type        = std::
    tuple<std::span<const char>, CompressionTypeT, std::filesystem::path>;

  /**
   * @brief The difference type used by this output iterator.
   */
  using difference_type = std::ptrdiff_t;

  /**
   * @brief The pointer type used by this output iterator.
   *
   * This output iterator does not support the use of pointers.
   */
  using pointer         = void;

  /**
   * @brief The reference type used by this output iterator.
   *
   * This output iterator does not support the use of references.
   */
  using reference       = void;

  /**
   * @brief Prefix increment operator for the output iterator.
   *
   * This function does nothing and returns a reference to the output iterator.
   *
   * @return A reference to the output iterator.
   */
  FIFLFSWriter &
    operator++() noexcept
  {
    return *this;
  }

  /**
   * @brief Postfix increment operator for the output iterator.
   *
   * This function does nothing and returns a reference to the output iterator.
   *
   * @return A reference to the output iterator.
   */
  FIFLFSWriter &
    operator++(int) noexcept
  {
    return *this;
  }

  /**
   * @brief Dereference operator for the output iterator.
   *
   * This function does nothing and returns a reference to the output iterator.
   *
   * @return A reference to the output iterator.
   */
  FIFLFSWriter &
    operator*() noexcept
  {
    return *this;
  }

  /**
   * @brief Assignment operator for the output iterator.
   *
   * This function writes the given tuple of arguments to a compressed file
   * using FIFLFS.
   *
   * @param tuple_of_arguments The tuple of arguments to write to the compressed
   * file.
   * @return A reference to the output iterator.
   */
  FIFLFSWriter &
    operator=(const value_type &tuple_of_arguments)
  {
    const auto &[buffer, compressed, filePath] = tuple_of_arguments;
    write(buffer, compressed, filePath);
    return *this;
  }
  /**
   * @brief Constructs an FIFLFSWriter object with the specified file path.
   *
   * @param filePath The path to the file to write.
   */
  FIFLFSWriter(std::filesystem::path filePath)
    : m_fs(
        filePath.replace_extension(FS::EXT),
        std::ios::binary | std::ios::out | std::ios::trunc),
      m_fi(filePath.replace_extension(FI::EXT)),
      m_fl(
        filePath.replace_extension(fl::EXT),
        std::ios::binary | std::ios::out | std::ios::trunc)
  {}

  /**
   * @brief Writes the specified buffer to the file system.
   *
   * This method writes the specified buffer to the data file (FS) and adds
   * an entry to the file information file (FI) with the offset and size of
   * the written data, as well as the specified compression type. It also
   * adds the file path to the file list file (FL).
   *
   * @param buffer The buffer to write to the file system.
   * @param compressed The compression type used for the data.
   * @param filePath The path to the file being written.
   */
  void
    write(
      std::span<const char>        buffer,
      CompressionTypeT             compressed,
      const std::filesystem::path &filePath)
  {
    // Write the raw data to the FS file
    m_fs.seekp(0, std::ios_base::end);
    m_fs.write(
      buffer.data(),
      static_cast<std::ofstream::off_type>(buffer.size()));

    // Write the FI entry to the FI file
    FI fi{ static_cast<std::uint32_t>(
             m_fs.tellp()
             - static_cast<std::ofstream::off_type>(buffer.size())),
           static_cast<std::uint32_t>(buffer.size()),
           compressed };
    m_fi = fi;
    ++m_fi;

    // Add the file path to the FL file
    using namespace std::string_literals;
    std::string string = "C:\\"s + filePath.string() + "\r\n"s;
    tl::string::undo_replace_slashes(string);
    m_fl.seekp(0, std::ios_base::end);
    m_fl.write(
      string.data(),
      static_cast<std::ofstream::off_type>(string.size()));
  }

private:
  std::ofstream m_fs;///< The file stream for the data file.
  FIOutIterator m_fi;///< The output iterator for the file information file.
  std::ofstream m_fl;///< The file stream for the file list file.
};
static_assert(std::output_iterator<FIFLFSWriter, FIFLFSWriter::value_type>);

}// namespace open_viii::archive
#endif// OPENVIII_CPP_WIP_FIFLFSWRITER_HPP
