#-------------------------------------------------
#
# Project created by QtCreator 2015-04-08T12:46:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceDetectingTest
TEMPLATE = app


SOURCES += main.cpp\
        facerecognizer.cpp

HEADERS  += facerecognizer.h

FORMS    += facerecognizer.ui


win32 {

    OPENCV_INCLUDE_PATH         = D:/Qt/opencv3/OpenCV_Git/include
    OPENCV_INCLUDE_MODULE_PATH  = D:/Qt/opencv3/OpenCV_Git/modules

    OPENCV_LIBS_PATH            = D:/Qt/opencv3/OpenCV_Git_Make/lib
}

unix {

    OPENCV_INCLUDE_PATH        = ~/Libs/OpenCV/opencv-3.0.0-alpha/include
    OPENCV_INCLUDE_MODULE_PATH = ~/Libs/OpenCV/opencv-3.0.0-alpha/build/modules

    OPENCV_LIBS_PATH           = ~/Libs/OpenCV/opencv-3.0.0-alpha/build/lib
}

INCLUDEPATH += $$OPENCV_INCLUDE_PATH

INCLUDEPATH += $$OPENCV_INCLUDE_MODULE_PATH/core/include

INCLUDEPATH += $$OPENCV_INCLUDE_MODULE_PATH/imgproc/include
INCLUDEPATH += $$OPENCV_INCLUDE_MODULE_PATH/imgcodecs/include

INCLUDEPATH += $$OPENCV_INCLUDE_MODULE_PATH/photo/include

INCLUDEPATH += $$OPENCV_INCLUDE_MODULE_PATH/video/include
INCLUDEPATH += $$OPENCV_INCLUDE_MODULE_PATH/videoio/include

INCLUDEPATH += $$OPENCV_INCLUDE_MODULE_PATH/objdetect/include

INCLUDEPATH += $$OPENCV_INCLUDE_MODULE_PATH/highgui/include

INCLUDEPATH += $$OPENCV_INCLUDE_MODULE_PATH/features2d/include
INCLUDEPATH += $$OPENCV_INCLUDE_MODULE_PATH/calib3d/include
INCLUDEPATH += $$OPENCV_INCLUDE_MODULE_PATH/ml/include

INCLUDEPATH += $$OPENCV_INCLUDE_MODULE_PATH/flann/include

win32   {

    CONFIG ( release ) {

        LIBS    += -L$$OPENCV_LIBS_PATH/Release
        LIBS    += -lopencv_core300         -lopencv_imgproc300
        LIBS    += -lopencv_objdetect300    -lopencv_videoio300 -lopencv_video300
    }

    CONFIG ( debug ) {

        LIBS    += -L$$OPENCV_LIBS_PATH/Debug
        LIBS    += -lopencv_core300d        -lopencv_imgproc300d
        LIBS    += -lopencv_objdetect300d   -lopencv_videoio300d -lopencv_video300d
    }
}

unix:CONFIG ( release, debug ) {

    LIBS    += -L$$OPENCV_LIBS_PATH
    LIBS    += -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect

}

