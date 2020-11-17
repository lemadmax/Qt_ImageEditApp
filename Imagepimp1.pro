#-------------------------------------------------
#
# Project created by QtCreator 2018-10-23T17:22:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Imagepimp1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += E:/tools/opencv/opencv/build/include

CONFIG(debug, debug|release): {
LIBS += -LE:/tools/opencv/opencv/build/x64/vc15/lib \
-lopencv_world430d
} else:CONFIG(release, debug|release): {
LIBS += -LE:/tools/opencv/opencv/build/x64/vc15/lib \
-lopencv_world430
}


CONFIG += c++11

SOURCES += \
    acokmeans.cpp \
    gakmeans.cpp \
        main.cpp \
        mainwindow.cpp \
    kmeans.cpp \
    mypba.cpp \
    pbakmeans.cpp \
    psokmeans.cpp \
    sakmeans.cpp \
    suedpbakmeans.cpp \
    tmeans.cpp

HEADERS += \
    acokmeans.h \
    gakmeans.h \
        mainwindow.h \
    myba.h \
    kmeans.h \
    mypba.h \
    pbakmeans.h \
    psokmeans.h \
    sakmeans.h \
    suedpbakmeans.h \
    tmeans.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../gdal/lib/ -lgdal_i
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../gdal/lib/ -lgdal_i
#else:unix: LIBS += -L$$PWD/../../../gdal/lib/ -lgdal_i

#INCLUDEPATH += $$PWD/../../../gdal/include
#DEPENDPATH += $$PWD/../../../gdal/include


win32:CONFIG(release, debug|release): LIBS += -LD:/APP/matlab/extern/lib/win64/microsoft/ -llibeng
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/APP/matlab/extern/lib/win64/microsoft/ -llibengd
else:unix: LIBS += -LD:/APP/matlab/extern/lib/win64/microsoft/ -llibeng

INCLUDEPATH += D:/APP/matlab/extern/include
DEPENDPATH += D:/APP/matlab/extern/include

win32:CONFIG(release, debug|release): LIBS += -LD:/APP/matlab/extern/lib/win64/microsoft/ -llibmat
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/APP/matlab/extern/lib/win64/microsoft/ -llibmatd
else:unix: LIBS += -LD:/APP/matlab/extern/lib/win64/microsoft/ -llibmat


win32:CONFIG(release, debug|release): LIBS += -LD:/APP/matlab/extern/lib/win64/microsoft/ -llibmex
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/APP/matlab/extern/lib/win64/microsoft/ -llibmexd
else:unix: LIBS += -LD:/APP/matlab/extern/lib/win64/microsoft/ -llibmex

win32:CONFIG(release, debug|release): LIBS += -LD:/APP/matlab/extern/lib/win64/microsoft/ -llibmx
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/APP/matlab/extern/lib/win64/microsoft/ -llibmxd
else:unix: LIBS += -LD:/APP/matlab/extern/lib/win64/microsoft/ -llibmx

