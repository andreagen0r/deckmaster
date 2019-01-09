TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
    board.c \
    player.c \
    card.c \
    pack.c \
    game.c \
    point.c

HEADERS += \
    board.h \
    player.h \
    card.h \
    pack.h \
    game.h \
    point.h
