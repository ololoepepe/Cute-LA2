TEMPLATE = lib

TARGET = calc

VERSION = 0.1.0
VER_MAJ = 0
VER_MIN = 1
VER_PAT = 0

BEQT = core widgets

include(../plugin.pri)

HEADERS += \
    calc.h \
    calcwidget.h \
    characterclass.h \
    tattoo.h \
    specialability.h \
    weapon.h \
    grade.h \
    dualswordspecialability.h \
    heroweaponspecialability.h

SOURCES += \
    calc.cpp \
    calcwidget.cpp \
    characterclass.cpp \
    tattoo.cpp \
    specialability.cpp \
    weapon.cpp \
    grade.cpp \
    dualswordspecialability.cpp \
    heroweaponspecialability.cpp
