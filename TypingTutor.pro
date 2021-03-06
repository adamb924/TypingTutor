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
    courseeditorwindow.cpp \
    coursemodel.cpp \
    coursetreeview.cpp \
    keyboardeditorwindow.cpp \
    keyboardmodel.cpp \
    keyboardlistview.cpp \
    edittextstylesdialog.cpp

HEADERS  += mainwindow.h \
    targethighlighter.h \
    keyboard.h \
    letter.h \
    course.h \
    section.h \
    prompt.h \
    promptform.h \
    shortcuttextedit.h \
    courseeditorwindow.h \
    coursemodel.h \
    coursetreeview.h \
    keyboardeditorwindow.h \
    keyboardmodel.h \
    keyboardlistview.h \
    edittextstylesdialog.h

FORMS    += mainwindow.ui \
    promptform.ui \
    courseeditorwindow.ui \
    keyboardeditorwindow.ui \
    edittextstylesdialog.ui
