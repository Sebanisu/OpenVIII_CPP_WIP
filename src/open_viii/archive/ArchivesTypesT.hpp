//
// Created by pcvii on 2/10/2021.
//
#ifndef VIIIARCHIVE_ARCHIVESTYPEST_HPP
#define VIIIARCHIVE_ARCHIVESTYPEST_HPP
#include <concepts>
#include <cstdint>
namespace open_viii::archive {

/**
 * There are 6 main FIFLFS archives for ff8 and 2 2 main zzz archives for ff8
remaster. These enums are for get<> in archives to ask for the one you want.
 */
enum class ArchiveTypeT : std::uint8_t {
  battle = 0U,
  field,
  magic,
  main,
  menu,
  world,
  zzz_main,
  zzz_other,
  count,
  first = 0U,
  last  = count - 1U,
  begin = battle,
  end   = count,
};

/**
 * Confirm valid value of enum ArchiveTypeT
 * @param I ArchiveTypeT equivalent
 * @param include_end if true ArchiveTypeT::end is a valid value
 * @return true if valid.
 */
constexpr bool
  test_valid_archive_type_t(std::signed_integral auto I,
                            bool                      include_end = false)
{
  return ((static_cast<std::intmax_t>(I)
           >= static_cast<std::intmax_t>(ArchiveTypeT::first))
          && (static_cast<std::intmax_t>(I)
              <= static_cast<std::intmax_t>(ArchiveTypeT::last)))
      || (include_end
          && (static_cast<std::intmax_t>(I)
              == static_cast<std::intmax_t>(ArchiveTypeT::end)));
}

/**
 * Confirm valid value of enum ArchiveTypeT
 * @tparam I ArchiveTypeT equivalent
 * @tparam include_end if true ArchiveTypeT::end is a valid value
 */
template<auto I, bool include_end = false>
concept valid_archive_type_t_signed = test_valid_archive_type_t(I, include_end);
constexpr bool
  test_valid_archive_type_t(std::unsigned_integral auto I,
                            bool                        include_end = false)
{
  return static_cast<std::size_t>(I)
        <= static_cast<std::size_t>(ArchiveTypeT::last)
      || (include_end
          && (static_cast<std::size_t>(I)
              == static_cast<std::size_t>(ArchiveTypeT::end)));
}

/**
 * Confirm valid value of enum ArchiveTypeT
 * @tparam I ArchiveTypeT equivalent
 * @tparam include_end if true ArchiveTypeT::end is a valid value
 */
template<auto I, bool include_end = false>
concept valid_archive_type_t_unsigned = test_valid_archive_type_t(I,
                                                                  include_end);

/**
 * Confirm valid value of enum ArchiveTypeT
 * @param I ArchiveTypeT equivalent
 * @param include_end if true ArchiveTypeT::end is a valid value
 * @return true if valid.
 */
constexpr bool
  test_valid_archive_type_t(ArchiveTypeT I, bool include_end = false)
{
  return test_valid_archive_type_t(static_cast<std::size_t>(I), include_end);
}

/**
 * Confirm valid value of enum ArchiveTypeT
 * @tparam I ArchiveTypeT equivalent
 * @tparam include_end if true ArchiveTypeT::end is a valid value
 */
template<ArchiveTypeT I, bool include_end = false>
concept valid_archive_type_t_enum = test_valid_archive_type_t(I, include_end);

/**
 * Confirm valid value of enum ArchiveTypeT
 * @tparam I ArchiveTypeT equivalent
 * @tparam include_end if true ArchiveTypeT::end is a valid value
 */
template<auto I, bool include_end = false>
concept valid_archive_type_t =
  valid_archive_type_t_signed<I, include_end> || valid_archive_type_t_unsigned<
    I,
    include_end> || valid_archive_type_t_enum
  < static_cast<ArchiveTypeT>(I)
, include_end > ;

static_assert(valid_archive_type_t<ArchiveTypeT::battle>);
static_assert(valid_archive_type_t<ArchiveTypeT::field>);
static_assert(valid_archive_type_t<ArchiveTypeT::magic>);
static_assert(valid_archive_type_t<ArchiveTypeT::main>);
static_assert(valid_archive_type_t<ArchiveTypeT::menu>);
static_assert(valid_archive_type_t<ArchiveTypeT::world>);
static_assert(valid_archive_type_t<ArchiveTypeT::zzz_main>);
static_assert(valid_archive_type_t<ArchiveTypeT::zzz_other>);
static_assert(valid_archive_type_t<ArchiveTypeT::first>);
static_assert(valid_archive_type_t<ArchiveTypeT::last>);
static_assert(valid_archive_type_t<ArchiveTypeT::begin>);
static_assert(!valid_archive_type_t<ArchiveTypeT::end>);
static_assert(valid_archive_type_t<ArchiveTypeT::end, true>);
static_assert(!valid_archive_type_t<-1>);
static_assert(!valid_archive_type_t<99>);
static_assert(!valid_archive_type_t<99U>);
}// namespace open_viii::archive
#endif// VIIIARCHIVE_ARCHIVESTYPEST_HPP
