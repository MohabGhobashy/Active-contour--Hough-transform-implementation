QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#INCLUDEPATH += C:\opencv\build\include
#LIBS += C:\opencv\release\bin\libopencv_calib3d470.dll
#LIBS += C:\opencv\release\bin\libopencv_core470.dll
#LIBS += C:\opencv\release\bin\libopencv_highgui470.dll
#LIBS += C:\opencv\release\bin\libopencv_imgcodecs470.dll
#LIBS += C:\opencv\release\bin\libopencv_imgproc470.dll

INCLUDEPATH += C:\openCV\opencv\build\include
LIBS += C:\openCV\opencv\release\bin\libopencv_calib3d470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_core470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_highgui470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_imgcodecs470.dll
LIBS += C:\openCV\opencv\release\bin\libopencv_imgproc470.dll


SOURCES += \
    image.cpp \
    main.cpp \
    mainwindow.cpp \
    shapesdetection.cpp\
    image2.cpp\
    active_contour.cpp\
    utils.cpp

HEADERS += \
    image.h \
    mainwindow.h \
    shapesdetection.h\
    image2.hpp\
    active_contour.hpp\
    utils.hpp\
    cvector.hpp

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    my_resources.qrc
RC_ICONS= icon.ico
