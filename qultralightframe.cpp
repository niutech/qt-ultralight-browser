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

#include "qultralightframe.h"
#include "qultralightpage.h"

#include <QFileInfo>

using namespace WebCore;

QT_BEGIN_NAMESPACE
extern Q_GUI_EXPORT int qt_defaultDpi();
QT_END_NAMESPACE

QUltralightFrame::QUltralightFrame(QUltralightPage *parentPage) : QObject(parentPage)
{
    _page = parentPage;
    _view = parentPage->_overlay->view();
}

QUltralightFrame::QUltralightFrame(QUltralightFrame* parent, QUltralightFrameData* frameData) : QObject(parent)
{
    _page = parent->_page;
}

QUltralightFrame::~QUltralightFrame()
{
    delete _page;
}

void QUltralightFrame::addToJavaScriptWindowObject(const QString &name, QObject *object, ValueOwnership ownership)
{
//    d->addToJavaScriptWindowObject(name, object, static_cast<QUltralightFrameAdapter::ValueOwnership>(ownership));
}


QString QUltralightFrame::toHtml() const
{
//    return d->toHtml();
    return QString();
}

QString QUltralightFrame::toPlainText() const
{
//    return d->toPlainText();
    return QString();
}

QString QUltralightFrame::title() const
{
    return ulStringToQString(_view->title());
}

QMultiMap<QString, QString> QUltralightFrame::metaData() const
{
//    return d->metaData();
    return QMultiMap<QString, QString>();
}

void QUltralightFrame::setUrl(const QUrl &url)
{
    const QUrl absolute = ensureAbsoluteUrl(url);
    load(absolute);
}

QUrl QUltralightFrame::url() const
{
    return ulStringToQUrl(_view->url());
}

QUrl QUltralightFrame::requestedUrl() const
{
    return url();
}

QUrl QUltralightFrame::baseUrl() const
{
    return url().resolved(QUrl("./"));
}

QIcon QUltralightFrame::icon() const
{
//    return QUltralightSettings::iconForUrl(d->coreFrameUrl());
    return QIcon();
}

QString QUltralightFrame::frameName() const
{
//    return d->uniqueName();
    return QString();
}

QUltralightPage *QUltralightFrame::page() const
{
    return _page;
}

void QUltralightFrame::load(const QUrl &url)
{
    // The load() overload ensures that the url is absolute.
    load(QNetworkRequest(url));
}

void QUltralightFrame::load(const QNetworkRequest &req, QNetworkAccessManager::Operation operation, const QByteArray &body)
{
    _view->LoadURL(QUrlToUlString(req.url()));
}

void QUltralightFrame::setHtml(const QString &html, const QUrl &baseUrl)
{
    _view->LoadHTML(QStringToUlString(html), QUrlToUlString(baseUrl));
}

void QUltralightFrame::setContent(const QByteArray &data, const QString &mimeType, const QUrl &baseUrl)
{
//    d->setContent(data, mimeType, baseUrl);
}

QUltralightFrame *QUltralightFrame::parentFrame() const
{
//    return d->parentFrame();
    return const_cast<QUltralightFrame*>(this);
}

QList<QUltralightFrame*> QUltralightFrame::childFrames() const
{
//    QList<QObject*> objects = d->childFrames();
//    QList<QUltralightFrame*> rc;
//    rc.reserve(objects.size());
//    Q_FOREACH(QObject* object, objects) {
//        if (QUltralightFrame* frame = qobject_cast<QUltralightFrame*>(object))
//            rc.append(frame);
//    }

//    return rc;
    return QList<QUltralightFrame*>();
}

Qt::ScrollBarPolicy QUltralightFrame::scrollBarPolicy(Qt::Orientation orientation) const
{
//    if (orientation == Qt::Horizontal)
//        return d->horizontalScrollBarPolicy;
//    return d->verticalScrollBarPolicy;
    return Qt::ScrollBarPolicy();
}

void QUltralightFrame::setScrollBarPolicy(Qt::Orientation orientation, Qt::ScrollBarPolicy policy)
{
//    d->setScrollBarPolicy(orientation, policy);
}

void QUltralightFrame::setScrollBarValue(Qt::Orientation orientation, int value)
{
//    d->setScrollBarValue(orientation, value);
}

int QUltralightFrame::scrollBarValue(Qt::Orientation orientation) const
{
//    return d->scrollBarValue(orientation);
    return 0;
}

int QUltralightFrame::scrollBarMaximum(Qt::Orientation orientation) const
{
//    return d->scrollBarMaximum(orientation);
    return 0;
}

int QUltralightFrame::scrollBarMinimum(Qt::Orientation orientation) const
{
    Q_UNUSED(orientation);
    return 0;
}

