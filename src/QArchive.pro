#-------------------------------------------------
#
# Project created by QtCreator 2019-03-27T18:54:22
#
#-------------------------------------------------

QT       -= gui

TARGET = QArchive
TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= import_plugins

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Add user custom dir
INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include.usr
LIBS += -L$$[QT_INSTALL_PREFIX]/lib.usr

CONFIG(release, debug|release):CONFIG -= console
CONFIG(debug, debug|release):TARGET = $${TARGET}d

include(QArchive.pri)

INCLUDEPATH += $$PWD/../deps/libarchive
DEPENDPATH += $$PWD/../deps/libarchive

# Source code
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
  QArchive.pri
