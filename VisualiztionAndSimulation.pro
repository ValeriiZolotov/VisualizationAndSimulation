QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++11

TARGET      = 3D-programming

SOURCES += main.cpp \
    grid.cpp \
    octahedronball.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    vertex.cpp \
    visualobject.cpp

HEADERS += \
    gltypes.h \
    grid.h \
    octahedronball.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    vertex.h \
    visualobject.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    plainfragment.frag \
    plainvertex.vert
