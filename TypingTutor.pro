#-------------------------------------------------
#
# Project created by QtCreator 2014-10-27T11:10:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TypingTutor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    targethighlighter.cpp \
    keyboard.cpp \
    letter.cpp \
    course.cpp \
    section.cpp \
    prompt.cpp \
    promptform.cpp \
    shortcuttextedit.cpp \
    courseeditorwindow.cpp

HEADERS  += mainwindow.h \
    targethighlighter.h \
    keyboard.h \
    letter.h \
    course.h \
    section.h \
    prompt.h \
    promptform.h \
    shortcuttextedit.h \
    courseeditorwindow.h

FORMS    += mainwindow.ui \
    promptform.ui \
    courseeditorwindow.ui
