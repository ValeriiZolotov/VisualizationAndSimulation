QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++11

TARGET      = 3D-programming

SOURCES += main.cpp \
    boundingbox.cpp \
    bsplinecurve.cpp \
    camera.cpp \
    delaunator.cpp \
    fsm.cpp \
    grid.cpp \
    light.cpp \
    mesh.cpp \
    npc.cpp \
    objloader.cpp \
    octahedronball.cpp \
    player.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    triangle.cpp \
    trianglesurface.cpp \
    vertex.cpp \
    visualobject.cpp

HEADERS += \
    boundingbox.h \
    bsplinecurve.h \
    camera.h \
    delaunator.h \
    fsm.h \
    gltypes.h \
    grid.h \
    light.h \
    mesh.h \
    npc.h \
    objloader.h \
    octahedronball.h \
    player.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    triangle.h \
    trianglesurface.h \
    vertex.h \
    visualobject.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    phnogcolorfragment.frag \
    phongcolorvertex.vert \
    plainfragment.frag \
    plainvertex.vert
