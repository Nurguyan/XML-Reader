#-------------------------------------------------
#
# Project created by QtCreator 2020-07-21T13:15:12
#
#-------------------------------------------------

QT       += core gui sql

RC_ICONS = resources/icons/icon.ico

CONFIG   += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XMLReader
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


SOURCES += \
        comboboxdelegate.cpp \
        databaseworker.cpp \
        importdialog.cpp \
        listmodel.cpp \
        main.cpp \
        mainwindow.cpp \
        tablemodel.cpp \
        texteditor.cpp \
        xmlfilecontroller.cpp \
        xmlfileworker.cpp

HEADERS += \
        comboboxdelegate.h \
        databaseworker.h \
        importdialog.h \
        listmodel.h \
        mainwindow.h \
        tablemodel.h \
        texteditor.h \
        xmlfilecontroller.h \
        xmlfileworker.h

FORMS += \
        dialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
