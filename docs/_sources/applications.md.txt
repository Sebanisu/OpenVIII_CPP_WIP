Applications
======

## FIFLFS

* fiflfs_extract
    * Extract files and create index of the file compression used.
```shell
fiflfs_extract [Source fi/fl/fs file] [Destination path]
```
* fiflfs_create
    * Use the index to recreate the archive.
```shell
fiflfs_create [Source path] [Destination path] [Archive name]
```

### Compression

These compress or uncompress a file. This is then formatted as it would be in the **FS** files.

* lz4_ff8
    * Remaster uses this.
```shell
lz4_ff8 [input uncompressed] [output compressed]
lz4_ff8 -d [input compressed] [output uncompressed]
```
* lzss_ff8
```shell
lzss_ff8 [input uncompressed] [output compressed]
lzss_ff8 -d [input compressed] [output uncompressed]
```
## Tile Map

* tile_map_to_csv
    * dump the map entries to a csv file. Also has a hex of the original entry. The hex loads first then the other fields.
    * mim is used to tell which type of map this is.
```shell
tile_map_to_csv [input mim] [input map]
```
* tile_csv_to_map
    * Take a csv file and convert it to a map.
    * The number in the filename .1 .2 or .3 tell what type of map this is.
```shell
tile_csv_to_map [input csv]
```
Tests
======
These functions have hard coded paths forcing code to run on each version of **FF8** I have installed.