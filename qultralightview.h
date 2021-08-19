/*
    Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
    Copyright (C) 2008 Holger Hans Peter Freyther
    Copyright (C) 2009 Girish Ramakrishnan <girish@forwardbias.in>
    Copyright (C) 2021 Jerzy Głowacki
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef QULTRALIGHTVIEW_H
#define QULTRALIGHTVIEW_H

#include <QtCore/qurl.h>
#include <QtGui/qicon.h>
#include <QtGui/qpainter.h>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtWidgets/qwidget.h>
#include <QGridLayout>
#include <QKeyEvent>
#include <QTimer>
#include <AppCore/AppCore.h>
#include "qultralightpage.h"
//#include "qultralightframe.h"
#include "qultralighthistory.h"
#include "qultralightsettings.h"

QT_BEGIN_NAMESPACE
class QNetworkRequest;
class QPrinter;
QT_END_NAMESPACE

class QUltralightPage;
class QUltralightFrame;
class QWebNetworkRequest;

class QUltralightView : public QWidget,
        public ultralight::WindowListener,
        public ultralight::LoadListener,
        public ultralight::ViewListener {
    Q_OBJECT
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QIcon icon READ icon)
    Q_PROPERTY(QString selectedText READ selectedText)
    Q_PROPERTY(QString selectedHtml READ selectedHtml)
    Q_PROPERTY(bool hasSelection READ hasSelection)
    Q_PROPERTY(bool modified READ isModified)
    Q_PROPERTY(qreal textSizeMultiplier READ textSizeMultiplier WRITE setTextSizeMultiplier DESIGNABLE false)
    Q_PROPERTY(qreal zoomFactor READ zoomFactor WRITE setZoomFactor)
    Q_PROPERTY(QPainter::RenderHints renderHints READ renderHints WRITE setRenderHints)
    Q_FLAGS(QPainter::RenderHints)

public:
    explicit QUltralightView(QWidget* parent = Q_NULLPTR);
    ~QUltralightView();

    QUltralightPage* page() const;
    void setPage(QUltralightPage* page);

    void load(const QUrl& url);
    void load(const QNetworkRequest& request, QNetworkAccessManager::Operation operation = QNetworkAccessManager::GetOperation, const QByteArray &body = QByteArray());
    void setHtml(const QString& html, const QUrl& baseUrl = QUrl());
    void setContent(const QByteArray& data, const QString& mimeType = QString(), const QUrl& baseUrl = QUrl());

    QUltralightHistory* history() const;
    QUltralightSettings* settings() const;

    QString title() const;
    void setUrl(const QUrl &url);
    QUrl url() const;
    QIcon icon() const;

    bool hasSelection() const;
    QString selectedText() const;
    QString selectedHtml() const;

#ifndef QT_NO_ACTION
    QAction* pageAction(QUltralightPage::WebAction action) const;
#endif
    void triggerPageAction(QUltralightPage::WebAction action, bool checked = false);

    bool isModified() const;

    /*
    Qt::TextInteractionFlags textInteractionFlags() const;
    void setTextInteractionFlags(Qt::TextInteractionFlags flags);
    void setTextInteractionFlag(Qt::TextInteractionFlag flag);
    */

    QVariant inputMethodQuery(Qt::InputMethodQuery property) const Q_DECL_OVERRIDE;

    QSize sizeHint() const Q_DECL_OVERRIDE;

    qreal zoomFactor() const;
    void setZoomFactor(qreal factor);

    void setTextSizeMultiplier(qreal factor);
    qreal textSizeMultiplier() const;

    QPainter::RenderHints renderHints() const;
    void setRenderHints(QPainter::RenderHints hints);
    void setRenderHint(QPainter::RenderHint hint, bool enabled = true);

    bool findText(const QString& subString, QUltralightPage::FindFlags options = QUltralightPage::FindFlags());

    bool event(QEvent*) Q_DECL_OVERRIDE;

public Q_SLOTS:
    void stop();
    void back();
    void forward();
    void reload();

    void print(QPrinter*) const;

    //Ultralight
    void sendKey(QKeyEvent *event);

Q_SIGNALS:
    void loadStarted();
    void loadProgress(int progress);
    void loadFinished(bool);
    void titleChanged(const QString& title);
    void statusBarMessage(const QString& text);
    void linkClicked(const QUrl&);
    void selectionChanged();
    void iconChanged();
    void urlChanged(const QUrl&);

protected:
    void resizeEvent(QResizeEvent*) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;

    virtual QUltralightView *createWindow(QUltralightPage::WebWindowType type);

    void changeEvent(QEvent*) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent*) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent*) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent*) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent*) Q_DECL_OVERRIDE;
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent*) Q_DECL_OVERRIDE;
#endif
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent*) Q_DECL_OVERRIDE;
#endif
    void keyPressEvent(QKeyEvent*) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent*) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent*) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent*) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent*) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent*) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent*) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent*) Q_DECL_OVERRIDE;
    void inputMethodEvent(QInputMethodEvent*) Q_DECL_OVERRIDE;

    bool focusNextPrevChild(bool next) Q_DECL_OVERRIDE;

private:
    QUltralightPage* _page;
    QTimer* _progressTimer;
    int _progress = 0;

    // Ultralight
    ultralight::RefPtr<ultralight::Window> _window;
    ultralight::RefPtr<ultralight::Overlay> _overlay;
    static int QtModsToUlMods(int mods);
    static int QtKeyCodeToUlKeyCode(int key);
    static QString ulStringToQString(const ultralight::String string);
    static QUrl ulStringToQUrl(const ultralight::String string);
    static ultralight::String QStringToUlString(const QString string);
    static ultralight::String QUrlToUlString(const QUrl url);
    void OnResize(ultralight::Window* window, uint32_t width_px, uint32_t height_px) Q_DECL_OVERRIDE;
    void OnClose(ultralight::Window* window) Q_DECL_OVERRIDE;
    void OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor) Q_DECL_OVERRIDE;
    void OnChangeTitle(ultralight::View* caller, const ultralight::String& title) Q_DECL_OVERRIDE;
    void OnChangeURL(ultralight::View* caller, const ultralight::String& url) Q_DECL_OVERRIDE;
    void OnChangeTooltip(ultralight::View* caller, const ultralight::String& tooltip) Q_DECL_OVERRIDE;
    void OnAddConsoleMessage(ultralight::View* caller, ultralight::MessageSource source, ultralight::MessageLevel level,  const ultralight::String& message, uint32_t line_number, uint32_t column_number, const ultralight::String& source_id) Q_DECL_OVERRIDE;
    ultralight::RefPtr<ultralight::View> OnCreateChildView(ultralight::View* caller, const ultralight::String& opener_url, const ultralight::String& target_url, bool is_popup, const ultralight::IntRect& popup_rect) Q_DECL_OVERRIDE;
    void OnBeginLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url) Q_DECL_OVERRIDE;
    void OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url) Q_DECL_OVERRIDE;
    void OnFailLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url, const ultralight::String& description, const ultralight::String& error_domain, int error_code) Q_DECL_OVERRIDE;
    void OnWindowObjectReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url) Q_DECL_OVERRIDE;
    void OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url) Q_DECL_OVERRIDE;
    void OnUpdateHistory(ultralight::View* caller) Q_DECL_OVERRIDE;

    friend class QUltralightPage;
    friend class QUltralightFrame;
    friend class QUltralightHistory;
};

#endif // QULTRALIGHTVIEW_H
