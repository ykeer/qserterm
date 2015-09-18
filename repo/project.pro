lessThan(QT_MAJOR_VERSION, 5): error("requires Qt 5")

TEMPLATE     = app
TARGET       = uni

QT          += core gui qml quick serialport network printsupport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS     += src/lib/ql-settings.hpp src/lib/ql-files.hpp src/lib/ql-channel.hpp src/lib/ql-channel-serial.hpp src/lib/ql-channel-socket.hpp src/lib/ql-server.hpp src/lib/qcustomplot.h src/lib/ql-lineplot.hpp src/lib/ql-locale.hpp src/lib/ql-db-kvp-sqlite.hpp
SOURCES     += src/main.cpp src/lib/ql-settings.cpp src/lib/ql-files.cpp src/lib/ql-channel.cpp src/lib/ql-channel-serial.cpp src/lib/ql-channel-socket.cpp src/lib/ql-server.cpp src/lib/qcustomplot.cpp src/lib/ql-lineplot.cpp src/lib/ql-locale.cpp src/lib/ql-db-kvp-sqlite.cpp

lin {
    DESTDIR  = build/lin
    QMAKE_INCDIR += /usr/include/i386-linux-gnu
}

win {
    DESTDIR   = build/win
    LIBS     -= -lGl
    LIBS     += -L./dll_mingw -L./dll_mingw_stat
    QMAKE_CXX = /usr/bin/i686-w64-mingw32-g++

    QMAKE_INCDIR += /usr/i686-w64-mingw32/include

    QMAKE_CFLAGS_RELEASE += -Os -momit-leaf-frame-pointer
    QMAKE_LINK = /usr/bin/i686-w64-mingw32-g++
    QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -Wl,--nxcompat
    QMAKE_LFLAGS += -mthreads -Wl,-enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -mwindows
}

OBJECTS_DIR  = $$DESTDIR/.obj
MOC_DIR      = $$DESTDIR/.moc
RCC_DIR      = $$DESTDIR/.rcc
UI_DIR       = $$DESTDIR/.ui

QMAKE_CXXFLAGS += -std=gnu++0x -Wno-unused-parameter
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -Os

QMAKE_LFLAGS += -lstdc++

qml.path   = $$DESTDIR
qml.files += $$files(src/*.qml)

qmls.path   = $$DESTDIR
qmls.files += $$files(src/qml)

js.path    = $$DESTDIR
js.files  += $$files(src/js)

res.path    = $$DESTDIR
res.files  += $$files(src/res)

languages.path    = $$DESTDIR
languages.files  += $$files(languages)

static.path    = $$DESTDIR
static.files  += $$files(static)

db.path    = $$DESTDIR
db.files  += $$files(db)

INSTALLS  += qml qmls js res languages static db

