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

INCLUDEPATH += Core Utils Widgets

SOURCES  += main.cpp\
    mainwindow.cpp \
    Core/imagestack.cpp \
    Core/keypoints.cpp \
    Core/emisionanalyzer.cpp \
    Utils/imageprocessing.cpp \
    Utils/opencvutils.cpp \
    Widgets/horizontalsliderex.cpp \
    Widgets/qlabelimage.cpp \
    Widgets/mgraphicsview.cpp \
    Widgets/mgraphicsviewea.cpp \
    Widgets/qgraphicsparticleitem.cpp \
    Widgets/sequenceanalyzewidget.cpp

HEADERS  += mainwindow.h \
    Core/imagestack.h \
    Core/keypoints.h \
    Core/opencvutils_tempates.h \
    Core/emisionanalyzer.h \
    Utils/imageprocessing.h \
    Utils/opencvutils.h \
    Widgets/qlabelimage.h \
    Widgets/mgraphicsview.h \
    Widgets/horizontalsliderex.h \
    Widgets/mgraphicsviewea.h \
    Widgets/qgraphicsparticleitem.h \
    Widgets/sequenceanalyzewidget.h

FORMS    += mainwindow.ui \
    Widgets/horizontalsliderex.ui \
    Widgets/sequenceanalyzewidget.ui

FORMS    += mainwindow.ui \
    Widgets/horizontalsliderex.ui



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
