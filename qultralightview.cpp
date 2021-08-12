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

#include "qultralightview.h"
#include "browserapplication.h"

#include <QWindow>

QUltralightView::QUltralightView(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents);
    setAcceptDrops(true);
    setMouseTracking(true);
    setFocusPolicy(Qt::WheelFocus);

    //Ultralight
    _window = ultralight::Window::Create(BrowserApplication::instance()->app->main_monitor(), 0, 0, false, ultralight::kWindowFlags_Hidden);
    page()->_overlay = ultralight::Overlay::Create(*_window.get(), 0, 0, 0, 0);
    page()->_overlay->Resize(_window->width(), _window->height());
    page()->_overlay->view()->set_load_listener(this);
    page()->_overlay->view()->set_view_listener(this);
    _window->set_listener(this);

    QWidget *webview = createWindowContainer(QWindow::fromWinId((unsigned long) _window->native_handle()), this);
    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(webview);
    setLayout(layout);
}

QUltralightView::~QUltralightView()
{
    page()->_overlay->view()->set_load_listener(nullptr);
    page()->_overlay->view()->set_view_listener(nullptr);
    _window->set_listener(nullptr);
    _window->Close();
}

QUltralightPage *QUltralightView::page() const
{
    if (!_page) {
        QUltralightView *that = const_cast<QUltralightView *>(this);
        that->setPage(new QUltralightPage(that));
    }
    return _page;
}

void QUltralightView::setPage(QUltralightPage* page)
{
    if (_page == page)
        return;

    _page = page;

    if (_page) {
        _page->setView(this);
        _page->setPalette(palette());
        // #### connect signals
        QUltralightFrame *mainFrame = _page->mainFrame();
        connect(mainFrame, SIGNAL(titleChanged(QString)),
            this, SIGNAL(titleChanged(QString)));
        connect(mainFrame, SIGNAL(iconChanged()),
            this, SIGNAL(iconChanged()));
        connect(mainFrame, SIGNAL(urlChanged(QUrl)),
            this, SIGNAL(urlChanged(QUrl)));

        connect(_page, SIGNAL(loadStarted()),
            this, SIGNAL(loadStarted()));
        connect(_page, SIGNAL(loadProgress(int)),
            this, SIGNAL(loadProgress(int)));
        connect(_page, SIGNAL(loadFinished(bool)),
            this, SIGNAL(loadFinished(bool)));
        connect(_page, SIGNAL(statusBarMessage(QString)),
            this, SIGNAL(statusBarMessage(QString)));
        connect(_page, SIGNAL(linkClicked(QUrl)),
            this, SIGNAL(linkClicked(QUrl)));
        connect(_page, SIGNAL(selectionChanged()),
            this, SIGNAL(selectionChanged()));

        connect(_page, SIGNAL(microFocusChanged()),
            this, SLOT(updateMicroFocus()));
        connect(_page, SIGNAL(destroyed()),
            this, SLOT(_q_pageDestroyed()));
    }
    setAttribute(Qt::WA_OpaquePaintEvent, _page);
    update();
}

void QUltralightView::load(const QUrl &url)
{
    page()->mainFrame()->load(url);
}

void QUltralightView::load(const QNetworkRequest &request, QNetworkAccessManager::Operation operation, const QByteArray &body)
{
    page()->mainFrame()->load(request, operation, body);
}

void QUltralightView::setHtml(const QString &html, const QUrl &baseUrl)
{
    page()->mainFrame()->setHtml(html, baseUrl);
}

void QUltralightView::setContent(const QByteArray &data, const QString &mimeType, const QUrl &baseUrl)
{
    page()->mainFrame()->setContent(data, mimeType, baseUrl);
}

QUltralightHistory *QUltralightView::history() const
{
    return page()->history();
}

QUltralightSettings *QUltralightView::settings() const
{
    return page()->settings();
}

QString QUltralightView::title() const
{
    return page()->mainFrame()->title();
}

