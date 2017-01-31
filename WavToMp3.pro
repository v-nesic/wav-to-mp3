TEMPLATE = app
TARGET = wavtomp3
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LAME_DIR = $$PWD/lame-3.99.5
include($$PWD/libraries.pri)

SOURCES += src/main.cpp \
	src/arguments_parser.cpp \
	src/argv_iterator.cpp \
	src/directory.cpp \
	src/directory_encoder.cpp \
	src/filename.cpp \
	src/lame_encoder.cpp \
	src/posix_semaphore.cpp \
	src/posix_thread.cpp \
    src/thread_pool.cpp
	
HEADERS += \
	src/arguments_parser.h \
	src/argv_iterator.h \
	src/directory.h \
	src/directory_encoder.h \
	src/filename.h \
	src/lame_encoder.h \
	src/scope_execution_timer.h \
	src/posix_semaphore.h \
	src/posix_thread.h \
    src/thread_pool.h