QRect QUltralightFrame::scrollBarGeometry(Qt::Orientation orientation) const
{
//    return d->scrollBarGeometry(orientation);
    return QRect();
}

void QUltralightFrame::scroll(int dx, int dy)
{
//    d->scrollBy(dx, dy);
}

QPoint QUltralightFrame::scrollPosition() const
{
//    return d->scrollPosition();
    return QPoint();
}

void QUltralightFrame::setScrollPosition(const QPoint &pos)
{
//    QPoint current = scrollPosition();
//    int dx = pos.x() - current.x();
//    int dy = pos.y() - current.y();
//    scroll(dx, dy);
}

void QUltralightFrame::scrollToAnchor(const QString& anchor)
{
//    d->scrollToAnchor(anchor);
}

void QUltralightFrame::render(QPainter* painter, RenderLayers layer, const QRegion& clip)
{
//    if (!clip.isEmpty())
//        d->renderRelativeCoords(painter, layer, clip);
//    else if (d->hasView())
//        d->renderRelativeCoords(painter, layer, QRegion(d->frameRect()));
}

void QUltralightFrame::render(QPainter* painter, const QRegion& clip)
{
    render(painter, AllLayers, clip);
}

void QUltralightFrame::setTextSizeMultiplier(qreal factor)
{
//    d->setTextSizeMultiplier(factor);
}

qreal QUltralightFrame::textSizeMultiplier() const
{
    return 1.0;
}

void QUltralightFrame::setZoomFactor(qreal factor)
{
    //
}

qreal QUltralightFrame::zoomFactor() const
{
    return 1.0;
}

bool QUltralightFrame::hasFocus() const
{
    return _view->HasFocus();
}

void QUltralightFrame::setFocus()
{
    _view->Focus();
}

QPoint QUltralightFrame::pos() const
{
//    if (!d->hasView())
//        return QPoint();
//    return d->frameRect().topLeft();
    return QPoint();
}

QRect QUltralightFrame::geometry() const
{
//    return d->frameRect();
    return QRect();
}

QSize QUltralightFrame::contentsSize() const
{
//    return d->contentsSize();
    return QSize(_view->surface()->width(), _view->surface()->height());
}

//QWebElement QUltralightFrame::documentElement() const
//{
//    return d->documentElement();
//}

//QWebElement QUltralightFrame::ownerElement() const
//{
//    return d->ownerElement();
//}

//QWebElementCollection QUltralightFrame::findAllElements(const QString &selectorQuery) const
//{
//    return documentElement().findAll(selectorQuery);
//}

//QWebElement QUltralightFrame::findFirstElement(const QString &selectorQuery) const
//{
//    return documentElement().findFirst(selectorQuery);
//}

QUltralightHitTestResult QUltralightFrame::hitTestContent(const QPoint &pos) const
{
    return QUltralightHitTestResult();
}

bool QUltralightFrame::event(QEvent *e)
{
    return QObject::event(e);
}

#ifndef QT_NO_PRINTER
void QUltralightFrame::print(QPrinter *printer) const
{
//#if HAVE(QTPRINTSUPPORT)
//    QPainter painter;
//    if (!painter.begin(printer))
//        return;

//    const qreal zoomFactorX = (qreal)printer->logicalDpiX() / qt_defaultDpi();
//    const qreal zoomFactorY = (qreal)printer->logicalDpiY() / qt_defaultDpi();

//    QRect qprinterRect = printer->pageRect();

//    QRect pageRect(0, 0, int(qprinterRect.width() / zoomFactorX), int(qprinterRect.height() / zoomFactorY));

//    QtPrintContext printContext(&painter, pageRect, d);

//    int docCopies;
//    int pageCopies;
//    if (printer->collateCopies()) {
//        docCopies = 1;
//        pageCopies = printer->numCopies();
//    } else {
//        docCopies = printer->numCopies();
//        pageCopies = 1;
//    }

//    int fromPage = printer->fromPage();
//    int toPage = printer->toPage();
//    bool ascending = true;

//    if (!fromPage && !toPage) {
//        fromPage = 1;
//        toPage = printContext.pageCount();
//    }
//    // paranoia check
//    fromPage = qMax(1, fromPage);
//    toPage = qMin(static_cast<int>(printContext.pageCount()), toPage);
//    if (toPage < fromPage) {
//        // if the user entered a page range outside the actual number
//        // of printable pages, just return
//        return;
//    }

//    if (printer->pageOrder() == QPrinter::LastPageFirst) {
//        int tmp = fromPage;
//        fromPage = toPage;
//        toPage = tmp;
//        ascending = false;
//    }

//    painter.scale(zoomFactorX, zoomFactorY);

//    for (int i = 0; i < docCopies; ++i) {
//        int page = fromPage;
//        while (true) {
//            for (int j = 0; j < pageCopies; ++j) {
//                if (printer->printerState() == QPrinter::Aborted
//                    || printer->printerState() == QPrinter::Error) {
//                    return;
//                }
//                printContext.spoolPage(page - 1, pageRect.width());
//                if (j < pageCopies - 1)
//                    printer->newPage();
//            }

//            if (page == toPage)
//                break;

//            if (ascending)
//                ++page;
//            else
//                --page;

//            printer->newPage();
//        }

//        if (i < docCopies - 1)
//            printer->newPage();
//    }
//#endif // HAVE(PRINTSUPPORT)
}
#endif // QT_NO_PRINTER

