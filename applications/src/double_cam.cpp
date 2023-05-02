//
// Created by pcvii on 8/17/2021.
//
#include "double_cam.hpp"
void
  process_cam(const std::filesystem::path &);
int
  main(const int argc, const char *const argv[])
{
  const auto span_argv
    = std::span(argv, static_cast<std::size_t>(argc))
    | std::views::transform([](const char *const local_argv) {
        return std::filesystem::path(local_argv);
      });
  const auto validate_is_cam
    = [](const std::filesystem::path &local_path) -> bool {
    std::error_code error_code{};
    const bool      found
      = local_path.has_extension()
     && open_viii::tools::i_equals(local_path.extension().string(), ".cam")
     && std::filesystem::exists(local_path, error_code);
    if (error_code) {
      std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                << error_code.value() << ": " << error_code.message()
                << error_code.value() << " - path: " << local_path << std::endl;
      error_code.clear();
    }
    return found;
  };
  for (const auto path : span_argv) {
    if (!validate_is_cam(path)) {
      continue;
    }
    process_cam(path);
  }
}

void
  process_cam(const std::filesystem::path &path)
{
  std::cout << path << std::endl;
  open_viii::tools::read_from_file(
    [&path](std::istream &input_stream) {
      const auto cam        = open_viii::pak::Cam{ input_stream };
      auto       new_frames = std::vector<open_viii::pak::CamFrame>{};

      double_input_transform(
        cam,
        std::back_inserter(new_frames),
        [](
          const open_viii::pak::CamFrame &cam0,
          const open_viii::pak::CamFrame &cam1) {
          return cam0.midpoint(cam1);
        });
      const auto new_cam = cam.with_frames(std::move(new_frames));

      std::cout << "old size:\t" << cam.size() << std::endl;
      std::cout << "new size:\t" << new_cam.size() << std::endl;
      const auto new_filename
        = path.stem().string() + "_new" + path.extension().string();
      new_cam.save(new_filename);
    },
    path);
}