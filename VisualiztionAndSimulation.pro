QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++11

TARGET      = 3D-programming

SOURCES += main.cpp \
    camera.cpp \
    grid.cpp \
    octahedronball.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    trianglesurface.cpp \
    vertex.cpp \
    visualobject.cpp

HEADERS += \
    camera.h \
    gltypes.h \
    grid.h \
    octahedronball.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    trianglesurface.h \
    vertex.h \
    visualobject.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    plainfragment.frag \
    plainvertex.vert
