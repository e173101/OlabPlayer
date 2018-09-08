#-------------------------------------------------
#
# Project created by QtCreator 2018-05-04T09:10:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =  OlabPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    cvmatandqimage.cpp \
    matproducer.cpp \
    matcooker.cpp

HEADERS += \
        mainwindow.h \
    cvmatandqimage.h \
    matproducer.h \
    matcooker.h

FORMS += \
        mainwindow.ui


#OpenCV
#MINGW
#INCLUDEPATH += C:/OpenCV/2.4.13/include
#LIBS += C:/OpenCV/2.4.13/lib/lib*.a
#VS


INCLUDEPATH += \
        E:/OpenCV/vc/342/build/include

CONFIG(debug, debug|release): {
LIBS += \
        E:/OpenCV/vc/342/build/x64/vc14/lib/opencv_world342d.lib
}
else:{
LIBS += \
        E:/OpenCV/vc/342/build/x64/vc14/lib/opencv_world342.lib
}
