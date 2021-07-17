QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 file_copies

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qultralight.cpp

HEADERS += \
    mainwindow.h \
    qultralight.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ULTRALIGHT_PATH = ../ultralight

unix|win32: LIBS += -L$$PWD/$$ULTRALIGHT_PATH/lib/ -lUltralight -lUltralightCore -lAppCore -lWebCore

INCLUDEPATH += $$PWD/$$ULTRALIGHT_PATH/include
DEPENDPATH += $$PWD/$$ULTRALIGHT_PATH/include

DISTFILES += assets/ultralight.ico

RC_ICONS = assets/ultralight.ico

COPIES += ultralightBin ultralightRes

ultralightBin.files = $$files($$PWD/$$ULTRALIGHT_PATH/bin/*.dll)
debug: ultralightBin.path = $$OUT_PWD/debug
release: ultralightBin.path = $$OUT_PWD/release

ultralightRes.files = $$files($$PWD/$$ULTRALIGHT_PATH/bin/resources/*)
debug: ultralightRes.path = $$OUT_PWD/debug/assets/resources
release: ultralightRes.path = $$OUT_PWD/release/assets/resources
