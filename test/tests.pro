TEMPLATE = app
TARGET = run-tests
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/../libraries.pri)

DEFINES = TEST_DIR=$$PWD/files

INCLUDEPATH += ../src

SOURCES += \
	../src/arguments_parser.cpp \
	../src/argv_iterator.cpp \
	../src/directory.cpp \
	../src/directory_encoder.cpp \
	../src/filename.cpp \
	../src/lame_encoder.cpp \
	../src/posix_semaphore.cpp \
	../src/posix_thread.cpp \
	../src/thread_pool.cpp \
    argv_iterator_test.cpp \
    arguments_parser_test.cpp \
    filename_test.cpp \
    directory_test.cpp \
	lame_encoder_test.cpp \
    posix_semaphore_test.cpp \
    posix_thread_test.cpp \
    thread_pool_test.cpp

HEADERS += \
	../src/arguments_parser.h \
	../src/argv_iterator.h \
	../src/directory.h \
	../src/directory_encoder.h \
	../src/filename.h \
	../src/lame_encoder.h \
	../src/scope_execution_timer.h \
	../src/posix_semaphore.h \
	../src/posix_thread.h \
	../src/thread_pool.h

SOURCES += \
	main.cpp \

HEADERS += \
	tpunit++.hpp
