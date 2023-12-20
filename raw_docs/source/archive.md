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

The Archives object plays a crucial role in locating and organizing archives within a specified directory. Its primary function involves indexing these archives and facilitating a straightforward interface for reading the contained files.

```{eval-rst}
.. doxygenstruct:: open_viii::archive::Archives
    :members:
```

### ArchiveTypeT

The ArchiveTypeT enum encapsulates identifiers for different archive types in Final Fantasy VIII. There are six primary FIFLFS archives: battle, field, magic, main, menu, and world. Additionally, there are two main zzz archives for the FF8 remaster: zzz_main and zzz_other. These enums serve as convenient references for utilizing the "get<>" function in archives, allowing users to specify the desired archive type when interacting with the archives in the game code. The enum also provides utility constants such as 'count,' 'first,' 'last,' 'begin,' and 'end' for easy enumeration management.

```{eval-rst}
.. doxygenenum:: open_viii::archive::ArchiveTypeT
```

### FIFLFS

The FIFLFS archive format in Final Fantasy VIII for PC consists of three main components: File Index (FI), File Source (FS), and File List (FL). FI stores offset and size information, FS contains raw bytes, and FL holds virtual file paths. These elements collectively structure the archive, facilitating organized storage and retrieval of game assets.

```{eval-rst}
.. doxygenstruct:: open_viii::archive::FIFLFS
```

#### Grouping

Grouping is essentially an object that stores information about different components of the archive, serving as a reference for later reading. This includes details like file paths and offsets, enabling easy retrieval and interpretation.

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


ZZZ files initiate with a 4-byte file count, representing the number of File Data entries that follow. All entries within a ZZZ file remain uncompressed. To access an FS file within the ZZZ file, combine a given FI offset with the corresponding File Data offset.

```{eval-rst}
.. doxygenstruct:: open_viii::archive::ZZZ
    :members:
```

#### File Data

File Data entries feature variable sizes, requiring a one-at-a-time reading approach. To streamline this process, caching is employed. Each entry commences with a 32-bit size of the virtual file paths, where these paths are relative, such as `data\disk\disk1`. The entry then includes the corresponding virtual file paths, a 64-bit file offset, and a 32-bit file size.

```{eval-rst}
.. doxygenstruct:: open_viii::archive::FileData
    :members:
```