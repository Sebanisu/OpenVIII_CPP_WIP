// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include "TestKernel.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/kernel/Header.hpp"
#include "open_viii/paths/Paths.hpp"
/**
 * Dumps values to stream
 * @param os output stream
 * @param buffer contains encoded names and descriptions
 */
template<open_viii::LangT langVal, typename KernelElementT>
static void out(KernelElementT               kernel_element,
                std::ostream &               os,
                const std::span<const char> &buffer)
{
#define IF_EXIST_WRITE_DECODE_STRING(function_name)                            \
  {                                                                            \
    if constexpr (requires(KernelElementT ke) { ke.function_name(); }) {       \
      if (!std::ranges::empty(buffer)) {                                       \
        const auto decoded_string =                                            \
          kernel_element.function_name().template decoded_string<langVal>(     \
            buffer);                                                           \
        if (!std::empty(decoded_string)) {                                     \
          write(open_viii::tools::u8_to_sv(decoded_string));                   \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }
#define IF_EXIST_WRITE(function_name)                                          \
  {                                                                            \
    if constexpr (requires(KernelElementT ke) { ke.function_name(); }) {       \
      write(kernel_element.function_name());                                   \
    }                                                                          \
  }
#define IF_EXIST_WRITE_CASTINT(function_name)                                  \
  {                                                                            \
    if constexpr (requires(KernelElementT ke) { ke.function_name(); }) {       \
      write(static_cast<std::uint32_t>(kernel_element.function_name()));       \
    }                                                                          \
  }
  auto write = [&os, first = true](const auto &value) mutable {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << value;
  };
  IF_EXIST_WRITE_DECODE_STRING(name_offset)
  IF_EXIST_WRITE_DECODE_STRING(description_offset)
  if constexpr (requires(KernelElementT        ke,
                         std::ostream          tstream,
                         std::span<const char> tspan) {
                  ke.out(tstream, tspan);
                }) {
    kernel_element.out(os, buffer);
  } else {
    IF_EXIST_WRITE_CASTINT(ability_points_required_to_unlock)
    IF_EXIST_WRITE_CASTINT(ability_data_id)
    IF_EXIST_WRITE_CASTINT(unknown_flags)
    IF_EXIST_WRITE_CASTINT(get_target)
    IF_EXIST_WRITE_CASTINT(unknown)
    IF_EXIST_WRITE_CASTINT(index_to_battle_command)
    IF_EXIST_WRITE_CASTINT(unknown0)
    IF_EXIST_WRITE_CASTINT(unknown1)
    IF_EXIST_WRITE_CASTINT(unknown2)
    IF_EXIST_WRITE_CASTINT(magic_id)
    IF_EXIST_WRITE_CASTINT(attack_type)
    IF_EXIST_WRITE_CASTINT(attack_power)
    IF_EXIST_WRITE_CASTINT(attack_flags)
    IF_EXIST_WRITE_CASTINT(element)
    IF_EXIST_WRITE_CASTINT(status_attack_enabler)
    IF_EXIST_WRITE_CASTINT(attack_parameter)
    IF_EXIST_WRITE_CASTINT(m_persistent_statuses)
    IF_EXIST_WRITE_CASTINT(battle_only_statuses)
    IF_EXIST_WRITE(camera_change)
#undef IF_EXIST_WRITE_CASTINT
#undef IF_EXIST_WRITE
#undef IF_EXIST_WRITE_DECODE_STRING
  }
}
int main()
{
  std::vector<std::pair<std::filesystem::path, open_viii::kernel::Header>>
             kernels{};
  const auto execution_lambda = [&kernels](const std::filesystem::path &path) {
    std::cout << path << std::endl;
    static constexpr auto coo      = open_viii::LangT::en;
    const auto archives = open_viii::archive::Archives(path, open_viii::LangCommon::to_string<coo>());
    if(!static_cast<bool>(archives))
    {
      std::cerr << "Failed to load path: " << path.string();
      return;
    }
    [[maybe_unused]] const auto &main =
      archives.get<open_viii::archive::ArchiveTypeT::main>();
    std::cout << main << std::endl;
    auto &kernel = kernels.emplace_back(path, main).second;
    [[maybe_unused]] const auto &buffer = kernel.buffer();
    std::cout << "kernel.bin " << buffer.size() << " bytes; "
              << kernel.section_count() << " section count\n";
    std::cout << static_cast<int>(open_viii::kernel::SectionTypesT::count)
              << std::endl;
    kernel.static_for([](const std::string_view &     string,
                         const std::span<const char> &span,
                         const auto &                 data) {
      std::cout << string << " ( " << std::ranges::size(span) << "bytes) has "
                << std::ranges::size(data) << " entries\n";
      for (size_t i = 0; i < data.size(); i++) {
        auto entry = data.at(i);
        if (entry != decltype(entry){}) {
          std::cout << i << ": ";
          out<coo>(entry, std::cout, data.text_span());
          std::cout << '\n';
        }
      }
    });
  };
  open_viii::Paths::for_each_path(execution_lambda);
  int i = 0;
  for (const auto &[path, kernel] : kernels) {
    for (const auto &[other_path, other_kernel] :
         kernels | std::views::drop(++i)) {
      std::cout << "Comparing " << open_viii::kernel::Header::FILE_NAME << " {"
                << path.string() << ", " << other_path.string() << "}:\n";
      std::cout << (std::ranges::equal(kernel.buffer(),other_kernel.buffer()) ? "equal" : "not equal") << '\n';
    }
  }
  return 0;
}