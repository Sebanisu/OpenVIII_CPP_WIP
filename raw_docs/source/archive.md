Archive Library
=====

The Archive Library supports reading and writing files from the PC releases of the game.

[File List](#file-list) contains one or more path strings. Each starting with `C:\ff8\Data\` ending with [`\r\n`](https://en.wikipedia.org/wiki/Newline#Representation). These are in the same order as
the [File Index](#file-index). You scan the [File List](#file-list) to find the items you want, while doing this you would increment a counter. The counter is
used to know which entry to get from [File Index](#file-index). Lastly, you use the [File Index](#file-index) to read from the [File Source](#file-source).

[ZZZ](#zzz) contains multiple files including [FLFIFS](#fiflfs) archives of different languages. The [ZZZ](#zzz) header starts with the number of entries
followed by [FileData](#filedata) entries. [File Source](#file-source) functions can accept [FileData](#filedata) entries to extract from [ZZZ](#zzz) files.

**References:**
* <http://wiki.ffrtt.ru/index.php?title=FF8/PC_Media>
* <https://github.com/myst6re/deling>
* <https://github.com/MaKiPL/OpenVIII-monogame>

## Archives

```{eval-rst}
.. doxygenstruct:: open_viii::archive::Archives
    :members:
```

### ArchiveTypeT

```{eval-rst}
.. doxygenenum:: open_viii::archive::ArchiveTypeT
```

### FIFLFS

```{eval-rst}
.. doxygenstruct:: open_viii::archive::FIFLFS
```

#### Grouping

```{eval-rst}
.. doxygenstruct:: open_viii::archive::Grouping
    :members:
```

##### File List


The File List (FL) consists of entries with virtual paths starting with c:\ff8\. Each path is separated by a new line in Windows format (CRLF), maintaining the same order as the File Index (FI) entries.

```{eval-rst}
.. doxygennamespace:: open_viii::archive::fl
```

##### File Index

The File Index (FI) contains information such as the offset, uncompressed file size, and compression type, with each entry occupying 12 bytes. To determine the total number of files, divide the size of the FI file by 12.

```{eval-rst}
.. doxygenstruct:: open_viii::archive::FI
    :members: uncompressed_size, offset, compression_type, SIZE, EXT
```

##### File Source

File Source (FS) files store raw bytes of entries. For LZSS entries, the file begins with 4 bytes indicating the compressed file size. L4Z entries have a 12-byte header, including section size, an unused 4 bytes, and uncompressed size. The compressed size can be calculated by subtracting 8 from the section size.

```{eval-rst}
.. doxygennamespace:: open_viii::archive::FS
```

### ZZZ

```{eval-rst}
.. doxygenstruct:: open_viii::archive::ZZZ
    :members:
```

#### FileData

```{eval-rst}
.. doxygenstruct:: open_viii::archive::FileData
    :members:
```