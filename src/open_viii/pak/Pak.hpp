//
// Created by pcvii on 10/30/2020.
//
#ifndef VIIIARCHIVE_PAK_HPP
#define VIIIARCHIVE_PAK_HPP
#include "MovieClip.hpp"
#include "open_viii/tools/Tools.hpp"
#include <array>
#include <filesystem>
#include <fmt/format.h>
#include <span>
#include <vector>

namespace open_viii::pak {

/**
 * PAK reading struct, Gathers all the sections
 */
struct Pak
{
  // Constants
  static constexpr auto CAM_SECTION_SIZE       = static_cast<char>(0x2C);
  static constexpr auto ENABLE_EXTRACT_LOW_RES = true;

  static constexpr std::array<char, 6> BIK1{
    0x61, 0x64, 0x66, 0x67, 0x68, 0x69
  };
  static constexpr std::array<char, 6> BIK2{
    0x62, 0x64, 0x66, 0x67, 0x68, 0x69
  };

  // Public data
  std::vector<MovieClip> movies{};
  int                    disc_cache{ -1 };
  std::filesystem::path  file_path{};

  explicit Pak(std::filesystem::path path) : file_path(std::move(path))
  {
    read();
  }

  [[nodiscard]] auto
    count() const noexcept
  {
    return std::ranges::size(movies);
  }

  [[nodiscard]] const auto &
    operator[](std::size_t i) const noexcept
  {
    return movies[i];
  }
  [[nodiscard]] const auto &
    at(std::size_t i) const
  {
    return movies.at(i);
  }

  void
    extract(std::filesystem::path dest_path)
  {
    fmt::print("Extracting \"{}\"\n", file_path.string());

    tools::read_from_file(
      [this, &dest_path](std::istream &is) {
        for (const auto &item : movies) {
          const auto extract_one = [&](const FileSection &fs) {
            auto       out_path     = dest_path / fs.file_name;

            const bool should_write = [&]() -> bool {
              std::error_code ec{};
              const bool      exists = std::filesystem::exists(out_path, ec);

              if (ec) {
                fmt::print(
                  stderr,
                  "error {}:{} - {}: {} - path: {}\n",
                  __FILE__,
                  __LINE__,
                  ec.value(),
                  ec.message(),
                  out_path.string());
                return false;
              }

              if (exists) {
                const auto sz = std::filesystem::file_size(out_path, ec);
                if (ec) {
                  fmt::print(
                    stderr,
                    "error {}:{} - {}: {} - path: {}\n",
                    __FILE__,
                    __LINE__,
                    ec.value(),
                    ec.message(),
                    out_path.string());
                  return false;
                }
                return sz != fs.size;
              }
              return true;
            }();

            if (should_write && !fs.file_name.empty() && fs.size > 0) {
              tools::write_buffer(
                [&](std::ostream &os) {
                  fmt::print("Extracting \"{}\"\n", fs.file_name);

                  is.seekg(fs.offset, std::ios::beg);
                  std::vector<char> tmp(fs.size);

                  is.read(tmp.data(), static_cast<std::streamsize>(fs.size));
                  os.write(tmp.data(), static_cast<std::streamsize>(fs.size));
                },
                out_path.string());
            }
          };

          extract_one(item.cam_fs);
          extract_one(item.bink_high);
          if constexpr (ENABLE_EXTRACT_LOW_RES)
            extract_one(item.bink_low);
        }
      },
      file_path);
  }

  std::string
    generate_file_name(
      const std::string &extension,
      const std::string &suffix = {})
  {
    using namespace std::string_literals;
    static constexpr auto length = 2U;

    return "disc"s + tools::to_string_with_padding(get_disc_number(), length)
         + "_"s + tools::to_string_with_padding(count(), length) + suffix
         + extension;
  }

  int
    get_disc_number()
  {
    auto stem = file_path.stem().string();

    if (disc_cache == -1) {
      auto it = std::ranges::find_if(stem, [](char c) {
        return std::isdigit(c);
      });
      if (it != stem.end())
        disc_cache = *it - '0';
    }

    if (disc_cache <= 0)
      return 4;

    return disc_cache - 1;
  }

