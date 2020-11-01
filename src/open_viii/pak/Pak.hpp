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
 * @see https://github.com/MaKiPL/FF8-Rinoa-s-Toolset/tree/master/SerahToolkit_SharpGL/FF8_Core
 */
struct Pak
{
private:
  /**
   * Each frame section of cam file is this many bytes
   */
  static constexpr auto CAM_SECTION_SIZE = static_cast<char>(0x2C);


  /**
   * If false don't extract low res videos!
   */

  static constexpr auto ENABLE_EXTRACT_LOW_RES = false;


  /**
   *  Known valid Bink video formats "a,d,f,g,h,i"
   */
  static constexpr std::array<char, 6> BIK1 = { 0x61, 0x64, 0x66, 0x67, 0x68, 0x69 };
  /**
   * Known valid Bink 2 video formats "b,d,f,g,h,i"
   */
  static constexpr std::array<char, 6> BIK2 = { 0x62, 0x64, 0x66, 0x67, 0x68, 0x69 };

  /**
   * Each Movie has 1 cam and 2 versions of the video.
   */
  std::vector<MovieClip> m_movies;

  ///// <summary>
  ///// Depending on type you read it differently.
  ///// </summary>
  // private readonly Dictionary<Type, Func<BinaryReader, Type, FileSection>> _readFunctions;
  //  template<FileSectionTypeT fileSectionTypeT> readFunctions()
  //  {
  //  }
  //}

  /**
   * Remembers detected disc number.
   */
  int m_disc_cache{ -1 };

