QMAKE_EXTRA_TARGETS = LameEncoderLib
win32 {
    LameEncoderLib.commands = mingw32-make libmp3lame/libmp3lame.a \
        -C $$PWD/lame-3.99.5 -f Makefile.unix \
        UNAME=MSDOS \
        CC_OPTS=\"-Wall -pipe -O3 -fomit-frame-pointer -ffast-math -funroll-loops -fschedule-insns2\"
}
unix {
    LameEncoderLib.commands = make libmp3lame/libmp3lame.a \
        -C $$PWD/lame-3.99.5 -f Makefile.unix \
        UNAME=Linux
}
PRE_TARGETDEPS = LameEncoderLib

INCLUDEPATH += $$PWD/lame-3.99.5/include
LIBS += -L$$PWD/lame-3.99.5/libmp3lame -lmp3lame

win32 {
	INCLUDEPATH += "C:/Program Files (x86)/Mega-Nerd/libsndfile/include"
	# This is the path to libsndfile-1.dll and it is needed to properly
	# run the debugger, but it fails to build with -static option needed
	# to eliminate dependencies on qt's runtime libraries
	LIBS += -L"C:/Program Files (x86)/Mega-Nerd/libsndfile/bin"
	LIBS += -L"C:/Program Files (x86)/Mega-Nerd/libsndfile/lib"
	QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++
	LIBS += -llibsndfile-1
}

unix {
	LIBS += -lsndfile -pthread
}
