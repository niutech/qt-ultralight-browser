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

#include "qultralightpage.h"
#include "qultralightview.h"

#include <QAction>
#include <QApplication>
#include <QBitArray>
#include <QClipboard>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QStyle>
#include <QUndoCommand>

using namespace WebCore;

static QUltralightPage::WebAction webActionForAdapterMenuAction(QUltralightPage::MenuAction action)
{
    switch (action) {
    case QUltralightPage::MenuAction::OpenLink: return QUltralightPage::OpenLink;
    case QUltralightPage::MenuAction::OpenLinkInNewWindow: return QUltralightPage::OpenLinkInNewWindow;
    case QUltralightPage::MenuAction::OpenFrameInNewWindow: return QUltralightPage::OpenFrameInNewWindow;
    case QUltralightPage::MenuAction::DownloadLinkToDisk: return QUltralightPage::DownloadLinkToDisk;
    case QUltralightPage::MenuAction::CopyLinkToClipboard: return QUltralightPage::CopyLinkToClipboard;
    case QUltralightPage::MenuAction::OpenImageInNewWindow: return QUltralightPage::OpenImageInNewWindow;
    case QUltralightPage::MenuAction::DownloadImageToDisk: return QUltralightPage::DownloadImageToDisk;
    case QUltralightPage::MenuAction::CopyImageToClipboard: return QUltralightPage::CopyImageToClipboard;
    case QUltralightPage::MenuAction::Back: return QUltralightPage::Back;
    case QUltralightPage::MenuAction::Forward: return QUltralightPage::Forward;
    case QUltralightPage::MenuAction::Stop: return QUltralightPage::Stop;
    case QUltralightPage::MenuAction::Reload: return QUltralightPage::Reload;
    case QUltralightPage::MenuAction::Cut: return QUltralightPage::Cut;
    case QUltralightPage::MenuAction::Copy: return QUltralightPage::Copy;
    case QUltralightPage::MenuAction::Paste: return QUltralightPage::Paste;
    case QUltralightPage::MenuAction::Undo: return QUltralightPage::Undo;
    case QUltralightPage::MenuAction::Redo: return QUltralightPage::Redo;
    case QUltralightPage::MenuAction::SetTextDirectionDefault: return QUltralightPage::SetTextDirectionDefault;
    case QUltralightPage::MenuAction::SetTextDirectionLeftToRight: return QUltralightPage::SetTextDirectionLeftToRight;
    case QUltralightPage::MenuAction::SetTextDirectionRightToLeft: return QUltralightPage::SetTextDirectionRightToLeft;
    case QUltralightPage::MenuAction::ToggleBold: return QUltralightPage::ToggleBold;
    case QUltralightPage::MenuAction::ToggleItalic: return QUltralightPage::ToggleItalic;
    case QUltralightPage::MenuAction::ToggleUnderline: return QUltralightPage::ToggleUnderline;
    case QUltralightPage::MenuAction::InspectElement: return QUltralightPage::InspectElement;
    case QUltralightPage::MenuAction::SelectAll: return QUltralightPage::SelectAll;
    case QUltralightPage::MenuAction::CopyImageUrlToClipboard: return QUltralightPage::CopyImageUrlToClipboard;
    case QUltralightPage::MenuAction::OpenLinkInThisWindow: return QUltralightPage::OpenLinkInThisWindow;
    case QUltralightPage::MenuAction::DownloadMediaToDisk: return QUltralightPage::DownloadMediaToDisk;
    case QUltralightPage::MenuAction::CopyMediaUrlToClipboard: return QUltralightPage::CopyMediaUrlToClipboard;
    case QUltralightPage::MenuAction::ToggleMediaControls: return QUltralightPage::ToggleMediaControls;
    case QUltralightPage::MenuAction::ToggleMediaLoop: return QUltralightPage::ToggleMediaLoop;
    case QUltralightPage::MenuAction::ToggleMediaPlayPause: return QUltralightPage::ToggleMediaPlayPause;
    case QUltralightPage::MenuAction::ToggleMediaMute: return QUltralightPage::ToggleMediaMute;
    case QUltralightPage::MenuAction::ToggleVideoFullscreen: return QUltralightPage::ToggleVideoFullscreen;
    case QUltralightPage::MenuAction::ActionCount: return QUltralightPage::WebActionCount;
    default:
        Q_UNREACHABLE();
        break;
    }
    return QUltralightPage::NoWebAction;
}

