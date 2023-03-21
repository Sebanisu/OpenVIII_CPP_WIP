//
// Created by pcvii on 3/21/2023.
//

#ifndef OPENVIII_CPP_WIP_TRYADDT_HPP
#define OPENVIII_CPP_WIP_TRYADDT_HPP

/**
 * @brief Enumeration representing the result of trying to add an entry to an archive.
 *
 * This enum class defines three possible outcomes when attempting to add an entry to an archive:
 * 1. not_part_of_archive: The entry is not a part of the archive.
 * 2. added_to_archive: The entry has been successfully added to the archive.
 * 3. archive_full: The archive is full and cannot accept any more entries.
 */
enum class TryAddT
{
  not_part_of_archive,
  added_to_archive,
  archive_full
};

#endif// OPENVIII_CPP_WIP_TRYADDT_HPP

