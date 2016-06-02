#-------------------------------------------------
#
# Project created by QtCreator 2016-04-10T17:01:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Feedback
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_shape -lopencv_features2d  -lopencv_objdetect -lopencv_calib3d -lopencv_contrib -lopencv_legacy -lopencv_flann -lQt5PrintSupport -lQt5Widgets -lQt5Gui -lQt5Core -lGL

isEmpty(VIDEO){
  VIDEO = DEFAULT
}
isEmpty(SONAR){
  SONAR = DEFAULT
}
isEmpty(YAW){
  YAW = DEFAULT
}

QMAKE_CLEAN += ./Feedback
QMAKE_CXXFLAGS += -D$${SONAR} -D$${VIDEO} -D$${YAW} -std=c++11

SOURCES +=\
        mainwindow.cpp \
    feedback.cpp \
    opticalFlow.cpp \
    qcustomplot.cpp \
    sensors.cpp \
    serial_port.cpp \
    eulerplot.cpp \
    anglecorrection.cpp \
    opticalflowfunctions.cpp

HEADERS  += mainwindow.h \
    globals.h \
    opticalFlow.h \
    qcustomplot.h \
    sensors.h \
    serial_port.h \
    eulerplot.h \
    anglecorrection.h \
    opticalflowfunctions.h

FORMS    += mainwindow.ui \
    eulerplot.ui \
    anglecorrection.ui
