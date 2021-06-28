Applications
======

## FIFLFS

* fiflfs_extract
    * Extract files and create index of the file compression used.
* fiflfs_create
    * Use the index to recreate the archive.

### Compression

These compress or uncompress a file. This is then formatted as it would be in the **FS** files.

* lz4_ff8
* lzss_ff8

## Tile Map

* tile_map_to_csv
    * dump the map entries to a csv file. Also has a hex of the original entry. The hex loads first then the other fields.
* tile_csv_to_map
    * Take a csv file and convert it to a map.

Tests
======
These functions have hard coded paths forcing code to run on each version of **FF8** I have installed.