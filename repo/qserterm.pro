TEMPLATE     = app
TARGET       = qserterm

QT           = core gui quick qml serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS     += src/lib/ql-settings.hpp src/lib/ql-files.hpp src/lib/ql-server.hpp src/lib/ql-channel.hpp src/lib/ql-channel-serial.hpp
SOURCES     += src/main.cpp src/lib/ql-settings.cpp src/lib/ql-files.cpp src/lib/ql-server.cpp src/lib/ql-channel.cpp src/lib/ql-channel-serial.cpp

lin {
    DESTDIR  = build/lin
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

#CONFIG      += static

QMAKE_CXXFLAGS += -std=gnu++0x -Wno-unused-parameter
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -Os

#QMAKE_LFLAGS += -static -lstdc++

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

INSTALLS  += qml qmls js res languages

