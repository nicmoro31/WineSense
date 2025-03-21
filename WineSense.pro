TARGET = WineSense
INCLUDEPATH += .
QT       += core gui charts

QT_QPA_PLATFORM=wayland

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Define the source path
IMG_SOURCE_PATH = ./img

# Define a custom variable to distinguish between debug and release builds
CONFIG(debug, debug|release) {
    BUILD_TYPE = Debug
} else {
    BUILD_TYPE = Release
}

# Define the destination path based on the build type
IMG_DEST_PATH = ./build-WineSense-Desktop-$${BUILD_TYPE}

create_dest_dir.commands = mkdir -p $$shell_quote($$IMG_DEST_PATH)/img
copy_images.commands = cp -r $$shell_quote($$IMG_SOURCE_PATH) $$shell_quote($$IMG_DEST_PATH)



# Make the copy command run before the build
PRE_TARGETDEPS += create_dest_dir copy_images

# Add the command to the QMAKE_EXTRA_TARGETS so it gets executed
QMAKE_EXTRA_TARGETS += create_dest_dir copy_images

SOURCES += \
    addsensorwindow.cpp \
    autoclave.cpp \
    barchart.cpp \
    database.cpp \
    datagenerator.cpp \
    datawindow.cpp \
    generalchart.cpp \
    chartwindow.cpp \
    linechart.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    nuovaautoclavewindow.cpp \
    sensore.cpp \
    sensorelivello.cpp \
    sensoreph.cpp \
    sensorepressione.cpp \
    sensoretemperatura.cpp \
    sensoreumidita.cpp

HEADERS += \
    addsensorwindow.h \
    autoclave.h \
    barchart.h \
    database.h \
    datagenerator.h \
    datawindow.h \
    exceptions.h \
    generalchart.h \
    chartwindow.h \
    linechart.h \
    mainwindow.h \
    model.h \
    nuovaautoclavewindow.h \
    sensore.h \
    sensorelivello.h \
    sensoreph.h \
    sensorepressione.h \
    sensoretemperatura.h \
    sensoreumidita.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