  std::filesystem::path m_file_path{};

public:
  explicit Pak(std::filesystem::path path) : m_file_path(std::move(path))
  {
//    static constexpr auto default_size = 7U;
//    m_movies.reserve(default_size);
    //  _readFunctions = new Dictionary<Type, Func<BinaryReader, Type, FileSection>>()
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
  [[nodiscard]] auto count() const noexcept
  {
    return std::ranges::size(m_movies);
  }

  /**
   * Current path
   * @return ref to path of pak file.
   */
  [[maybe_unused]] [[nodiscard]] const auto &file_path() const noexcept
  {
    return m_file_path;
  }

  /**
   * Each Movie has 1 cam and 2 versions of the video.
   * @return vector of movies.
   */
  [[maybe_unused]] [[nodiscard]] const auto &movies() const noexcept
  {
    return m_movies;
  }


  /**
   * Gets Movie Clip at index
   * @param i = index
   * @return Movie clip
   */
  [[nodiscard]] const auto &operator[](const std::size_t i) const noexcept
  {
    return m_movies[i];
  }
  /**
   * Gets Movie Clip at index
   * @param i = index
   * @return Movie clip
   */
  [[nodiscard]] const auto &at(const std::size_t i) const noexcept
  {
    return m_movies.at(i);
  }


  void extract(std::filesystem::path dest_path)
  {
    std::cout << "Extracting \"" << m_file_path.string() << "\"\n";
    Tools::read_buffer(
      [this, &dest_path](std::istream &is) {
        std::ranges::for_each(m_movies, [&is, &dest_path](MovieClip item) {
          const auto e = [&dest_path, &is](FileSection &fs) {
            auto out_path = dest_path / fs.FileName;
            if ([&out_path, &fs]() -> bool {
                  if (std::filesystem::exists(out_path)) {
                    return std::filesystem::file_size(out_path) != fs.Size;
                  }
                  return true;
                }()) {

              if(!std::ranges::empty(fs.FileName) && fs.Size>0U) {
                Tools::write_buffer(
                  [&is, &fs](std::ostream &os) {
                    std::cout << "Extracting \"" << fs.FileName << "\"\n";
                    is.seekg(fs.Offset, std::ios::beg);
                    std::vector<char> tmp{};
                    tmp.resize(fs.Size);
                    is.read(std::ranges::data(tmp), static_cast<std::intmax_t>(fs.Size));
                    os.write(std::ranges::data(tmp), static_cast<std::intmax_t>(fs.Size));
                  },
                  out_path.string());
              }
            }
          };

          e(item.Cam);
          e(item.BinkHigh);
          if constexpr (ENABLE_EXTRACT_LOW_RES) {
            e(item.BinkLow);
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
  std::string generate_file_name(const std::string &extension, const std::string &suffix = "")
  {
    /**
     * number values should be 2 digit.
     */
    const auto pad = [](std::integral auto i) -> std::string {
      if (i >= 0) {
        std::string return_val = std::to_string(i);
        static constexpr auto breakpoint = 10;
        if (i < breakpoint) {
          return_val = "0" + return_val;
        }

        return return_val;
      }
      return "00";
    };
    return std::string("disc") + pad(get_disc_number()) + std::string("_") + pad(count()) + suffix + extension;
  }
  /**
   * Extract number from filename. typically there is 1 digit for the disc number.
   * @return disc number.
   */
  int get_disc_number()
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
    if(m_disc_cache <= 0) {
      return 4;
}
    return m_disc_cache-1;
  }
  /**
   * Read complete pak file for offsets and sizes of each section.
   * @param path File path info
   */
  void read()
  {
    Tools::read_buffer(
      [this](std::istream &is) {
        /**
         * Current working tmp movie clip
         */
        MovieClip movie{};
        while (!is.eof()) {
          const auto get_type = [&is]() {
            const auto sz = sizeof(std::uint32_t) - 1;
            std::array<char, sz> tmp{};
            is.read(std::ranges::data(tmp), sz);
            return tmp;
          };
          auto type = get_type();
          if (std::ranges::equal(type, FileSectionTypeT::BIK) || std::ranges::equal(type, FileSectionTypeT::KB2)) {
            //std::cout << "bink\n";
            /**
             * Read Bink video offset and size
             */
            [&is, &type, this, &movie]() -> void {
              char version{};
              is.read(&version, sizeof(version));

              FileSection fs{};

              if (std::ranges::equal(type, FileSectionTypeT::BIK) && Tools::any_of(version, BIK1)) {
                fs.Type = FileSectionTypeT::BIK;
              } else if (std::ranges::equal(type, FileSectionTypeT::KB2) && Tools::any_of(version, BIK2)) {
                fs.Type = FileSectionTypeT::KB2;
              } else {
                std::cerr << "location: " << std::hex << is.tellg() << std::endl;
                std::cerr << "bink version and type mismatch...\t" << fs.Type << '\t'
                          << static_cast<std::int32_t>(version) << std::dec << std::endl;
                return;
              }
              fs.Offset = static_cast<std::int64_t>(is.tellg()) - 4;

              {
                static constexpr auto sz = sizeof(std::uint32_t);
                std::array<char, sz> tmp{};

                is.read(std::ranges::data(tmp), sz);
                std::memcpy(&fs.Size, std::ranges::data(tmp), sz);

                static constexpr auto header_size = 8U;
                fs.Size += header_size;

                is.read(std::ranges::data(tmp), sz);
                std::memcpy(&fs.Frames, std::ranges::data(tmp), sz);
              }
              is.seekg(fs.Offset + fs.Size, std::ios::beg);

              if (std::ranges::empty(movie.BinkHigh.Type)) {
                movie.BinkHigh = fs;
                movie.BinkHigh.FileName =
                  generate_file_name(movie.BinkHigh.Type == FileSectionTypeT::BIK ? ".bik" : ".bk2", "h");
              } else {
                if (fs.Size > movie.BinkHigh.Size) {
                  movie.BinkLow = movie.BinkHigh;
                  movie.BinkHigh = fs;
                } else {
                  movie.BinkLow = fs;
                }

                movie.BinkHigh.FileName =
                  generate_file_name(movie.BinkLow.Type == FileSectionTypeT::BIK ? ".bik" : ".bk2", "h");
                movie.BinkLow.FileName =
                  generate_file_name(movie.BinkLow.Type == FileSectionTypeT::BIK ? ".bik" : ".bk2", "l");
                m_movies.push_back(movie);
                movie = {};
              }
            }();

          } else if (std::ranges::equal(type, FileSectionTypeT::CAM)) {
           // std::cout << "cam\n";
            /**
             * Read cam file offset and size
             */
            [&is, this, &get_type, &movie]() -> void {
              FileSection fs{};
              fs.Type = FileSectionTypeT::CAM;
              fs.Offset = static_cast<std::int64_t>(is.tellg()) - 3;

              is.seekg(3, std::ios::cur);
              std::uint16_t frames{};
              {
                const auto sz = sizeof(std::uint16_t);
                std::array<char, sz> tmp{};

                is.read(std::ranges::data(tmp), sz);
                std::memcpy(&frames, std::ranges::data(tmp), sz);
              }

              is.seekg(frames * CAM_SECTION_SIZE, std::ios::cur);

              // there seems to be 1 or more extra frames. Check for those.


              while ([&get_type]() {
                auto b = get_type();
                return !std::ranges::equal(b, FileSectionTypeT::BIK) && !std::ranges::equal(b, FileSectionTypeT::KB2);
              }()) {
                is.seekg(CAM_SECTION_SIZE - 3, std::ios::cur);
                frames++;
              }
              // Found the end go back to it.

              is.seekg(-static_cast<long>((sizeof(std::uint32_t) - 1)), std::ios::cur);

              // There is only one cam per movie. Checking for possibility of only one video instead of the normal 2 per
              // movie.
//              if (std::ranges::empty(movie.Cam.Type)) {
//                //              //add existing movie to movies list.
//                m_movies.emplace_back(std::move(movie));
//                // start from scratch
//                movie = {};
//              }
              fs.Size = static_cast<std::uint32_t>(is.tellg() - fs.Offset);
              fs.FileName = generate_file_name(".cam");
              fs.Frames = frames;
              movie.Cam = fs;
            }();

          } else if(!is.eof()) {
            std::cerr << "location: " << std::hex << is.tellg() << std::endl;
            std::cerr << "unknown\t\"" << type[0]  << type[1]  << type[2] << std::dec <<"\""<< std::endl;;
            return;
          }
        }
      },
      m_file_path);
  }
  friend std::ostream &operator<<(std::ostream &os, const Pak &pak)
  {
    os << pak.m_file_path << '\n';
    os<<"{| class=\"wikitable sortable\"\n! FileName !! Frames !! Offset !! Size !! Type\n";
    std::ranges::for_each(pak.m_movies, [&os](const MovieClip &item) mutable {
      os << item;
    });
    return os << "|}\n";
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_PAK_HPP
