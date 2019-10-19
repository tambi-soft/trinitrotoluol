######################################################################
# Automatically generated by qmake (3.1) Thu Nov 30 02:14:53 2017
######################################################################

TEMPLATE = app
TARGET = trinitrotoluol

linux-g++ | linux-g++-64 | linux-g++-32 {
    LIBS += -lcurl
#    # on linux we have to use gstreamer for audio recording directly
#    # because QAudioRecorder is broken here for over a year now
#    INCLUDEPATH += "/usr/include/gstreamer-1.0/"
#    INCLUDEPATH += "/usr/include/glib-2.0/"
#    INCLUDEPATH += "/usr/lib/glib-2.0/include/"
#    
#    LIBS += -lgstreamer-1.0
#    LIBS += -lgobject-2.0
#    #LIBS += -lglib-2.0
#
#    HEADERS += src/gst_audio_recorder.h
#    SOURCES += src/gst_audio_recorder.cpp
}
win32 {
    RC_ICONS += ./assets/logo.ico
    
    CONFIG += static

    INCLUDEPATH += "assets/windows/"
    INCLUDEPATH += "assets/windows/"
    
    #LIBS += -lqminimal
    #LIBS += -lqwindows
}

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += qt debug #release
#CONFIG += qt release
QT += widgets \
    sql \
    webenginewidgets

# Input
HEADERS += \
    src/currencies.h \
    src/expenses_list.h \
    src/journey_edit.h \
    src/journey_list.h \
    src/journey_tickets.h \
    src/journey_visits.h \
    src/mail_curl.h \
    src/mail_edit.h \
    src/mail_list.h \
    src/mail_preview.h \
    src/mail_send.h \
    src/main_window.h \
    src/config.h \
    src/db_adapter.h \
    src/menubar.h \
    src/people_list.h \
    src/people_selector.h \
    src/person_edit.h \
    src/settings.h \
    src/simplecrypt.h \
    src/sqlite_editor_widget.h \
    src/sqlite_sync.h \
    src/stats.h \
    src/tnt_adapter.h
    

SOURCES += \
    src/currencies.cpp \
    src/expenses_list.cpp \
    src/journey_edit.cpp \
    src/journey_list.cpp \
    src/journey_tickets.cpp \
    src/journey_visits.cpp \
    src/mail_curl.cpp \
    src/mail_edit.cpp \
    src/mail_list.cpp \
    src/mail_preview.cpp \
    src/mail_send.cpp \
    src/menubar.cpp \
    src/people_list.cpp \
    src/people_selector.cpp \
    src/person_edit.cpp \
    src/settings.cpp \
    src/simplecrypt.cpp \
    src/sqlite_editor_widget.cpp \
    src/sqlite_sync.cpp \
    src/stats.cpp \
    src/tnt_adapter.cpp \
    src/trinitrotoluol.cpp \
    src/main_window.cpp \
    src/config.cpp \
    src/db_adapter.cpp

RESOURCES += \
    resources.qrc

release:DESTDIR = release
release:OBJECTS_DIR = release/obj
release:MOC_DIR = release/moc
release:RCC_DIR = release/rcc
release:UI_DIR = release/ui

debug:DESTDIR = debug
debug:OBJECTS_DIR = debug/obj
debug:MOC_DIR = debug/moc
debug:RCC_DIR = debug/rcc
debug:UI_DIR = debug/ui
