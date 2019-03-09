TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    wavloader.cpp \
    fft.cpp \
    gnuplot.cpp \
    wavcreator.cpp \
    rawaudiomono.cpp \
    rawaudiostereo.cpp \
    detail.cpp

HEADERS += \
    wavloader.hpp \
    fft.hpp \
    gnuplot.hpp \
    rawaudio.hpp \
    primesgenerator.hpp \
    rsa.hpp \
    wavcreator.hpp \
    rawaudiomono.hpp \
    rawaudiostereo.hpp \
    channel.hpp \
    detail.hpp


Boost_USE_STATIC_LIBS = ON
win32 {
    BOOSTHOME = C:/local/boost_1_69_0
    INCLUDEPATH += C:/Users/karol/Documents/EMediaProject/boost
    LIBS += -LC:/local/boost_1_69_0/lib64-msvc-14.0 \
            -lboost_serialization-vc140-mt-x64-1_69 \
            -lboost_filesystem-vc140-mt-x64-1_69 \
            -lboost_system-vc140-mt-x64-1_69
}
