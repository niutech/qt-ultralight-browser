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

unix|win32: LIBS += -L$$PWD/../ultralight/lib/ -lUltralight -lUltralightCore -lAppCore -lWebCore

INCLUDEPATH += $$PWD/../ultralight/include
DEPENDPATH += $$PWD/../ultralight/include

DISTFILES += \
    assets/ultralight.ico

RC_ICONS = assets/ultralight.ico

COPIES += ultralightBin ultralightRes

ultralightBin.files = $$files($$PWD/../ultralight/bin/*.dll)
debug: ultralightBin.path = $$OUT_PWD/debug
else: ultralightBin.path = $$OUT_PWD/release

ultralightRes.files = $$files($$PWD/../ultralight/bin/resources/*)
debug: ultralightRes.path = $$OUT_PWD/debug/resources
else: ultralightRes.path = $$OUT_PWD/release/resources
