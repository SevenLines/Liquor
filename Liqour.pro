#-------------------------------------------------
#
# Project created by QtCreator 2013-10-05T01:41:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Liqour
TEMPLATE = app
CONFIG -= console
TRANSLATIONS += main.ts

SOURCES += main.cpp\
        mainwindow.cpp \
        qlabelimage.cpp \
    imageprocessing.cpp \
    mgraphicsview.cpp \
    emisionanalyzer.cpp \
    horizontalsliderex.cpp \
    imagestack.cpp \
    opencvutils.cpp \
    keypoints.cpp

HEADERS  += mainwindow.h \
            qlabelimage.h \
    imageprocessing.h \
    histogram1d.h \
    mgraphicsview.h \
    emisionanalyzer.h \
    horizontalsliderex.h \
    imagestack.h \
    opencvutils.h \
    keypoints.h \
    opencvutils_tempates.h

FORMS    += mainwindow.ui \
    horizontalsliderex.ui

FORMS    += mainwindow.ui \
    horizontalsliderex.ui

win32 {
   INCLUDEPATH += d:/_OpenCV/include/opencv
   INCLUDEPATH += d:/_OpenCV/include/
   INCLUDEPATH += d:/Boost/boost_1_54_0/ 
   LIBS += -Ld:/_OpenCV/build/x86/mingw44/lib/
   LIBS +=  libopencv_core246.dll \ 
	    libopencv_highgui246.dll \
	    libopencv_imgproc246.dll 
}

unix {
    INCLUDEPATH += /usr/locale/lib
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    LIBS += -lopencv_core -lopencv_highgui -lopencv_superres
}

message($$VERSION)

OTHER_FILES += \
    VERSION.txt \
    BUILD.txt