static QUltralightPage::MenuAction adapterMenuActionForWebAction(QUltralightPage::WebAction action)
{
    switch (action) {
    case QUltralightPage::OpenLink: return QUltralightPage::MenuAction::OpenLink;
    case QUltralightPage::OpenLinkInNewWindow: return QUltralightPage::MenuAction::OpenLinkInNewWindow;
    case QUltralightPage::OpenFrameInNewWindow: return QUltralightPage::MenuAction::OpenFrameInNewWindow;
    case QUltralightPage::DownloadLinkToDisk: return QUltralightPage::MenuAction::DownloadLinkToDisk;
    case QUltralightPage::CopyLinkToClipboard: return QUltralightPage::MenuAction::CopyLinkToClipboard;
    case QUltralightPage::OpenImageInNewWindow: return QUltralightPage::MenuAction::OpenImageInNewWindow;
    case QUltralightPage::DownloadImageToDisk: return QUltralightPage::MenuAction::DownloadImageToDisk;
    case QUltralightPage::CopyImageToClipboard: return QUltralightPage::MenuAction::CopyImageToClipboard;
    case QUltralightPage::Back: return QUltralightPage::MenuAction::Back;
    case QUltralightPage::Forward: return QUltralightPage::MenuAction::Forward;
    case QUltralightPage::Stop: return QUltralightPage::MenuAction::Stop;
    case QUltralightPage::Reload: return QUltralightPage::MenuAction::Reload;
    case QUltralightPage::Cut: return QUltralightPage::MenuAction::Cut;
    case QUltralightPage::Copy: return QUltralightPage::MenuAction::Copy;
    case QUltralightPage::Paste: return QUltralightPage::MenuAction::Paste;
    case QUltralightPage::Undo: return QUltralightPage::MenuAction::Undo;
    case QUltralightPage::Redo: return QUltralightPage::MenuAction::Redo;
    case QUltralightPage::SetTextDirectionDefault: return QUltralightPage::MenuAction::SetTextDirectionDefault;
    case QUltralightPage::SetTextDirectionLeftToRight: return QUltralightPage::MenuAction::SetTextDirectionLeftToRight;
    case QUltralightPage::SetTextDirectionRightToLeft: return QUltralightPage::MenuAction::SetTextDirectionRightToLeft;
    case QUltralightPage::ToggleBold: return QUltralightPage::MenuAction::ToggleBold;
    case QUltralightPage::ToggleItalic: return QUltralightPage::MenuAction::ToggleItalic;
    case QUltralightPage::ToggleUnderline: return QUltralightPage::MenuAction::ToggleUnderline;
    case QUltralightPage::InspectElement: return QUltralightPage::MenuAction::InspectElement;
    case QUltralightPage::SelectAll: return QUltralightPage::MenuAction::SelectAll;
    case QUltralightPage::CopyImageUrlToClipboard: return QUltralightPage::MenuAction::CopyImageUrlToClipboard;
    case QUltralightPage::OpenLinkInThisWindow: return QUltralightPage::MenuAction::OpenLinkInThisWindow;
    case QUltralightPage::DownloadMediaToDisk: return QUltralightPage::MenuAction::DownloadMediaToDisk;
    case QUltralightPage::CopyMediaUrlToClipboard: return QUltralightPage::MenuAction::CopyMediaUrlToClipboard;
    case QUltralightPage::ToggleMediaControls: return QUltralightPage::MenuAction::ToggleMediaControls;
    case QUltralightPage::ToggleMediaLoop: return QUltralightPage::MenuAction::ToggleMediaLoop;
    case QUltralightPage::ToggleMediaPlayPause: return QUltralightPage::MenuAction::ToggleMediaPlayPause;
    case QUltralightPage::ToggleMediaMute: return QUltralightPage::MenuAction::ToggleMediaMute;
    case QUltralightPage::ToggleVideoFullscreen: return QUltralightPage::MenuAction::ToggleVideoFullscreen;
    case QUltralightPage::WebActionCount: return QUltralightPage::MenuAction::ActionCount;
    default:
        Q_UNREACHABLE();
        break;
    }
    return QUltralightPage::MenuAction::NoAction;
}

QUltralightPage::QUltralightPage(QObject *parent) : QObject(parent)
{
    setView(qobject_cast<QWidget*>(parent));
    _history = new QUltralightHistory(this);
    createMainFrame();

    connect(mainFrame(), SIGNAL(loadStarted()), this, SIGNAL(loadStarted()));
    connect(mainFrame(), SIGNAL(loadFinished(bool)), this, SIGNAL(loadFinished(bool)));
}

QUltralightPage::~QUltralightPage()
{
}

void QUltralightPage::createMainFrame()
{
    _mainFrame = new QUltralightFrame(this);
    emit frameCreated(_mainFrame);
}

QUltralightFrame *QUltralightPage::mainFrame() const
{
    if (!_mainFrame) {
        QUltralightPage *that = const_cast<QUltralightPage *>(this);
        that->createMainFrame();
    }
    return _mainFrame;
}

QUltralightFrame *QUltralightPage::currentFrame() const
{
    if (!_mainFrame) {
        QUltralightPage *that = const_cast<QUltralightPage *>(this);
        that->createMainFrame();
    }
    return qobject_cast<QUltralightFrame*>(_mainFrame);
}

QUltralightFrame* QUltralightPage::frameAt(const QPoint& pos) const
{
    QUltralightFrame* webFrame = mainFrame();
    if (!webFrame->geometry().contains(pos))
        return 0;
    QUltralightHitTestResult hitTestResult = webFrame->hitTestContent(pos);
    return hitTestResult.frame();
}

QUltralightHistory *QUltralightPage::history() const
{
    if (!_mainFrame) {
        QUltralightPage *that = const_cast<QUltralightPage *>(this);
        that->createMainFrame();
    }
    return _history;
}

void QUltralightPage::setView(QWidget* view)
{
    if (this->view() == view)
        return;

    _view = view;
    setViewportSize(view ? view->size() : QSize(0, 0));

    // If we have no client, we install a special client delegating
    // the responsibility to the QWidget. This is the code path
    // handling a.o. the "legacy" QUltralightView.
    //
    // If such a special delegate already exist, we substitute the view.

//    if (d->client) {
//        if (d->client->isQWidgetClient())
//            static_cast<PageClientQWidget*>(d->client.data())->view = view;
//        return;
//    }

//    if (view)
//        d->client.reset(new PageClientQWidget(view, this));
}

QWidget *QUltralightPage::view() const
{
    return _view;
}

void QUltralightPage::javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID)
{
    Q_UNUSED(sourceID);

    // Catch plugin logDestroy message for LayoutTests/plugins/open-and-close-window-with-plugin.html
    // At this point DRT's WebPage has already been destroyed
//    if (QUltralightPageAdapter::drtRun) {
//        if (message == QLatin1String("PLUGIN: NPP_Destroy")) {
//            fprintf(stdout, "CONSOLE MESSAGE: ");
//            if (lineNumber)
//                fprintf(stdout, "line %d: ", lineNumber);
//            fprintf(stdout, "%s\n", message.toUtf8().constData());
//        }
//    }
}

void QUltralightPage::javaScriptAlert(QUltralightFrame *frame, const QString& msg)
{
    Q_UNUSED(frame);
#ifndef QT_NO_MESSAGEBOX
    QMessageBox box(view());
    box.setWindowTitle(tr("JavaScript Alert - %1").arg(mainFrame()->url().host()));
    box.setTextFormat(Qt::PlainText);
    box.setText(msg);
    box.setStandardButtons(QMessageBox::Ok);
    box.exec();
#endif
}

bool QUltralightPage::javaScriptConfirm(QUltralightFrame *frame, const QString& msg)
{
    Q_UNUSED(frame);
#ifdef QT_NO_MESSAGEBOX
    return true;
#else
    QMessageBox box(view());
    box.setWindowTitle(tr("JavaScript Confirm - %1").arg(mainFrame()->url().host()));
    box.setTextFormat(Qt::PlainText);
    box.setText(msg);
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    return QMessageBox::Ok == box.exec();
#endif
}

