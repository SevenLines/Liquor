#-------------------------------------------------
#
# Project created by QtCreator 2013-10-05T01:41:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Liqour
TEMPLATE = app
CONFIG += console


SOURCES += main.cpp\
        mainwindow.cpp \
        qlabelimage.cpp \
    imageprocessing.cpp \
    mgraphicsview.cpp \
    emisionanalyzer.cpp \
    horizontalsliderex.cpp \
    imagestack.cpp \
    opencvutils.cpp

HEADERS  += mainwindow.h \
            qlabelimage.h \
    imageprocessing.h \
    histogram1d.h \
    mgraphicsview.h \
    emisionanalyzer.h \
    horizontalsliderex.h \
    imagestack.h \
    opencvutils.h

FORMS    += mainwindow.ui \
    horizontalsliderex.ui

unix {
    INCLUDEPATH += /usr/locale/lib
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

LIBS += -lopencv_core -lopencv_highgui -lopencv_superres
