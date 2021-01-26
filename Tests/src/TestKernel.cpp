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
  auto write = [&os, first = true](const auto &value) mutable {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << value;
  };
  if constexpr (requires(KernelElementT        ke,
                         std::ostream          tstream,
                         std::span<const char> tspan) {
                  ke.out(tstream, tspan);
                }) {
    kernel_element.out(os, buffer);
  } else {
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
    IF_EXIST_WRITE_DECODE_STRING(name_offset)
    IF_EXIST_WRITE_DECODE_STRING(description_offset)
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
  const auto execution_lambda = [](const std::filesystem::path &path) {
    std::cout << path << std::endl;
    const auto coo      = open_viii::LangT::en;
    const auto archives = open_viii::archive::Archives<coo>(path);
    [[maybe_unused]] const auto &main =
      archives.get<open_viii::archive::ArchiveTypeT::main>();
    std::cout << main << std::endl;
    auto kernel = open_viii::kernel::Header<coo>{ main };
    [[maybe_unused]] const auto &buffer = kernel.buffer();
    std::cout << "kernel.bin " << buffer.size() << " bytes; "
              << kernel.section_count() << " section count\n";
    std::cout << static_cast<int>(open_viii::kernel::SectionTypesT::count)
              << std::endl;
    // const auto &sectionOffsets = kernel.SectionOffsets();
    //    std::for_each(
    //      sectionOffsets.begin(), sectionOffsets.end(),[](const auto &value) {
    //      std::cout << value << std::endl; });
    //    kernel.static_for<static_cast<int>(open_viii::kernel::SectionTypesT::first),
    //      static_cast<int>(open_viii::kernel::SectionTypesT::count)>([](const
    //      std::string_view &string,const std::span<const char> & span, const
    //      auto & data) { std::cout << "  " << string << " - " <<
    //      std::ranges::size(span)
    //      << " bytes\n";
    //
    //      return data;
    //    });
    kernel.static_for([]<typename kernel_section_t>(
      const std::string_view &     string,
      const std::span<const char> &span,
      const kernel_section_t
        &data) requires(!std::is_null_pointer_v<kernel_section_t>) {
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
  execution_lambda(
    "/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII "
    "Remastered");
  // open_viii::Paths::for_each_path(execution_lambda);
  return 0;
}