bool QUltralightPage::javaScriptPrompt(QUltralightFrame *frame, const QString& msg, const QString& defaultValue, QString* result)
{
    Q_UNUSED(frame);
    bool ok = false;
#ifndef QT_NO_INPUTDIALOG

    QInputDialog dlg(view());
    dlg.setWindowTitle(tr("JavaScript Prompt - %1").arg(mainFrame()->url().host()));

    // Hack to force the dialog's QLabel into plain text mode
    // prevents https://bugs.webkit.org/show_bug.cgi?id=34429
    QLabel* label = dlg.findChild<QLabel*>();
    if (label)
        label->setTextFormat(Qt::PlainText);

    // double the &'s because single & will underline the following character
    // (Accelerator mnemonics)
    QString escMsg(msg);
    escMsg.replace(QChar::fromLatin1('&'), QLatin1String("&&"));
    dlg.setLabelText(escMsg);

    dlg.setTextEchoMode(QLineEdit::Normal);
    dlg.setTextValue(defaultValue);

    ok = !!dlg.exec();

    if (ok && result)
        *result = dlg.textValue();
#endif
    return ok;
}

bool QUltralightPage::shouldInterruptJavaScript()
{
#ifdef QT_NO_MESSAGEBOX
    return false;
#else
    return QMessageBox::Yes == QMessageBox::information(view(), tr("JavaScript Problem - %1").arg(mainFrame()->url().host()), tr("The script on this page appears to have a problem. Do you want to stop the script?"), QMessageBox::Yes, QMessageBox::No);
#endif
}

void QUltralightPage::setFeaturePermission(QUltralightFrame* frame, Feature feature, PermissionPolicy policy)
{
//#if !ENABLE(NOTIFICATIONS) && !ENABLE(GEOLOCATION)
//    Q_UNUSED(frame);
//    Q_UNUSED(policy);
//#endif
//    switch (feature) {
//    case Notifications:
//#if ENABLE(NOTIFICATIONS)
//        if (policy != PermissionUnknown)
//            d->setNotificationsAllowedForFrame(frame->d, (policy == PermissionGrantedByUser));
//#endif
//        break;
//    case Geolocation:
//#if ENABLE(GEOLOCATION) && HAVE(QTPOSITIONING)
//        if (policy != PermissionUnknown)
//            d->setGeolocationEnabledForFrame(frame->d, (policy == PermissionGrantedByUser));
//#endif
//        break;

//    default:
//        break;
//    }
}

QUltralightPage *QUltralightPage::createWindow(WebWindowType type)
{
    QUltralightView *webView = qobject_cast<QUltralightView*>(view());
    if (webView) {
        QUltralightView *newView = webView->createWindow(type);
        if (newView)
            return newView->page();
    }
    return 0;
}

QObject *QUltralightPage::createPlugin(const QString &classid, const QUrl &url, const QStringList &paramNames, const QStringList &paramValues)
{
    Q_UNUSED(classid);
    Q_UNUSED(url);
    Q_UNUSED(paramNames);
    Q_UNUSED(paramValues);
    return 0;
}

QStringList QUltralightPage::supportedContentTypes() const
{
//    return d->supportedContentTypes();
    return QStringList();
}

bool QUltralightPage::supportsContentType(const QString& mimeType) const
{
//    return d->supportsContentType(mimeType);
    return true;
}

static void collectChildFrames(QUltralightFrame* frame, QList<QUltralightFrame*>& list)
{
    list << frame->childFrames();
    QListIterator<QUltralightFrame*> it(frame->childFrames());
    while (it.hasNext())
        collectChildFrames(it.next(), list);
}

void QUltralightPage::triggerAction(WebAction action, bool)
{
//    const char *command = 0;
    MenuAction mappedAction = MenuAction::NoAction;

    switch (action) {
    case OpenLink:
    case OpenLinkInNewWindow:
    case OpenLinkInThisWindow:
    case OpenFrameInNewWindow:
    case CopyLinkToClipboard:
    case OpenImageInNewWindow:
    case DownloadImageToDisk:
    case DownloadLinkToDisk:
    case Back:
    case Forward:
    case Stop:
    case Reload:
    case SetTextDirectionDefault:
    case SetTextDirectionLeftToRight:
    case SetTextDirectionRightToLeft:
    case DownloadMediaToDisk:
    case ToggleMediaControls:
    case ToggleMediaLoop:
    case ToggleMediaPlayPause:
    case ToggleMediaMute:
    case ToggleVideoFullscreen:
        mappedAction = adapterMenuActionForWebAction(action);
        break;
    case ReloadAndBypassCache: // Manual mapping
        mappedAction = MenuAction::Reload;
        break;
#ifndef QT_NO_CLIPBOARD
    case CopyImageToClipboard:
        QApplication::clipboard()->setPixmap(_hitTestResult->pixmap());
        break;
    case CopyImageUrlToClipboard:
        QApplication::clipboard()->setText(_hitTestResult->imageUrl().toString());
        break;
    case CopyMediaUrlToClipboard:
        QApplication::clipboard()->setText(_hitTestResult->mediaUrl().toString());
        break;
#endif
    case InspectElement: {
        if (!_hitTestResult->isNull()) {
            mappedAction = MenuAction::InspectElement;
            //Ultralight
            qobject_cast<QUltralightView*>(view())->_overlay->view()->inspector()->Resize(qobject_cast<QUltralightView*>(view())->_overlay->width(), 400);
        }
        break;
    }
    case StopScheduledPageRefresh: {
//        QUltralightFrame* topFrame = mainFrame();
//        topFrame->d->cancelLoad();
//        QList<QUltralightFrame*> childFrames;
//        collectChildFrames(topFrame, childFrames);
//        QListIterator<QUltralightFrame*> it(childFrames);
//        while (it.hasNext())
//            it.next()->d->cancelLoad();
        mappedAction = MenuAction::Stop;
        break;
    }
    case RequestClose: {
//        bool success = d->tryClosePage();
//        if (success)
            emit windowCloseRequested();
        break;
    }
    default:
//        command = QUltralightPage::editorCommandForWebActions(action);
        break;
    }
//    if (command || mappedAction != MenuAction::NoAction)
//        d->triggerAction(mappedAction, hitTestResult, command, /*endToEndReload*/ action == ReloadAndBypassCache);
}

QSize QUltralightPage::viewportSize() const
{
    return QSize(qobject_cast<QUltralightView*>(view())->_overlay->width(), qobject_cast<QUltralightView*>(view())->_overlay->height());
}

void QUltralightPage::setViewportSize(const QSize &size) const
{
    //qobject_cast<QUltralightView*>(view())->_overlay->Resize(size.width(), size.height());
}

