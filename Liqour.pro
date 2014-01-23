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

TRANSLATIONS += liqour_ru.ts

INCLUDEPATH += Core Utils Widgets Settings

SOURCES  += main.cpp\
    mainwindow.cpp \
    Core/imagestack.cpp \
    Core/keypoints.cpp \
    Utils/imageprocessing.cpp \
    Utils/opencvutils.cpp \
    Utils/qcustomplot.cpp \
    Widgets/horizontalsliderex.cpp \
    Widgets/qlabelimage.cpp \
    Widgets/mgraphicsview.cpp \
    Widgets/mgraphicsviewea.cpp \
    Widgets/qgraphicsparticleitem.cpp \
    Widgets/sequenceanalyzewidget.cpp \
    Core/multikeypoints.cpp \
    Widgets/qimagebrowser.cpp \
    Core/multikeypointsmodel.cpp \
    Settings/formsettings.cpp \
    Settings/formsettingsgeneral.cpp \
    Widgets/tabdocumentswidget.cpp \
    Widgets/progressdialog.cpp \
    Core/pointvalue.cpp \
    Core/qthreadex.cpp \
    Widgets/aboutdialog.cpp \
    Core/emisionanalyzer.cpp \
    Core/particlesseeker.cpp \
    Core/areasseeker.cpp \
    Core/emisionanalyzerthread.cpp \
    Widgets/lightcorrector.cpp \
    Widgets/lightcorrectorwidget.cpp \
    Widgets/qgraphicsitemholder.cpp \
    Widgets/qgraphicsitemframe.cpp \
    Widgets/qgraphicsitemlightcorrector.cpp

HEADERS  += mainwindow.h \
    Core/imagestack.h \
    Core/keypoints.h \
    Core/opencvutils_tempates.h \
    Utils/imageprocessing.h \
    Utils/opencvutils.h \
    Utils/qcustomplot.h \
    Widgets/qlabelimage.h \
    Widgets/mgraphicsview.h \
    Widgets/horizontalsliderex.h \
    Widgets/mgraphicsviewea.h \
    Widgets/qgraphicsparticleitem.h \
    Widgets/sequenceanalyzewidget.h \
    Core/multikeypoints.h \
    Widgets/qimagebrowser.h \
    Core/multikeypointsmodel.h \
    Settings/formsettings.h \
    Settings/formsettingsgeneral.h \
    ApplicationInfo.h \
    MainInclude.h \
    Widgets/tabdocumentswidget.h \
    Widgets/progressdialog.h \
    Core/pointvalue.h \
    Core/qthreadex.h \
    Widgets/aboutdialog.h \
    Core/emisionanalyzer.h \
    Core/particlesseeker.h \
    Core/areasseeker.h \
    Core/emisionanalyzerthread.h \
    Utils/opencvutils_tempates.h \
    Widgets/lightcorrector.h \
    Widgets/lightcorrectorwidget.h \
    Widgets/qgraphicsitemholder.h \
    Widgets/qgraphicsitemframe.h \
    Widgets/qgraphicsitemlightcorrector.h

FORMS    += mainwindow.ui \
    Widgets/horizontalsliderex.ui \
    Widgets/sequenceanalyzewidget.ui \
    Widgets/qimagebrowser.ui \
    Settings/formsettings.ui \
    Settings/formsettingsgeneral.ui \
    Widgets/progressdialog.ui \
    Widgets/aboutdialog.ui \
    Widgets/lightcorrectorwidget.ui



win32 {
    INCLUDEPATH += d:/_OpenCV/include/opencv
    INCLUDEPATH += d:/_OpenCV/include/
    INCLUDEPATH += d:/_DISTR/_QT/
    INCLUDEPATH += d:/Boost/boost_1_54_0/ 
    
    # widows resource
    RC_FILE = Assets/windows_resource.rc

    OPENCV_PATH = d:/_OpenCV/build/x86/mingw4.4/
    OPENCV_PATH ~= s,/,\\,g
    OPENCV_LIBS = libopencv_core246.dll \ 
	    libopencv_highgui246.dll \
	    libopencv_imgproc246.dll

    LIBS += -L$${OPENCV_PATH}lib
    LIBS += $$OPENCV_LIBS

    SDIR = $${PWD}/
    SDIR ~= s,/,\\,g
    DDIR = $${OUT_PWD}
    DDIR ~= s,/,\\,g

    #copy OPENCV libs
    for(FILE, OPENCV_LIBS) {
	QMAKE_POST_LINK += $${QMAKE_COPY} $${OPENCV_PATH}bin\\$$FILE $${DDIR}$$escape_expand(\\n\\t)
    }

    OTHER_FILES += \
	Assets/windows_resource.rc \
	Assets/icon.ico
}

unix {
    INCLUDEPATH += /usr/locale/lib
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    LIBS += -lopencv_core -lopencv_highgui -lopencv_superres
    SDIR = $${PWD}/
    DDIR = $${OUT_PWD}
}


OTHER_FILES += \
    VERSION.txt \
    BUILD.txt \
    Assets/logo_3.png \
    Assets/loading.gif \
    Assets/loading2.gif \
    Assets/qt_ru.qm \
    Assets/icon2.ico \
    Assets/icon_16x16.ico \
    Assets/icon_32x32.ico \
    Assets/icon_64x64.ico

# copy linquist files
for(FILE, TRANSLATIONS) {
    QMAKE_POST_LINK += $${QMAKE_COPY} $${SDIR}$$replace(FILE,ts,qm) $${DDIR}$$escape_expand(\\n\\t)
}


RESOURCES += \
    Assets/main.qrc