  void
    get_cam(std::istream &is, MovieClip &movie)
  {
    FileSection fs{};
    fs.type   = FileSectionTypeT::CAM;
    fs.offset = static_cast<int64_t>(is.tellg()) - 3;

    is.seekg(3, std::ios::cur);

    auto frames = tools::read_val<uint16_t>(is);
    is.seekg(frames * CAM_SECTION_SIZE, std::ios::cur);

    while (!FileSectionTypeT::valid_type(
      FileSectionTypeT::get_type(is),
      FileSectionTypeT::BIK,
      FileSectionTypeT::KB2)) {
      is.seekg(CAM_SECTION_SIZE - 3, std::ios::cur);
      frames++;
    }

    is.seekg(-3, std::ios::cur);

    fs.size
      = static_cast<uint32_t>(is.tellg()) - static_cast<uint32_t>(fs.offset);
    fs.file_name = generate_file_name(".cam");
    fs.frames    = frames;

    movie.cam    = Cam(is, fs);
    movie.cam_fs = std::move(fs);
  }

  void
    get_bik(std::istream &is, MovieClip &movie, std::span<const char> type)
  {
    char        version = tools::read_val<char>(is);
    FileSection fs{};

    if (
      std::ranges::equal(type, FileSectionTypeT::BIK)
      && std::ranges::any_of(BIK1, [&](auto c) {
           return version == c;
         })) {
      fs.type = FileSectionTypeT::BIK;
    }
    else if (
      std::ranges::equal(type, FileSectionTypeT::KB2)
      && std::ranges::any_of(BIK2, [&](auto c) {
           return version == c;
         })) {
      fs.type = FileSectionTypeT::KB2;
    }
    else {
      fmt::print(
        stderr,
        "bink mismatch at {:x}, type={} version={}\n",
        static_cast<long long>(is.tellg()),
        fs.type,
        static_cast<int>(version));
      return;
    }

    fs.offset = static_cast<int64_t>(is.tellg()) - 4;
    fs.size   = tools::read_val<uint32_t>(is) + 8;
    fs.frames = tools::read_val<uint32_t>(is);

    is.seekg(fs.offset + fs.size, std::ios::beg);

    const auto ext = (fs.type == FileSectionTypeT::BIK) ? ".bik" : ".bk2";

    if (movie.bink_high.type.empty()) {
      movie.bink_high           = fs;
      movie.bink_high.file_name = generate_file_name(ext, "h");
    }
    else {
      if (fs.size > movie.bink_high.size) {
        movie.swap_bink();
        movie.bink_high = fs;
      }
      else {
        movie.bink_low = fs;
      }

      movie.bink_high.file_name = generate_file_name(ext, "h");
      movie.bink_low.file_name  = generate_file_name(ext, "l");

      movies.push_back(movie);
      movie = {};
    }
  }

  void
    read()
  {
    tools::read_from_file(
      [this](std::istream &is) {
        MovieClip movie{};

        while (!is.eof()) {
          auto type = FileSectionTypeT::get_type(is);

          if (
            FileSectionTypeT::valid_type(
              type,
              FileSectionTypeT::BIK,
              FileSectionTypeT::KB2))
            get_bik(is, movie, type);
          else if (std::ranges::equal(type, FileSectionTypeT::CAM))
            get_cam(is, movie);
          else if (!is.eof()) {
            fmt::print(
              stderr,
              "unknown type at {:x}: \"{}{}{}\"\n",
              static_cast<long long>(is.tellg()),
              type[0],
              type[1],
              type[2]);
            return;
          }
        }
      },
      file_path);
  }
};

}// namespace open_viii::pak

template<>
struct fmt::formatter<open_viii::pak::Pak>
{
  constexpr auto
    parse(format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const open_viii::pak::Pak &v, FormatContext &ctx) const
  {
    auto out = ctx.out();

    out      = fmt::format_to(
      out,
      "{}\n{{| class=\"wikitable sortable\"\n"
      "! FileName !! Frames !! Offset !! Size !! Type\n",
      v.file_path.string());

    for (const auto &m : v.movies)
      out = fmt::format_to(out, "{}", m);

    return fmt::format_to(out, "|}}\n");
  }
};
#endif// VIIIARCHIVE_PAK_HPP