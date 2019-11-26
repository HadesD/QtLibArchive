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

CONFIG(debug, debug|release) {
  CONFIGURATION = Debug
} else {
  CONFIGURATION = Release
}

# Libraries and Dependencies settings

#LIBS += -L$$PWD/../bin/lib

DLLDESTDIR = "$$PWD/../../lib/$$CONFIGURATION/"

QMAKE_POST_LINK += "copy \"$$OUT_PWD/$(DESTDIR_TARGET)\" \"$$PWD/../../lib/$$CONFIGURATION/\""

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
