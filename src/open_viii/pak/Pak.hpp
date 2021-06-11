//
// Created by pcvii on 10/30/2020.
//
#ifndef VIIIARCHIVE_PAK_HPP
#define VIIIARCHIVE_PAK_HPP
#include "MovieClip.hpp"
#include "open_viii/tools/Tools.hpp"
#include <array>
#include <cctype>
#include <cinttypes>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>
namespace open_viii {
/**
 * PAK reading struct, Gathers all the sections
 * @remark Original class wrote by Maki for the ToolKit
 * @see
 * https://github.com/MaKiPL/FF8-Rinoa-s-Toolset/tree/master/SerahToolkit_SharpGL/FF8_Core
 */
struct Pak
{
private:
  /**
   * Each frame section of cam file is this many bytes
   */
  static constexpr auto CAM_SECTION_SIZE       = static_cast<char>(0x2C);
  /**
   * If false don't extract low res videos!
   */
  static constexpr auto ENABLE_EXTRACT_LOW_RES = false;
  /**
   *  Known valid Bink video formats "a,d,f,g,h,i"
   */
  static constexpr std::array<char, 6> BIK1
    = { 0x61, 0x64, 0x66, 0x67, 0x68, 0x69 };
  /**
   * Known valid Bink 2 video formats "b,d,f,g,h,i"
   */
  static constexpr std::array<char, 6> BIK2
    = { 0x62, 0x64, 0x66, 0x67, 0x68, 0x69 };
  /**
   * Each Movie has 1 cam and 2 versions of the video.
   */
  std::vector<MovieClip> m_movies;
  ///// <summary>
  ///// Depending on type you read it differently.
  ///// </summary>
  // private readonly Dictionary<Type, Func<BinaryReader, Type, FileSection>>
  // _readFunctions;
  //  template<FileSectionTypeT fileSectionTypeT> readFunctions()
  //  {
  //  }
  //}
  /**
   * Remembers detected disc number.
   */
  int                    m_disc_cache{ -1 };
  std::filesystem::path  m_file_path{};

public:
  explicit Pak(std::filesystem::path path) : m_file_path(std::move(path))
  {
    //    static constexpr auto default_size = 7U;
    //    m_movies.reserve(default_size);
    //  _readFunctions = new Dictionary<Type, Func<BinaryReader, Type,
    //  FileSection>>()
    //  {
    //    {Type.Cam, ReadCam },
    //    {Type.Bik, ReadBik },
    //    {Type.Kb2, ReadKb2 },
    //  };
    read();
  }
  /**
   * Total number of movies detected
   * @return count of movies
   */
  [[nodiscard]] auto
    count() const noexcept
  {
    return std::ranges::size(m_movies);
  }
  /**
   * Current path
   * @return ref to path of pak file.
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    file_path() const noexcept
  {
    return m_file_path;
  }
  /**
   * Each Movie has 1 cam and 2 versions of the video.
   * @return vector of movies.
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    movies() const noexcept
  {
    return m_movies;
  }
  /**
   * Gets Movie Clip at index
   * @param i = index
   * @return Movie clip
   */
  [[nodiscard]] const auto &
    operator[](const std::size_t i) const noexcept
  {
    return m_movies[i];
  }
  /**
   * Gets Movie Clip at index
   * @param i = index
   * @return Movie clip
   */
  [[nodiscard]] const auto &
    at(const std::size_t i) const noexcept
  {
    return m_movies.at(i);
  }
  void
    extract(std::filesystem::path dest_path)
  {
    std::cout << "Extracting \"" << m_file_path.string() << "\"\n";
    tools::read_from_file(
      [this, &dest_path](std::istream &is) {
        std::ranges::for_each(
          m_movies,
          [&is, &dest_path](const MovieClip &item) {
            const auto e = [&dest_path, &is](const FileSection &fs) {
              auto out_path = dest_path / fs.file_name();
              if ([&out_path, &fs]() -> bool {
                    if (std::filesystem::exists(out_path)) {
                      return std::filesystem::file_size(out_path) != fs.size();
                    }
                    return true;
                  }()) {
                if (!std::ranges::empty(fs.file_name()) && fs.size() > 0U) {
                  tools::write_buffer(
                    [&is, &fs](std::ostream &os) {
                      std::cout << "Extracting \"" << fs.file_name() << "\"\n";
                      is.seekg(fs.offset(), std::ios::beg);
                      std::vector<char> tmp{};
                      tmp.resize(fs.size());
                      is.read(std::ranges::data(tmp),
                              static_cast<std::intmax_t>(fs.size()));
                      os.write(std::ranges::data(tmp),
                               static_cast<std::intmax_t>(fs.size()));
                    },
                    out_path.string());
                }
              }
            };
            e(item.cam_fs());
            e(item.bink_high());
            if constexpr (ENABLE_EXTRACT_LOW_RES) {
              e(item.bink_low());
            }
          });
      },
      m_file_path);
  }
  /**
   * Generate filename that would be saved on export.
   * @param extension extension of the file
   * @param suffix suffix that gets appended to end of names
   * @return std::string
   */
  std::string
    generate_file_name(const std::string &extension,
                       const std::string &suffix = {})
  {
    using namespace std::string_literals;
    static constexpr auto length = 2U;
    return "disc"s + tools::to_string_with_padding(get_disc_number(), length)
         + "_"s + tools::to_string_with_padding(count(), length) + suffix
         + extension;
  }
  /**
   * Extract number from filename. typically there is 1 digit for the disc
   * number.
   * @return disc number.
   */
  int
    get_disc_number()
  {
    auto stem = m_file_path.stem().string();
    if (m_disc_cache == -1) {
      auto b = std::ranges::find_if(stem, [](const char &c) {
        return std::isdigit(c);
      });
      if (b != std::ranges::end(stem)) {
        m_disc_cache = *b - '0';
      }
    }
    if (m_disc_cache <= 0) {
      return 4;
    }
    return m_disc_cache - 1;
  }
  void
    get_cam(std::istream &is, MovieClip &movie)
  { /**
     * Read cam file offset and size
     */
    FileSection fs{};
    fs.type(FileSectionTypeT::CAM);
    fs.offset(static_cast<int64_t>(is.tellg()) - 3);
    is.seekg(3, std::ios::cur);
    auto frames = tools::read_val<uint16_t>(is);
    is.seekg(frames * CAM_SECTION_SIZE, std::ios::cur);
    // there seems to be 1 or more extra frames. Check for those.
    while (!FileSectionTypeT::valid_type(FileSectionTypeT::get_type(is),
                                         FileSectionTypeT::BIK,
                                         FileSectionTypeT::KB2)) {
      is.seekg(CAM_SECTION_SIZE - 3, std::ios::cur);
      frames++;
    }
    // Found the end go back to it.
    is.seekg(-3, std::ios::cur);
    using namespace std::string_literals;
    fs.size(static_cast<uint32_t>(is.tellg() - fs.offset()));
    fs.file_name(generate_file_name(".cam"s));
    fs.frames(frames);
    movie.cam(Cam(is, fs));
    movie.cam_fs(std::move(fs));
  }
  void
    get_bik(std::istream &               is,
            MovieClip &                  movie,
            const std::span<const char> &type)
  { /**
     * Read Bink video offset and size
     */
    char        version = tools::read_val<char>(is);
    FileSection fs{};
    if (std::ranges::equal(type, FileSectionTypeT::BIK)
        && std::ranges::any_of(BIK1, [&version](const auto &item) {
             return version == item;
           })) {
      fs.type(FileSectionTypeT::BIK);
    }
    else if (std::ranges::equal(type, FileSectionTypeT::KB2)
             && std::ranges::any_of(BIK2, [&version](const auto &item) {
                  return version == item;
                })) {
      fs.type(FileSectionTypeT::KB2);
    }
    else {
      std::cerr << "location: " << std::hex << is.tellg() << std::endl;
      std::cerr << "bink version and type mismatch...\t" << fs.type() << '\t'
                << static_cast<int32_t>(version) << std::dec << std::endl;
      return;
    }
    fs.offset(static_cast<int64_t>(is.tellg()) - 4);
    fs.size(tools::read_val<uint32_t>(is));
    static constexpr auto header_size = 8U;
    fs.size(fs.size() + header_size);
    fs.frames(tools::read_val<uint32_t>(is));
    is.seekg(fs.offset() + fs.size(), std::ios::beg);
    using namespace std::string_literals;
    static constexpr auto get_ext = [](const std::string_view &t) {
      if (t == FileSectionTypeT::BIK) {
        return ".bik"s;
      }
      return ".bk2"s;
    };
    if (std::ranges::empty(movie.bink_high().type())) {
      movie.bink_high(std::move(fs));
      movie.mutable_bink_high().file_name(
        generate_file_name(get_ext(movie.bink_high().type()), "h"s));
    }
    else {
      if (fs.size() > movie.bink_high().size()) {
        movie.swap_bink();
        movie.bink_high(std::move(fs));
      }
      else {
        movie.bink_low(std::move(fs));
      }
      movie.mutable_bink_high().file_name(
        generate_file_name(get_ext(movie.bink_high().type()), "h"s));
      movie.mutable_bink_low().file_name(
        generate_file_name(get_ext(movie.bink_low().type()), "l"s));
      m_movies.push_back(movie);
      movie = {};
    }
  }
  /**
   * Read complete pak file for offsets and sizes of each section.
   * @param path File path info
   */
  void
    read()
  {
    tools::read_from_file(
      [this](std::istream &is) {
        /**
         * Current working tmp movie clip
         */
        MovieClip movie{};
        while (!is.eof()) {
          auto type = FileSectionTypeT::get_type(is);
          if (FileSectionTypeT::valid_type(type,
                                           FileSectionTypeT::BIK,
                                           FileSectionTypeT::KB2)) {
            get_bik(is, movie, type);
          }
          else if (std::ranges::equal(type, FileSectionTypeT::CAM)) {
            get_cam(is, movie);
          }
          else if (!is.eof()) {
            std::cerr << "location: " << std::hex << is.tellg() << std::endl;
            std::cerr << "unknown\t\"" << type[0] << type[1] << type[2]
                      << std::dec << "\"" << std::endl;
            return;
          }
        }
      },
      m_file_path);
  }
  friend std::ostream &
    operator<<(std::ostream &os, const Pak &pak)
  {
    os << pak.m_file_path << '\n';
    os << "{| class=\"wikitable sortable\"\n! FileName !! Frames !! Offset !! "
          "Size !! Type\n";
    std::ranges::for_each(pak.m_movies, [&os](const MovieClip &item) mutable {
      os << item;
    });
    return os << "|}\n";
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_PAK_HPP