QVariant QUltralightFrame::evaluateJavaScript(const QString& scriptSource)
{
    return ulStringToQString(_view->EvaluateScript(QStringToUlString(scriptSource)));
}

//QUltralightSecurityOrigin QUltralightFrame::securityOrigin() const
//{
//    return d->securityOrigin();
//}

//QUltralightFrameAdapter *QUltralightFrame::handle() const
//{
//    return d;
//}

//ultralight::RefPtr<ultralight::View> QUltralightFrame::ulView() const
//{
//    return _view;
//}

QString QUltralightFrame::ulStringToQString(const ultralight::String string) const
{
    return QString::fromUtf8(string.utf8().data());
}

QUrl QUltralightFrame::ulStringToQUrl(const ultralight::String string) const
{
    return QUrl(ulStringToQString(string));
}

ultralight::String QUltralightFrame::QStringToUlString(const QString string) const
{
    return (ultralight::String8) string.toUtf8().constData();
}

ultralight::String QUltralightFrame::QUrlToUlString(const QUrl url) const
{
    return QStringToUlString(url.toString());
}

QUrl QUltralightFrame::ensureAbsoluteUrl(const QUrl& url)
{
    if (!url.isValid() || !url.isRelative())
        return url;

    // This contains the URL with absolute path but without
    // the query and the fragment part.
    QUrl baseUrl = QUrl::fromLocalFile(QFileInfo(url.toLocalFile()).absoluteFilePath());

    // The path is removed so the query and the fragment parts are there.
    QString pathRemoved = url.toString(QUrl::RemovePath);
    QUrl toResolve(pathRemoved);

    return baseUrl.resolved(toResolve);
}


QUltralightHitTestResult::QUltralightHitTestResult()
{
}

QUltralightHitTestResult::QUltralightHitTestResult(const QUltralightHitTestResult &other)
{
}

QUltralightHitTestResult &QUltralightHitTestResult::operator=(const QUltralightHitTestResult &other)
{
    if (this != &other) {
        _pos = other.pos();
    }
    return *this;
}

QUltralightHitTestResult::~QUltralightHitTestResult()
{
}

bool QUltralightHitTestResult::isNull() const
{
    return _pos.isNull();
}

QPoint QUltralightHitTestResult::pos() const
{
    return _pos;
}

QRect QUltralightHitTestResult::boundingRect() const
{
    return _boundingRect;
}

//QWebElement QUltralightHitTestResult::enclosingBlockElement() const
//{
//    return _enclosingBlock;
//}

QString QUltralightHitTestResult::title() const
{
    return QString();
}

QString QUltralightHitTestResult::linkText() const
{
    return QString();
}

QUrl QUltralightHitTestResult::linkUrl() const
{
    return QUrl();
}

#if QT_DEPRECATED_SINCE(5,5)
QUrl QUltralightHitTestResult::linkTitle() const
{
    return QUrl();
}
#endif // QT_DEPRECATED_SINCE(5,5)

QString QUltralightHitTestResult::linkTitleString() const
{
    return QString();
}

//QWebElement QUltralightHitTestResult::linkElement() const
//{
//    return QWebElement();
//}

QUltralightFrame *QUltralightHitTestResult::linkTargetFrame() const
{
    return 0;
}

QString QUltralightHitTestResult::alternateText() const
{
    return QString();
}

QUrl QUltralightHitTestResult::imageUrl() const
{
    return QUrl();
}

QUrl QUltralightHitTestResult::mediaUrl() const
{
    return QUrl();
}

QPixmap QUltralightHitTestResult::pixmap() const
{
    return QPixmap();
}

bool QUltralightHitTestResult::isContentEditable() const
{
    return false;
}

bool QUltralightHitTestResult::isContentSelected() const
{
    return false;
}

//QWebElement QUltralightHitTestResult::element() const
//{
//    return QWebElement();
//}

QUltralightFrame *QUltralightHitTestResult::frame() const
{
    return 0;
}
