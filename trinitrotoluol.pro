######################################################################
# Automatically generated by qmake (3.1) Thu Nov 30 02:14:53 2017
######################################################################

TEMPLATE = app
TARGET = trinitrotoluol

linux-g++ | linux-g++-64 | linux-g++-32 {
    LIBS += -lcurl

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
    webenginewidgets\
    charts

# Input
HEADERS += \
    src/combo_currencies.h \
    src/currencies.h \
    src/donations.h \
    src/donations_import.h \
    src/donations_import_matrix.h \
    src/donations_import_progress.h \
    src/donations_list.h \
    src/expenses_list.h \
    src/export.h \
    src/firstrun.h \
    src/groups_edit.h \
    src/help_message.h \
    src/journey_edit.h \
    src/journey_list.h \
    src/journey_tickets.h \
    src/journey_visits.h \
    src/journey_visits_edit.h \
    src/lib/db_access.h \
    src/lib/db_merge.h \
    src/lib/grid_widget.h \
    src/lib/groups_and_relations_edit.h \
    src/lib/growing_text_edit.h \
    src/mail_edit.h \
    src/mail_list.h \
    src/mail_message.h \
    src/mail_preview.h \
    src/mail_send.h \
    src/main_window.h \
    src/config.h \
    src/db_adapter.h \
    src/menubar.h \
    src/merge_database_widget.h \
    src/parse_csv.h \
    src/people_list.h \
    src/people_selector.h \
    src/person_edit.h \
    src/person_groups.h \
    src/person_relations.h \
    src/person_visits.h \
    src/relations_edit.h \
    src/settings.h \
    src/simplecrypt.h \
    src/sqlite_editor_widget.h \
    src/sqlite_sync.h \
    src/stats.h \
    src/tnt_adapter.h \
    src/help_about_widget.h
    
SOURCES += \
    src/combo_currencies.cpp \
    src/currencies.cpp \
    src/donations.cpp \
    src/donations_import.cpp \
    src/donations_import_matrix.cpp \
    src/donations_import_progress.cpp \
    src/donations_list.cpp \
    src/expenses_list.cpp \
    src/export.cpp \
    src/firstrun.cpp \
    src/groups_edit.cpp \
    src/help_message.cpp \
    src/journey_edit.cpp \
    src/journey_list.cpp \
    src/journey_tickets.cpp \
    src/journey_visits.cpp \
    src/journey_visits_edit.cpp \
    src/lib/db_access.cpp \
    src/lib/db_merge.cpp \
    src/lib/grid_widget.cpp \
    src/lib/groups_and_relations_edit.cpp \
    src/lib/growing_text_edit.cpp \
    src/mail_edit.cpp \
    src/mail_list.cpp \
    src/mail_message.cpp \
    src/mail_preview.cpp \
    src/mail_send.cpp \
    src/menubar.cpp \
    src/merge_database_widget.cpp \
    src/parse_csv.cpp \
    src/people_list.cpp \
    src/people_selector.cpp \
    src/person_edit.cpp \
    src/person_groups.cpp \
    src/person_relations.cpp \
    src/person_visits.cpp \
    src/relations_edit.cpp \
    src/settings.cpp \
    src/simplecrypt.cpp \
    src/sqlite_editor_widget.cpp \
    src/sqlite_sync.cpp \
    src/stats.cpp \
    src/tnt_adapter.cpp \
    src/trinitrotoluol.cpp \
    src/main_window.cpp \
    src/config.cpp \
    src/db_adapter.cpp \
    src/help_about_widget.cpp

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