void QUltralightView::setUrl(const QUrl &url)
{
    page()->mainFrame()->setUrl(url);
}

QUrl QUltralightView::url() const
{
    return page()->mainFrame()->url();
}

QIcon QUltralightView::icon() const
{
    return page()->mainFrame()->icon();
}

bool QUltralightView::hasSelection() const
{
    return page()->hasSelection();
}

QString QUltralightView::selectedText() const
{
    return page()->selectedText();
}


QString QUltralightView::selectedHtml() const
{
    return page()->selectedHtml();
}

#ifndef QT_NO_ACTION
QAction *QUltralightView::pageAction(QUltralightPage::WebAction action) const
{
    return page()->action(action);
}
#endif

void QUltralightView::triggerPageAction(QUltralightPage::WebAction action, bool checked)
{
    page()->triggerAction(action, checked);
}

bool QUltralightView::isModified() const
{
    return page()->isModified();
}

QSize QUltralightView::sizeHint() const
{
    return QSize(800, 600);
}

void QUltralightView::setZoomFactor(qreal factor)
{
    page()->mainFrame()->setZoomFactor(factor);
}

qreal QUltralightView::zoomFactor() const
{
    return page()->mainFrame()->zoomFactor();
}

void QUltralightView::setTextSizeMultiplier(qreal factor)
{
    page()->mainFrame()->setTextSizeMultiplier(factor);
}

qreal QUltralightView::textSizeMultiplier() const
{
    return page()->mainFrame()->textSizeMultiplier();
}

QPainter::RenderHints QUltralightView::renderHints() const
{
    return QPainter::RenderHints();
}

void QUltralightView::setRenderHints(QPainter::RenderHints hints)
{
//    if (hints == d->renderHints)
//        return;
//    d->renderHints = hints;
//    update();
}

void QUltralightView::setRenderHint(QPainter::RenderHint hint, bool enabled)
{
//    QPainter::RenderHints oldHints = d->renderHints;
//    if (enabled)
//        d->renderHints |= hint;
//    else
//        d->renderHints &= ~hint;
//    if (oldHints != d->renderHints)
//        update();
}

bool QUltralightView::findText(const QString &subString, QUltralightPage::FindFlags options)
{
    return page()->findText(subString, options);
}

bool QUltralightView::event(QEvent *e)
{
#ifndef QT_NO_CONTEXTMENU
    if (e->type() == QEvent::ContextMenu) {
        if (!isEnabled())
            return false;
        QContextMenuEvent *event = static_cast<QContextMenuEvent *>(e);
        if (page()->swallowContextMenuEvent(event)) {
            e->accept();
            return true;
        }
        page()->updatePositionDependentActions(event->pos());
    } else
#endif // QT_NO_CONTEXTMENU
    if (e->type() == QEvent::ShortcutOverride
        || e->type() == QEvent::Show
        || e->type() == QEvent::Hide) {
        page()->event(e);
#ifndef QT_NO_CURSOR
    } else if (e->type() == QEvent::CursorChange) {
        // An unsetCursor will set the cursor to Qt::ArrowCursor.
        // Thus this cursor change might be a QWidget::unsetCursor()
        // If this is not the case and it came from WebCore, the
        // QUltralightPageClient already has set its cursor internally
        // to Qt::ArrowCursor, so updating the cursor is always
        // right, as it falls back to the last cursor set by
        // WebCore.
        // FIXME: Add a QEvent::CursorUnset or similar to Qt.
        //if (cursor().shape() == Qt::ArrowCursor)
        //    page()->resetCursor();
#endif
    } else if (e->type() == QEvent::TouchBegin
        || e->type() == QEvent::TouchEnd
        || e->type() == QEvent::TouchUpdate
        || e->type() == QEvent::TouchCancel) {
        if (page()->event(e))
            return true;
    } else if (e->type() == QEvent::Leave)
        page()->event(e);
    return QWidget::event(e);
}

