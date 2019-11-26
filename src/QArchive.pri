DEFINES += LIBARCHIVE_STATIC

#LIBS += -larchive # -lzlib -lliblzma

SOURCES += \
    DiskExtractor.cpp \
    DiskCompressor.cpp

HEADERS += \
        ../include/QArchive \
    DiskExtractor.hpp \
    DiskCompressor.hpp \
    Log.hpp
