QT += core gui network widgets printsupport
qtHaveModule(uitools):!embedded: QT += uitools
else: DEFINES += QT_NO_UITOOLS

DEFINES += QT_NO_OPENSSL QT_NO_UNDOSTACK

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

VERSION = 1.0

DEFINES += \
    "DEVELOPER_NAME=\"\\\"Niu Tech\\\"\"" \
    "APP_NAME=\"\\\"Qt Ultralight Browser\\\"\"" \
    "APP_VERSION=\"\\\"$${VERSION}\\\"\""

SOURCES += \
    autosaver.cpp \
    bookmarks.cpp \
    browserapplication.cpp \
    browsermainwindow.cpp \
    chasewidget.cpp \
    cookiejar.cpp \
    downloadmanager.cpp \
    edittableview.cpp \
    edittreeview.cpp \
    history.cpp \
    modelmenu.cpp \
    networkaccessmanager.cpp \
    qultralightframe.cpp \
    qultralighthistory.cpp \
    qultralightpage.cpp \
    qultralightsettings.cpp \
    qultralightview.cpp \
    searchlineedit.cpp \
    settings.cpp \
    squeezelabel.cpp \
    tabwidget.cpp \
    toolbarsearch.cpp \
    urllineedit.cpp \
    webview.cpp \
    xbel.cpp \
    main.cpp

HEADERS += \
    autosaver.h \
    bookmarks.h \
    browserapplication.h \
    browsermainwindow.h \
    chasewidget.h \
    cookiejar.h \
    downloadmanager.h \
    edittableview.h \
    edittreeview.h \
    history.h \
    modelmenu.h \
    networkaccessmanager.h \
    qultralightframe.h \
    qultralighthistory.h \
    qultralightpage.h \
    qultralightsettings.h \
    qultralightview.h \
    searchlineedit.h \
    settings.h \
    squeezelabel.h \
    tabwidget.h \
    toolbarsearch.h \
    urllineedit.h \
    webview.h \
    xbel.h

FORMS += \
    addbookmarkdialog.ui \
    bookmarks.ui \
    cookies.ui \
    cookiesexceptions.ui \
    downloaditem.ui \
    downloads.ui \
    history.ui \
    passworddialog.ui \
    proxy.ui \
    settings.ui

RESOURCES += data/data.qrc htmls/htmls.qrc

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

win32 {
   RC_FILE = browser.rc
}

mac {
    ICON = browser.icns
    QMAKE_INFO_PLIST = Info_mac.plist
    TARGET = Browser
}

EXAMPLE_FILES = Info_mac.plist browser.icns browser.ico browser.rc

# Default rules for deployment.
unix:!android: target.path = /opt/$${TARGET}/bin
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
