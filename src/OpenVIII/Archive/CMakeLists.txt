add_library(VIIIArchive
        FI.h
        FIFLFS.h
        FL.h
        FS.h
        ZZZ.h
        FileData.h
        Archive.h
        Archive.cpp
        ZZZ.cpp
        FS.cpp
        FL.cpp
        FileData.cpp
        FIFLFS.cpp
        FI.cpp Archives.cpp Archives.h CompressionTypeT.cpp CompressionTypeT.h ../MenuGroup/mngrphd.cpp ../MenuGroup/mngrphd.h)
set_target_properties(VIIIArchive PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(VIIIArchive VIIITools VIIICompression)