void QUltralightView::print(QPrinter *printer) const
{
#if !defined(QT_NO_PRINTER)
    page()->mainFrame()->print(printer);
#endif
}

void QUltralightView::stop()
{
    page()->triggerAction(QUltralightPage::Stop);
}

void QUltralightView::back()
{
    page()->triggerAction(QUltralightPage::Back);
}

void QUltralightView::forward()
{
    page()->triggerAction(QUltralightPage::Forward);
}

void QUltralightView::reload()
{
    page()->triggerAction(QUltralightPage::Reload);
}

void QUltralightView::resizeEvent(QResizeEvent *e)
{
    page()->setViewportSize(e->size());
}

void QUltralightView::paintEvent(QPaintEvent *ev)
{
#ifdef QWEBKIT_TIME_RENDERING
    QTime time;
    time.start();
#endif

    QUltralightFrame *frame = page()->mainFrame();
    QPainter p(this);
//    p.setRenderHints(d->renderHints);

    frame->render(&p, ev->region());

#ifdef    QWEBKIT_TIME_RENDERING
    int elapsed = time.elapsed();
    qDebug() << "paint event on " << ev->region() << ", took to render =  " << elapsed;
#endif
}

QUltralightView *QUltralightView::createWindow(QUltralightPage::WebWindowType type)
{
    Q_UNUSED(type)
    return 0;
}

void QUltralightView::mouseMoveEvent(QMouseEvent* ev)
{
    const bool accepted = ev->isAccepted();
    page()->event(ev);
    ev->setAccepted(accepted);
}

void QUltralightView::mousePressEvent(QMouseEvent* ev)
{
    const bool accepted = ev->isAccepted();
    page()->event(ev);
    ev->setAccepted(accepted);
}

void QUltralightView::mouseDoubleClickEvent(QMouseEvent* ev)
{
    const bool accepted = ev->isAccepted();
    page()->event(ev);
    ev->setAccepted(accepted);
}

void QUltralightView::mouseReleaseEvent(QMouseEvent* ev)
{
    const bool accepted = ev->isAccepted();
    page()->event(ev);
    ev->setAccepted(accepted);
}

#ifndef QT_NO_CONTEXTMENU
void QUltralightView::contextMenuEvent(QContextMenuEvent* ev)
{
    const bool accepted = ev->isAccepted();
    page()->event(ev);
    ev->setAccepted(accepted);
}
#endif // QT_NO_CONTEXTMENU

#ifndef QT_NO_WHEELEVENT
void QUltralightView::wheelEvent(QWheelEvent* ev)
{
    const bool accepted = ev->isAccepted();
    page()->event(ev);
    ev->setAccepted(accepted);
}
#endif // QT_NO_WHEELEVENT

void QUltralightView::keyPressEvent(QKeyEvent* ev)
{
    page()->event(ev);
    if (!ev->isAccepted())
        QWidget::keyPressEvent(ev);
}

void QUltralightView::keyReleaseEvent(QKeyEvent* ev)
{
    page()->event(ev);
    if (!ev->isAccepted())
        QWidget::keyReleaseEvent(ev);
}

void QUltralightView::focusInEvent(QFocusEvent* ev)
{
    page()->event(ev);
}

void QUltralightView::focusOutEvent(QFocusEvent* ev)
{
    page()->event(ev);
}

void QUltralightView::dragEnterEvent(QDragEnterEvent* ev)
{
//#if ENABLE(DRAG_SUPPORT)
    page()->event(ev);
//#endif
}

void QUltralightView::dragLeaveEvent(QDragLeaveEvent* ev)
{
//#if ENABLE(DRAG_SUPPORT)
    page()->event(ev);
//#endif
}

void QUltralightView::dragMoveEvent(QDragMoveEvent* ev)
{
//#if ENABLE(DRAG_SUPPORT)
    page()->event(ev);
//#endif
}

void QUltralightView::dropEvent(QDropEvent* ev)
{
//#if ENABLE(DRAG_SUPPORT)
    page()->event(ev);
//#endif
}