void QUltralightPage::setDevicePixelRatio(qreal ratio)
{
//    d->setDevicePixelRatio(ratio);
//    d->m_customDevicePixelRatioIsSet = true;
}

qreal QUltralightPage::devicePixelRatio() const
{
//    return d->devicePixelRatio();
    return 1;
}

void QUltralightPage::resetDevicePixelRatio()
{
//    d->m_customDevicePixelRatioIsSet = false;
//    d->updateWindow();
}

static int getintenv(const char* variable)
{
    bool ok;
    int value = qgetenv(variable).toInt(&ok);
    return (ok) ? value : -1;
}

static QSize queryDeviceSizeForScreenContainingWidget(const QWidget* widget)
{
    QDesktopWidget* desktop = QApplication::desktop();
    if (!desktop)
        return QSize();

    QSize size;

    if (widget) {
        // Returns the available geometry of the screen which contains widget.
        // NOTE: this must be the the full screen size including any fixed status areas etc.
        size = desktop->availableGeometry(widget).size();
    } else
        size = desktop->availableGeometry().size();

    // This must be in portrait mode, adjust if not.
    if (size.width() > size.height()) {
        int width = size.width();
        size.setWidth(size.height());
        size.setHeight(width);
    }

    return size;
}

//QUltralightPage::ViewportAttributes QUltralightPage::viewportAttributesForSize(const QSize& availableSize) const
//{
//    ViewportAttributes result;

//    if (availableSize.isEmpty())
//        return result; // Returns an invalid instance.

//    QSize deviceSize(getintenv("QTWEBKIT_DEVICE_WIDTH"), getintenv("QTWEBKIT_DEVICE_HEIGHT"));

//    // Both environment variables need to be set - or they will be ignored.
//    if (deviceSize.isNull())
//        deviceSize = queryDeviceSizeForScreenContainingWidget(view());
//    ViewportAttributes attr = d->viewportAttributesForSize(availableSize, deviceSize);

//    result.m_isValid = true;
//    result.m_size = attr.size;
//    result.m_initialScaleFactor = attr.initialScaleFactor;
//    result.m_minimumScaleFactor = attr.minimumScaleFactor;
//    result.m_maximumScaleFactor = attr.maximumScaleFactor;
//    result.m_devicePixelRatio = attr.devicePixelRatio;
//    result.m_isUserScalable = attr.isUserScalable;

//    return result;
//}

QSize QUltralightPage::preferredContentsSize() const
{
//    QUltralightFrameAdapter* mainFrame = d->mainFrame ? d->mainFrame->d : 0;
//    QSize customSize;
//    if (mainFrame && mainFrame->hasView())
//        customSize = mainFrame->customLayoutSize();

//    return customSize.isNull() ? d->fixedLayoutSize : customSize;
    return mainFrame()->contentsSize();
}

void QUltralightPage::setPreferredContentsSize(const QSize& size) const
{
    // FIXME: Rename this method to setCustomLayoutSize

//    d->fixedLayoutSize = size;

//    QUltralightFrameAdapter& mainFrame = d->mainFrameAdapter();
//    if (!mainFrame.hasView())
//        return;

//    mainFrame.setCustomLayoutSize(size);
}

void QUltralightPage::setActualVisibleContentRect(const QRect& rect) const
{
//    QUltralightFrameAdapter& mainFrame = d->mainFrameAdapter();
//    if (!mainFrame.hasView())
//        return;

//    mainFrame.setFixedVisibleContentRect(rect);
}

bool QUltralightPage::acceptNavigationRequest(QUltralightFrame *frame, const QNetworkRequest &request, QUltralightPage::NavigationType type)
{
//    Q_UNUSED(frame);
//    if (type == NavigationTypeLinkClicked) {
//        switch (d->linkPolicy) {
//        case DontDelegateLinks:
//            return true;

//        case DelegateExternalLinks:
//            if (request.url().scheme().isEmpty() && QUltralightPageAdapter::treatSchemeAsLocal(frame->baseUrl().scheme()))
//                return true;
//            if (QUltralightPageAdapter::treatSchemeAsLocal(request.url().scheme()))
//                return true;
//            emit linkClicked(request.url());
//            return false;

//        case DelegateAllLinks:
//            emit linkClicked(request.url());
//            return false;
//        }
//    }
    return true;
}

bool QUltralightPage::hasSelection() const
{
//    d->createMainFrame();
//    return d->hasSelection();
    return false;
}

QString QUltralightPage::selectedText() const
{
//    d->createMainFrame();
//    return d->selectedText();
    return QString();
}

QString QUltralightPage::selectedHtml() const
{
//    d->createMainFrame();
//    return d->selectedHtml();
    return QString();
}

