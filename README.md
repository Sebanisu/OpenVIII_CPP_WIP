Credits:
 - https://github.com/MaKiPL
 - https://github.com/myst6re
 - https://github.com/antiquechrono

Reference:
 - http://wiki.ffrtt.ru/index.php?title=FF8/PC_Media
 - http://wiki.ffrtt.ru/index.php?title=FF7/LZSS_format
 - https://github.com/MaKiPL/OpenVIII-monogame
 - https://github.com/myst6re/deling
 
Requires:
 - cpp17
 - https://github.com/lz4/lz4

This is WIP and everything is subject to change. I plan to upgrade to cpp20 when it's more widely supported.

This Libary is for handling FI FL FS and ZZZ files in a read only fashion. I may add the ability to write changes back. Though that is a bit of work. As you have to recreate the containers from scratch and readd the elements.

Will have the ability to find files in the archive and extract the one you want. Unsure if we should store the filelist as a vector of strings. Depends how many times you could go through the files.