bool QUltralightView::focusNextPrevChild(bool next)
{
    if (page()->focusNextPrevChild(next))
        return true;
    return QWidget::focusNextPrevChild(next);
}

QVariant QUltralightView::inputMethodQuery(Qt::InputMethodQuery property) const
{
    return page()->inputMethodQuery(property);
}

void QUltralightView::inputMethodEvent(QInputMethodEvent *e)
{
    page()->event(e);
}

void QUltralightView::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::PaletteChange)
        page()->setPalette(palette());
    QWidget::changeEvent(e);
}

//ultralight::RefPtr<ultralight::Window> QUltralightView::ulWindow() const
//{
//    return _window;
//}

int QUltralightView::QtModsToUltralightMods(int mods)
{
    int result = 0;
    if (mods & Qt::AltModifier)
      result |= ultralight::KeyEvent::kMod_AltKey;
    if (mods & Qt::ControlModifier)
      result |= ultralight::KeyEvent::kMod_CtrlKey;
    if (mods & Qt::MetaModifier)
      result |= ultralight::KeyEvent::kMod_MetaKey;
    if (mods & Qt::ShiftModifier)
      result |= ultralight::KeyEvent::kMod_ShiftKey;
    return result;
}

int QUltralightView::QtKeyCodeToUltralightKeyCode(int key)
{
    using namespace ultralight::KeyCodes;
    switch (key) {
        case Qt::Key_Space: return GK_SPACE;
        case Qt::Key_Apostrophe: return GK_OEM_7;
        case Qt::Key_Comma: return GK_OEM_COMMA;
        case Qt::Key_Minus: return GK_OEM_MINUS;
        case Qt::Key_Period: return GK_OEM_PERIOD;
        case Qt::Key_Slash: return GK_OEM_2;
        case Qt::Key_0: return GK_0;
        case Qt::Key_1: return GK_1;
        case Qt::Key_2: return GK_2;
        case Qt::Key_3: return GK_3;
        case Qt::Key_4: return GK_4;
        case Qt::Key_5: return GK_5;
        case Qt::Key_6: return GK_6;
        case Qt::Key_7: return GK_7;
        case Qt::Key_8: return GK_8;
        case Qt::Key_9: return GK_9;
        case Qt::Key_Semicolon: return GK_OEM_1;
        case Qt::Key_Equal: return GK_OEM_PLUS;
        case Qt::Key_A: return GK_A;
        case Qt::Key_B: return GK_B;
        case Qt::Key_C: return GK_C;
        case Qt::Key_D: return GK_D;
        case Qt::Key_E: return GK_E;
        case Qt::Key_F: return GK_F;
        case Qt::Key_G: return GK_G;
        case Qt::Key_H: return GK_H;
        case Qt::Key_I: return GK_I;
        case Qt::Key_J: return GK_J;
        case Qt::Key_K: return GK_K;
        case Qt::Key_L: return GK_L;
        case Qt::Key_M: return GK_M;
        case Qt::Key_N: return GK_N;
        case Qt::Key_O: return GK_O;
        case Qt::Key_P: return GK_P;
        case Qt::Key_Q: return GK_Q;
        case Qt::Key_R: return GK_R;
        case Qt::Key_S: return GK_S;
        case Qt::Key_T: return GK_T;
        case Qt::Key_U: return GK_U;
        case Qt::Key_V: return GK_V;
        case Qt::Key_W: return GK_W;
        case Qt::Key_X: return GK_X;
        case Qt::Key_Y: return GK_Y;
        case Qt::Key_Z: return GK_Z;
        case Qt::Key_BracketLeft: return GK_OEM_4;
        case Qt::Key_Backslash: return GK_OEM_5;
        case Qt::Key_BracketRight: return GK_OEM_6;
        case Qt::Key_QuoteLeft: return GK_OEM_3;
        case Qt::Key_Escape: return GK_ESCAPE;
        case Qt::Key_Enter: return GK_RETURN;
        case Qt::Key_Tab: return GK_TAB;
        case Qt::Key_Backspace: return GK_BACK;
        case Qt::Key_Insert: return GK_INSERT;
        case Qt::Key_Delete: return GK_DELETE;
        case Qt::Key_Right: return GK_RIGHT;
        case Qt::Key_Left: return GK_LEFT;
        case Qt::Key_Down: return GK_DOWN;
        case Qt::Key_Up: return GK_UP;
        case Qt::Key_PageUp: return GK_PRIOR;
        case Qt::Key_PageDown: return GK_NEXT;
        case Qt::Key_Home: return GK_HOME;
        case Qt::Key_End: return GK_END;
        case Qt::Key_CapsLock: return GK_CAPITAL;
        case Qt::Key_ScrollLock: return GK_SCROLL;
        case Qt::Key_NumLock: return GK_NUMLOCK;
        case Qt::Key_SysReq: return GK_SNAPSHOT;
        case Qt::Key_Pause: return GK_PAUSE;
        case Qt::Key_F1: return GK_F1;
        case Qt::Key_F2: return GK_F2;
        case Qt::Key_F3: return GK_F3;
        case Qt::Key_F4: return GK_F4;
        case Qt::Key_F5: return GK_F5;
        case Qt::Key_F6: return GK_F6;
        case Qt::Key_F7: return GK_F7;
        case Qt::Key_F8: return GK_F8;
        case Qt::Key_F9: return GK_F9;
        case Qt::Key_F10: return GK_F10;
        case Qt::Key_F11: return GK_F11;
        case Qt::Key_F12: return GK_F12;
        case Qt::Key_F13: return GK_F13;
        case Qt::Key_F14: return GK_F14;
        case Qt::Key_F15: return GK_F15;
        case Qt::Key_F16: return GK_F16;
        case Qt::Key_F17: return GK_F17;
        case Qt::Key_F18: return GK_F18;
        case Qt::Key_F19: return GK_F19;
        case Qt::Key_F20: return GK_F20;
        case Qt::Key_F21: return GK_F21;
        case Qt::Key_F22: return GK_F22;
        case Qt::Key_F23: return GK_F23;
        case Qt::Key_F24: return GK_F24;
        case Qt::Key_Shift: return GK_SHIFT;
        case Qt::Key_Control: return GK_CONTROL;
        case Qt::Key_Alt: return GK_MENU;
        case Qt::Key_Super_L: return GK_LWIN;
        case Qt::Key_AltGr: return GK_MENU;
        case Qt::Key_Super_R: return GK_RWIN;
        default: return GK_UNKNOWN;
    }
}

void QUltralightView::OnResize(ultralight::Window *window, uint32_t width_px, uint32_t height_px)
{
    page()->_overlay->Resize(width_px, height_px);
}

void QUltralightView::OnClose(ultralight::Window *window)
{
    BrowserApplication::instance()->app->Quit();
}

void QUltralightView::OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor) {
    _window->SetCursor(cursor);
}

void QUltralightView::sendKey(QKeyEvent *event)
{
    ultralight::KeyEvent evt;
    evt.type = ultralight::KeyEvent::kType_RawKeyDown;
    evt.virtual_key_code = QtKeyCodeToUltralightKeyCode(event->key());
    evt.native_key_code = event->nativeScanCode();
    GetKeyIdentifierFromVirtualKeyCode(event->nativeVirtualKey(), evt.key_identifier);
    evt.modifiers = QtModsToUltralightMods(event->modifiers());
    page()->_overlay->view()->FireKeyEvent(evt);
    // Support typing chars
    evt.type = ultralight::KeyEvent::kType_Char;
    evt.text = (ultralight::String8) event->text().toUtf8().constData();
    evt.unmodified_text = evt.text;
    page()->_overlay->view()->FireKeyEvent(evt);
}