#ifndef QT_NO_ACTION
QAction *QUltralightPage::action(WebAction action) const
{
    if (action == NoWebAction)
        return 0;
//    if (d->actions[action])
//        return d->actions[action];

    QString text;
    QIcon icon;
    QStyle *style = qApp->style();
    bool checkable = false;
    MenuAction mappedAction = MenuAction::NoAction;

    switch (action) {
    // to be fetched from LocalizedStringsQt via the page adapter
    case OpenLink:
        text = tr("Open link");
        mappedAction = MenuAction::OpenLink;
        break;
    case OpenLinkInNewWindow:
        text = tr("Open link in new window");
        mappedAction = MenuAction::OpenLinkInNewWindow;
        break;
    case OpenFrameInNewWindow:
        text = tr("Open frame in new window");
        mappedAction = MenuAction::OpenFrameInNewWindow;
        break;
    case OpenLinkInThisWindow:
        text = tr("Open link in this window");
        mappedAction = MenuAction::OpenLinkInThisWindow;
        break;
    case DownloadLinkToDisk:
        text = tr("Download link to disk");
        mappedAction = MenuAction::DownloadLinkToDisk;
        break;
    case CopyLinkToClipboard:
        text = tr("Copy link to clipboard");
        mappedAction = MenuAction::CopyLinkToClipboard;
        break;
    case OpenImageInNewWindow:
        text = tr("Open image in new window");
        mappedAction = MenuAction::OpenImageInNewWindow;
        break;
    case DownloadImageToDisk:
        text = tr("Download image to disk");
        mappedAction = MenuAction::DownloadImageToDisk;
        break;
    case CopyImageToClipboard:
        text = tr("Copy image to clipboard");
        mappedAction = MenuAction::CopyImageToClipboard;
        break;
    case CopyImageUrlToClipboard:
        text = tr("Copy image URL to clipboard");
        mappedAction = MenuAction::CopyImageUrlToClipboard;
        break;
    case Cut:
        text = tr("Cut");
        mappedAction = MenuAction::Cut;
        break;
    case Copy:
        text = tr("Copy");
        mappedAction = MenuAction::Copy;
        break;
    case Paste:
        text = tr("Paste");
        mappedAction = MenuAction::Paste;
        break;
#ifdef QT_NO_UNDOSTACK
    case Undo:
        text = tr("Undo");
        mappedAction = MenuAction::Undo;
        break;
    case Redo:
        text = tr("Redo");
        mappedAction = MenuAction::Redo;
        break;
#endif // QT_NO_UNDOSTACK
    case SelectAll:
        text = tr("Select all");
        mappedAction = MenuAction::SelectAll;
        break;
    case SetTextDirectionDefault:
        text = tr("Set text direction Default");
        mappedAction = MenuAction::SetTextDirectionDefault;
        break;
    case SetTextDirectionLeftToRight:
        text = tr("Set text direction Left To Right");
        mappedAction = MenuAction::SetTextDirectionLeftToRight;
        break;
    case SetTextDirectionRightToLeft:
        text = tr("Set text direction Right To Left");
        mappedAction = MenuAction::SetTextDirectionRightToLeft;
        break;
    case ToggleBold:
        text = tr("Toggle bold");
        mappedAction = MenuAction::ToggleBold;
        break;
    case ToggleItalic:
        text = tr("Toggle italic");
        mappedAction = MenuAction::ToggleItalic;
        break;
    case ToggleUnderline:
        text = tr("Toggle underline");
        mappedAction = MenuAction::ToggleUnderline;
        break;
    case DownloadMediaToDisk:
        text = tr("Download media to disk");
        mappedAction = MenuAction::DownloadMediaToDisk;
        break;
    case CopyMediaUrlToClipboard:
        text = tr("Copy media URL to clipboard");
        mappedAction = MenuAction::CopyMediaUrlToClipboard;
        break;
    case ToggleMediaControls:
        text = tr("Toggle media controls");
        mappedAction = MenuAction::ToggleMediaControls;
        break;
    case ToggleMediaLoop:
        text = tr("Toggle media loop");
        mappedAction = MenuAction::ToggleMediaLoop;
        break;
    case ToggleMediaPlayPause:
        text = tr("Toggle media play/pause");
        mappedAction = MenuAction::ToggleMediaPlayPause;
        break;
    case ToggleMediaMute:
        text = tr("Toggle media mute");
        mappedAction = MenuAction::ToggleMediaMute;
        break;
    case ToggleVideoFullscreen:
        text = tr("Toggle video fullscreen");
        mappedAction = MenuAction::ToggleVideoFullscreen;
        break;
    case InspectElement:
        text = tr("Inspect element");
        mappedAction = MenuAction::InspectElement;
        break;
    case Back:
        text = tr("Back");
        mappedAction = MenuAction::Back;
        icon = style->standardIcon(QStyle::SP_ArrowBack);
        break;
    case Forward:
        text = tr("Forward");
        mappedAction = MenuAction::Forward;
        icon = style->standardIcon(QStyle::SP_ArrowForward);
        break;
    case Stop:
        text = tr("Stop");
        mappedAction = MenuAction::Stop;
        icon = style->standardIcon(QStyle::SP_BrowserStop);
        break;
    case Reload:
        text = tr("Reload");
        mappedAction = MenuAction::Reload;
        icon = style->standardIcon(QStyle::SP_BrowserReload);
        break;

#ifndef QT_NO_UNDOSTACK
    case Undo: {
        QAction *a = undoStack()->createUndoAction(const_cast<QUltralightPage *>(this));
//        d->actions[action] = a;
        return a;
    }
    case Redo: {
        QAction *a = undoStack()->createRedoAction(const_cast<QUltralightPage *>(this));
//        d->actions[action] = a;
        return a;
    }
#endif // QT_NO_UNDOSTACK
    // in place l10n
    case MoveToNextChar:
        text = tr("Move the cursor to the next character");
        break;
    case MoveToPreviousChar:
        text = tr("Move the cursor to the previous character");
        break;
    case MoveToNextWord:
        text = tr("Move the cursor to the next word");
        break;
    case MoveToPreviousWord:
        text = tr("Move the cursor to the previous word");
        break;
    case MoveToNextLine:
        text = tr("Move the cursor to the next line");
        break;
    case MoveToPreviousLine:
        text = tr("Move the cursor to the previous line");
        break;
    case MoveToStartOfLine:
        text = tr("Move the cursor to the start of the line");
        break;
    case MoveToEndOfLine:
        text = tr("Move the cursor to the end of the line");
        break;
    case MoveToStartOfBlock:
        text = tr("Move the cursor to the start of the block");
        break;
    case MoveToEndOfBlock:
        text = tr("Move the cursor to the end of the block");
        break;
    case MoveToStartOfDocument:
        text = tr("Move the cursor to the start of the document");
        break;
    case MoveToEndOfDocument:
        text = tr("Move the cursor to the end of the document");
        break;
    case SelectNextChar:
        text = tr("Select to the next character");
        break;
    case SelectPreviousChar:
        text = tr("Select to the previous character");
        break;
    case SelectNextWord:
        text = tr("Select to the next word");
        break;
    case SelectPreviousWord:
        text = tr("Select to the previous word");
        break;
    case SelectNextLine:
        text = tr("Select to the next line");
        break;
    case SelectPreviousLine:
        text = tr("Select to the previous line");
        break;
    case SelectStartOfLine:
        text = tr("Select to the start of the line");
        break;
    case SelectEndOfLine:
        text = tr("Select to the end of the line");
        break;
    case SelectStartOfBlock:
        text = tr("Select to the start of the block");
        break;
    case SelectEndOfBlock:
        text = tr("Select to the end of the block");
        break;
    case SelectStartOfDocument:
        text = tr("Select to the start of the document");
        break;
    case SelectEndOfDocument:
        text = tr("Select to the end of the document");
        break;
    case DeleteStartOfWord:
        text = tr("Delete to the start of the word");
        break;
    case DeleteEndOfWord:
        text = tr("Delete to the end of the word");
        break;

    case InsertParagraphSeparator:
        text = tr("Insert a new paragraph");
        break;
    case InsertLineSeparator:
        text = tr("Insert a new line");
        break;

    case PasteAndMatchStyle:
        text = tr("Paste and Match Style");
        break;
    case RemoveFormat:
        text = tr("Remove formatting");
        break;

    case ToggleStrikethrough:
        text = tr("Strikethrough");
        checkable = true;
        break;
    case ToggleSubscript:
        text = tr("Subscript");
        checkable = true;
        break;
    case ToggleSuperscript:
        text = tr("Superscript");
        checkable = true;
        break;
    case InsertUnorderedList:
        text = tr("Insert Bulleted List");
        checkable = true;
        break;
    case InsertOrderedList:
        text = tr("Insert Numbered List");
        checkable = true;
        break;
    case Indent:
        text = tr("Indent");
        break;
    case Outdent:
        text = tr("Outdent");
        break;
    case AlignCenter:
        text = tr("Center");
        break;
    case AlignJustified:
        text = tr("Justify");
        break;
    case AlignLeft:
        text = tr("Align Left");
        break;
    case AlignRight:
        text = tr("Align Right");
        break;
    case NoWebAction:
        return 0;
    default:
        break;
    }

    if (text.isEmpty())
        return 0;

    QAction *a = new QAction();
    a->setText(text);
    a->setData(action);
    a->setCheckable(checkable);
    a->setIcon(icon);

//    connect(a, SIGNAL(triggered(bool)),
//        this, SLOT(_q_webActionTriggered(bool)));

//    d->actions[action] = a;
//    d->updateAction(action);
    return a;
}

