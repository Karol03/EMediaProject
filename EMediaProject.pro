TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    wavloader.cpp \
    fft.cpp \
    gnuplot.cpp

HEADERS += \
    wavloader.hpp \
    fft.hpp \
    gnuplot.hpp