QAction* QUltralightPage::customAction(int action) const
{
//    auto actionIter = d->customActions.constFind(action);
//    if (actionIter != d->customActions.constEnd())
//        return *actionIter;

    QAction* a = new QAction();
    a->setData(action);
    connect(a, SIGNAL(triggered(bool)),
        this, SLOT(_q_customActionTriggered(bool)));

//    d->customActions.insert(action, a);
    return a;
}
#endif // QT_NO_ACTION

bool QUltralightPage::isModified() const
{
#ifdef QT_NO_UNDOSTACK
    return false;
#else
    if (!_undoStack)
        return false;
    return _undoStack->canUndo();
#endif // QT_NO_UNDOSTACK
    return false;
}

#ifndef QT_NO_UNDOSTACK
QUndoStack *QUltralightPage::undoStack() const
{
    if (!_undoStack) {
        QUltralightPage *that = const_cast<QUltralightPage *>(this);
        that->_undoStack = new QUndoStack(that);
    }
    return _undoStack;
}
#endif // QT_NO_UNDOSTACK

bool QUltralightPage::event(QEvent *ev)
{
//    switch (ev->type()) {
//    case QEvent::Timer:
//        d->timerEvent(static_cast<QTimerEvent*>(ev));
//        break;
//    case QEvent::MouseMove:
//        d->mouseMoveEvent(static_cast<QMouseEvent*>(ev));
//        break;
//    case QEvent::MouseButtonPress:
//        d->mousePressEvent(static_cast<QMouseEvent*>(ev));
//        break;
//    case QEvent::MouseButtonDblClick:
//        d->mouseDoubleClickEvent(static_cast<QMouseEvent*>(ev));
//        break;
//    case QEvent::MouseButtonRelease:
//        d->mouseReleaseEvent(static_cast<QMouseEvent*>(ev));
//        break;
//#if !defined(QT_NO_GRAPHICSVIEW)
//    case QEvent::GraphicsSceneMouseMove: {
//        QGraphicsSceneMouseEvent *gsEv = static_cast<QGraphicsSceneMouseEvent*>(ev);
//        QMouseEvent dummyEvent(QEvent::MouseMove, gsEv->pos(), gsEv->screenPos(), gsEv->button(), gsEv->buttons(), gsEv->modifiers());
//        d->mouseMoveEvent(&dummyEvent);
//        ev->setAccepted(dummyEvent.isAccepted());
//        break;
//    }
//    case QEvent::GraphicsSceneMouseRelease: {
//        QGraphicsSceneMouseEvent *gsEv = static_cast<QGraphicsSceneMouseEvent*>(ev);
//        QMouseEvent dummyEvent(QEvent::MouseButtonRelease, gsEv->pos(), gsEv->screenPos(), gsEv->button(), gsEv->buttons(), gsEv->modifiers());
//        d->adjustPointForClicking(&dummyEvent);
//        d->mouseReleaseEvent(&dummyEvent);
//        ev->setAccepted(dummyEvent.isAccepted());
//        break;
//    }
//    case QEvent::GraphicsSceneMousePress: {
//        QGraphicsSceneMouseEvent *gsEv = static_cast<QGraphicsSceneMouseEvent*>(ev);
//        QMouseEvent dummyEvent(QEvent::MouseButtonPress, gsEv->pos(), gsEv->screenPos(), gsEv->button(), gsEv->buttons(), gsEv->modifiers());
//        d->adjustPointForClicking(&dummyEvent);
//        d->mousePressEvent(&dummyEvent);
//        ev->setAccepted(dummyEvent.isAccepted());
//        break;
//    }
//    case QEvent::GraphicsSceneMouseDoubleClick: {
//        QGraphicsSceneMouseEvent *gsEv = static_cast<QGraphicsSceneMouseEvent*>(ev);
//        QMouseEvent dummyEvent(QEvent::MouseButtonDblClick, gsEv->pos(), gsEv->screenPos(), gsEv->button(), gsEv->buttons(), gsEv->modifiers());
//        d->adjustPointForClicking(&dummyEvent);
//        d->mouseDoubleClickEvent(&dummyEvent);
//        ev->setAccepted(dummyEvent.isAccepted());
//        break;
//    }
//#endif
//#ifndef QT_NO_CONTEXTMENU
//    case QEvent::ContextMenu:
//        d->contextMenuEvent(static_cast<QContextMenuEvent*>(ev)->globalPos());
//        break;
//#if !defined(QT_NO_GRAPHICSVIEW)
//    case QEvent::GraphicsSceneContextMenu:
//        d->contextMenuEvent(static_cast<QGraphicsSceneContextMenuEvent*>(ev)->screenPos());
//        break;
//#endif
//#endif
//#ifndef QT_NO_WHEELEVENT
//    case QEvent::Wheel:
//        d->wheelEvent(static_cast<QWheelEvent*>(ev), QApplication::wheelScrollLines());
//        break;
//#if !defined(QT_NO_GRAPHICSVIEW)
//    case QEvent::GraphicsSceneWheel: {
//        QGraphicsSceneWheelEvent *gsEv = static_cast<QGraphicsSceneWheelEvent*>(ev);
//        QWheelEvent dummyEvent(gsEv->pos(), gsEv->screenPos(), gsEv->delta(), gsEv->buttons(), gsEv->modifiers(), gsEv->orientation());
//        d->wheelEvent(&dummyEvent, QApplication::wheelScrollLines());
//        ev->setAccepted(dummyEvent.isAccepted());
//        break;
//    }
//#endif
//#endif
//    case QEvent::KeyPress:
//        qobject_cast<QUltralightView*>(view())->sendKey(static_cast<QKeyEvent*>(ev));
//        break;
//    case QEvent::KeyRelease:
//        d->keyReleaseEvent(static_cast<QKeyEvent*>(ev));
//        break;
//    case QEvent::FocusIn:
//        d->focusInEvent(static_cast<QFocusEvent*>(ev));
//        break;
//    case QEvent::FocusOut:
//        d->focusOutEvent(static_cast<QFocusEvent*>(ev));
//        break;
//#if ENABLE(DRAG_SUPPORT)
//    case QEvent::DragEnter:
//        d->dragEnterEvent(static_cast<QDragEnterEvent*>(ev));
//        break;
//    case QEvent::DragLeave:
//        d->dragLeaveEvent();
//        ev->accept();
//        break;
//    case QEvent::DragMove:
//        d->dragMoveEvent(static_cast<QDragMoveEvent*>(ev));
//        break;
//    case QEvent::Drop:
//        d->dropEvent(static_cast<QDropEvent*>(ev));
//        break;
//#if !defined(QT_NO_GRAPHICSVIEW)
//    case QEvent::GraphicsSceneDragEnter:
//        d->dragEnterEvent(static_cast<QGraphicsSceneDragDropEvent*>(ev));
//        break;
//    case QEvent::GraphicsSceneDragMove:
//        d->dragMoveEvent(static_cast<QGraphicsSceneDragDropEvent*>(ev));
//        break;
//    case QEvent::GraphicsSceneDragLeave:
//        d->dragLeaveEvent();
//        ev->accept();
//        break;
//    case QEvent::GraphicsSceneDrop:
//        d->dropEvent(static_cast<QGraphicsSceneDragDropEvent*>(ev));
//        break;
//#endif

//#endif
//    case QEvent::InputMethod:
//        d->inputMethodEvent(static_cast<QInputMethodEvent*>(ev));
//        break;
//    case QEvent::ShortcutOverride:
//        d->shortcutOverrideEvent(static_cast<QKeyEvent*>(ev));
//        break;
//    case QEvent::Leave:
//        d->leaveEvent(ev);
//        break;
//    case QEvent::TouchBegin:
//    case QEvent::TouchUpdate:
//    case QEvent::TouchEnd:
//    case QEvent::TouchCancel:
//        // Return whether the default action was cancelled in the JS event handler
//        return d->touchEvent(static_cast<QTouchEvent*>(ev));
//#ifndef QT_NO_GESTURES
//    case QEvent::Gesture:
//        d->gestureEvent(static_cast<QGestureEvent*>(ev));
//        break;
//#endif
//#ifndef QT_NO_PROPERTIES
//    case QEvent::DynamicPropertyChange:
//        d->dynamicPropertyChangeEvent(this, static_cast<QDynamicPropertyChangeEvent*>(ev));
//        break;
//#endif
//    case QEvent::Show:
//        d->setPluginsVisible(true);
//        break;
//    case QEvent::Hide:
//        d->setPluginsVisible(false);
//        break;
//    default:
//        return QObject::event(ev);
//    }
    return true;
}

bool QUltralightPage::focusNextPrevChild(bool next)
{
    QKeyEvent ev(QEvent::KeyPress, Qt::Key_Tab, Qt::KeyboardModifiers(next ? Qt::NoModifier : Qt::ShiftModifier));
//    qobject_cast<QUltralightView*>(view())->sendKey(&ev);
    return true;
}

QVariant QUltralightPage::inputMethodQuery(Qt::InputMethodQuery property) const
{
    return QVariant();
}

void QUltralightPage::setContentEditable(bool editable)
{
//    if (isContentEditable() != editable) {
//        d->setContentEditable(editable);
//        d->updateEditorActions();
//    }
}

bool QUltralightPage::isContentEditable() const
{
//    return d->isContentEditable();
    return false;
}

void QUltralightPage::setForwardUnsupportedContent(bool forward)
{
//    d->forwardUnsupportedContent = forward;
}

bool QUltralightPage::forwardUnsupportedContent() const
{
//    return d->forwardUnsupportedContent;
    return false;
}

void QUltralightPage::setLinkDelegationPolicy(LinkDelegationPolicy policy)
{
//    d->linkPolicy = policy;
}

QUltralightPage::LinkDelegationPolicy QUltralightPage::linkDelegationPolicy() const
{
//    return d->linkPolicy;
    return LinkDelegationPolicy();
}

void QUltralightPage::setPalette(const QPalette &pal)
{
//    d->palette = pal;
//    if (!d->mainFrame || !d->mainFrame.data()->d->hasView())
//        return;

//    QBrush brush = pal.brush(QPalette::Base);
//    QColor backgroundColor = brush.style() == Qt::SolidPattern ? brush.color() : QColor();
//    d->mainFrame.data()->d->updateBackgroundRecursively(backgroundColor);
}

QPalette QUltralightPage::palette() const
{
    return QPalette();
}

#ifndef QT_NO_CONTEXTMENU
bool QUltralightPage::swallowContextMenuEvent(QContextMenuEvent *event)
{
//    QUltralightFrame* webFrame = frameAt(event->pos());
//    return d->swallowContextMenuEvent(event, webFrame ? webFrame->d : 0);
    return false;
}
#endif // QT_NO_CONTEXTMENU

void QUltralightPage::updatePositionDependentActions(const QPoint &pos)
{
#ifndef QT_NO_ACTION
    // First we disable all actions, but keep track of which ones were originally enabled.
    QBitArray originallyEnabledWebActions(QUltralightPage::WebActionCount);
    for (int i = (int) MenuAction::NoAction + 1; i < (int) MenuAction::ActionCount; ++i) {
        QUltralightPage::WebAction action = webActionForAdapterMenuAction(MenuAction(i));
        if (QAction *a = this->action(action)) {
            originallyEnabledWebActions.setBit(action, a->isEnabled());
            a->setEnabled(false);
        }
    }
#endif // QT_NO_ACTION

    QBitArray visitedWebActions(QUltralightPage::WebActionCount);
//    d->createMainFrame();
//    // Then we let updatePositionDependantMenuActions() enable the actions that are put into the menu
//    QUltralightHitTestResult* result = d->updatePositionDependentMenuActions(pos, &visitedWebActions);
//    if (!result)
//        _hitTestResult = QUltralightHitTestResult();
//    else
//        _hitTestResult = QUltralightHitTestResult(result);

#ifndef QT_NO_ACTION
    // Finally, we restore the original enablement for the actions that were not put into the menu.
    originallyEnabledWebActions &= ~visitedWebActions; // Mask out visited actions (they're part of the menu)
    for (int i = 0; i < QUltralightPage::WebActionCount; ++i) {
        if (originallyEnabledWebActions.at(i)) {
            if (QAction *a = this->action(QUltralightPage::WebAction(i)))
                a->setEnabled(true);
        }
    }
#endif // QT_NO_ACTION

    // This whole process ensures that any actions put into to the context menu has the right
    // enablement, while also keeping the correct enablement for actions that were left out of
    // the menu.
}

bool QUltralightPage::extension(Extension extension, const ExtensionOption *option, ExtensionReturn *output)
{
#ifndef QT_NO_FILEDIALOG
    if (extension == ChooseMultipleFilesExtension) {
        // FIXME: do not ignore suggestedFiles
        QStringList suggestedFiles = static_cast<const ChooseMultipleFilesExtensionOption*>(option)->suggestedFileNames;
        QStringList names = QFileDialog::getOpenFileNames(view(), QString());
        static_cast<ChooseMultipleFilesExtensionReturn*>(output)->fileNames = names;
        return true;
    }
#endif

    if (extension == ErrorPageExtension) {
        auto* errorOption = static_cast<const ErrorPageExtensionOption*>(option);

        QString errorCode;
        switch (errorOption->domain) {
        case QUltralightPage::Http:
            errorCode = tr("HTTP Error %0").arg(errorOption->error);
            break;
        case QUltralightPage::QtNetwork:
            errorCode = tr("QtNetwork Error %0").arg(errorOption->error);
            break;
        case QUltralightPage::WebKit:
            errorCode = tr("WebKit Error %0").arg(errorOption->error);
            break;
        }

        QString pageHeader = errorOption->errorString;
        if (pageHeader.isEmpty())
            pageHeader = errorCode;
        else if (pageHeader.endsWith(QLatin1Char('.')))
            pageHeader.chop(1);

        auto* pageOutput = static_cast<ErrorPageExtensionReturn*>(output);
        pageOutput->baseUrl = errorOption->url;
        QString escapedUrl = errorOption->url.toDisplayString().toHtmlEscaped();
        pageOutput->content = QStringLiteral("<html><head>"
            "<meta charset=\"utf-8\">"
            "<title>%0</title>"
            "<style>"
            "html{font-family:sans;background:#EEE;color:#000;}"
            "body{max-width:600px;margin:150px auto 0;padding:10px;}"
            "pre{text-align:right;color:#999;}"
            "</style>"
            "</head><body>"
            "<h1>%0</h1><hr>"
            "<p>%1</p><pre>%2</pre>"
            "</body></html>").arg(
                pageHeader.toHtmlEscaped(),
                tr("Failed to load URL %0.").toHtmlEscaped().arg(QLatin1String("<a href=\"") + escapedUrl + QLatin1String("\">") + escapedUrl + QLatin1String("</a>")),
                errorCode.toHtmlEscaped()).toUtf8();

        return true;
    }

    return false;
}

bool QUltralightPage::supportsExtension(Extension extension) const
{
#ifndef QT_NO_FILEDIALOG
    if (extension == ChooseMultipleFilesExtension)
        return true;
#endif
    return extension == ErrorPageExtension && settings()->testAttribute(QUltralightSettings::ErrorPageEnabled);
}


//QUltralightPageAdapter *QUltralightPage::handle() const
//{
//    return d;
//}

bool QUltralightPage::findText(const QString &subString, FindFlags options)
{
//    return d->findText(subString, static_cast<QUltralightPageAdapter::FindFlag>(
//        static_cast<FindFlags::Int>(options)));
    return false;
}

QUltralightSettings *QUltralightPage::settings() const
{
    return _settings;
}

QString QUltralightPage::chooseFile(QUltralightFrame *parentFrame, const QString& suggestedFile)
{
    Q_UNUSED(parentFrame);
#ifndef QT_NO_FILEDIALOG
    return QFileDialog::getOpenFileName(view(), QString(), suggestedFile);
#else
    return QString();
#endif
}

void QUltralightPage::setNetworkAccessManager(QNetworkAccessManager *manager)
{
//    d->setNetworkAccessManager(manager);
}

QNetworkAccessManager *QUltralightPage::networkAccessManager() const
{
//    return d->networkAccessManager();
    return new QNetworkAccessManager();
}

void QUltralightPage::setPluginFactory(QWebPluginFactory *factory)
{
//    d->pluginFactory = factory;
}

//QUltralightPluginFactory *QUltralightPage::pluginFactory() const
//{
//    return d->pluginFactory;
//}

QString QUltralightPage::userAgentForUrl(const QUrl&) const
{
    return QString();
}

quint64 QUltralightPage::totalBytes() const
{
//    return d->m_totalBytes;
    return 0;
}

quint64 QUltralightPage::bytesReceived() const
{
//    return d->m_bytesReceived;
    return 0;
}

void QUltralightPage::setVisibilityState(VisibilityState state)
{
//    d->setVisibilityState(static_cast<QUltralightPageAdapter::VisibilityState>(state));
}

QUltralightPage::VisibilityState QUltralightPage::visibilityState() const
{
//    return static_cast<VisibilityState>(d->visibilityState());
    return VisibilityState();
}


bool QUltralightPage::recentlyAudible() const
{
//    return d->isPlayingAudio();
    return false;
}
