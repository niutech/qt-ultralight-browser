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

#ifndef QULTRALIGHTFRAME_H
#define QULTRALIGHTFRAME_H

#include <QtCore/qobject.h>
#include <QtCore/qurl.h>
#include <QtCore/qvariant.h>
#include <QtGui/qicon.h>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <AppCore/AppCore.h>

QT_BEGIN_NAMESPACE
class QRect;
class QPoint;
class QPainter;
class QPixmap;
class QMouseEvent;
class QWheelEvent;
class QNetworkRequest;
class QRegion;
class QPrinter;
QT_END_NAMESPACE

class QWebNetworkRequest;
class QUltralightFrameAdapter;
//class QUltralightFramePrivate;
class QUltralightPage;
class QUltralightPageAdapter;
//class QUltralightHitTestResult;
class QWebHistoryItem;
class QWebSecurityOrigin;
class QWebElement;
class QWebElementCollection;
class QWebScriptWorld;

class DumpRenderTreeSupportQt;
namespace WebCore {
    class WidgetPrivate;
    class FrameLoaderClientQt;
    class ChromeClientQt;
    class TextureMapperLayerClientQt;
}
namespace WebKit {
    class InspectorClientWebPage;
}
class QUltralightFrameData;
//class QUltralightHitTestResultPrivate;
class QUltralightFrame;

class QUltralightHitTestResult {
public:
    QUltralightHitTestResult();
    QUltralightHitTestResult(const QUltralightHitTestResult &other);
    QUltralightHitTestResult &operator=(const QUltralightHitTestResult &other);
    ~QUltralightHitTestResult();

    bool isNull() const;

    QPoint pos() const;
    QRect boundingRect() const;
    //QWebElement enclosingBlockElement() const;
    QString title() const;

    QString linkText() const;
    QUrl linkUrl() const;
#if QT_DEPRECATED_SINCE(5,5)
    QT_DEPRECATED QUrl linkTitle() const;
#endif // QT_DEPRECATED_SINCE(5,5)
    QString linkTitleString() const;
    QUltralightFrame *linkTargetFrame() const;
    //QWebElement linkElement() const;

    QString alternateText() const; // for img, area, input and applet

    QUrl imageUrl() const;
    QPixmap pixmap() const;
    QUrl mediaUrl() const;

    bool isContentEditable() const;
    bool isContentSelected() const;

    //QWebElement element() const;

    QUltralightFrame *frame() const;

private:
    friend class QUltralightFrame;
    //friend class QUltralightPagePrivate;
    friend class QUltralightPage;

    QPoint _pos;
    QRect _boundingRect;
};

class QUltralightFrame : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal textSizeMultiplier READ textSizeMultiplier WRITE setTextSizeMultiplier DESIGNABLE false)
    Q_PROPERTY(qreal zoomFactor READ zoomFactor WRITE setZoomFactor)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QUrl requestedUrl READ requestedUrl)
    Q_PROPERTY(QUrl baseUrl READ baseUrl)
    Q_PROPERTY(QIcon icon READ icon)
    Q_PROPERTY(QSize contentsSize READ contentsSize)
    Q_PROPERTY(QPoint scrollPosition READ scrollPosition WRITE setScrollPosition)
    Q_PROPERTY(bool focus READ hasFocus)
private:
    QUltralightFrame(QUltralightPage *parentPage);
    QUltralightFrame(QUltralightFrame* parent, QUltralightFrameData* frameData);
    ~QUltralightFrame();

public:
    enum ValueOwnership {
        QtOwnership,
        ScriptOwnership,
        AutoOwnership
    };

    QUltralightPage *page() const;

    void load(const QUrl &url);
    void load(const QNetworkRequest &request, QNetworkAccessManager::Operation operation = QNetworkAccessManager::GetOperation, const QByteArray &body = QByteArray());
    void setHtml(const QString &html, const QUrl &baseUrl = QUrl());
    void setContent(const QByteArray &data, const QString &mimeType = QString(), const QUrl &baseUrl = QUrl());

    void addToJavaScriptWindowObject(const QString &name, QObject *object, ValueOwnership ownership = QtOwnership);
    QString toHtml() const;
    QString toPlainText() const;

    QString title() const;
    void setUrl(const QUrl &url);
    QUrl url() const;
    QUrl requestedUrl() const;
    QUrl baseUrl() const;
    QIcon icon() const;
    QMultiMap<QString, QString> metaData() const;

    QString frameName() const;

    QUltralightFrame *parentFrame() const;
    QList<QUltralightFrame*> childFrames() const;

    Qt::ScrollBarPolicy scrollBarPolicy(Qt::Orientation orientation) const;
    void setScrollBarPolicy(Qt::Orientation orientation, Qt::ScrollBarPolicy policy);

    void setScrollBarValue(Qt::Orientation orientation, int value);
    int scrollBarValue(Qt::Orientation orientation) const;
    int scrollBarMinimum(Qt::Orientation orientation) const;
    int scrollBarMaximum(Qt::Orientation orientation) const;
    QRect scrollBarGeometry(Qt::Orientation orientation) const;

    void scroll(int, int);
    QPoint scrollPosition() const;
    void setScrollPosition(const QPoint &pos);

    void scrollToAnchor(const QString& anchor);

    enum RenderLayer {
        ContentsLayer = 0x10,
        ScrollBarLayer = 0x20,
        PanIconLayer = 0x40,

        AllLayers = 0xff
    };
    Q_DECLARE_FLAGS(RenderLayers, RenderLayer)

    void render(QPainter*, const QRegion& clip = QRegion());
    void render(QPainter*, RenderLayers layer, const QRegion& clip = QRegion());

    void setTextSizeMultiplier(qreal factor);
    qreal textSizeMultiplier() const;

    qreal zoomFactor() const;
    void setZoomFactor(qreal factor);

    bool hasFocus() const;
    void setFocus();

    QPoint pos() const;
    QRect geometry() const;
    QSize contentsSize() const;

    QWebElement documentElement() const;
    QWebElement ownerElement() const;
    QWebElementCollection findAllElements(const QString &selectorQuery) const;
    QWebElement findFirstElement(const QString &selectorQuery) const;

    QUltralightHitTestResult hitTestContent(const QPoint &pos) const;

    bool event(QEvent *) Q_DECL_OVERRIDE;

    QWebSecurityOrigin securityOrigin() const;
    //QUltralightFrameAdapter* handle() const;

    //Ultralight
    //ultralight::RefPtr<ultralight::View> ulView() const;

public Q_SLOTS:
    QVariant evaluateJavaScript(const QString& scriptSource);
#ifndef QT_NO_PRINTER
    void print(QPrinter *printer) const;
#endif

Q_SIGNALS:
    void javaScriptWindowObjectCleared();

    void provisionalLoad();
    void titleChanged(const QString &title);
    void urlChanged(const QUrl &url);

    void initialLayoutCompleted();

    void iconChanged();

    void contentsSizeChanged(const QSize &size);

    void loadStarted();
    void loadFinished(bool ok);

    void pageChanged();

private:
    //friend class QGraphicsWebView;
    friend class QUltralightPage;
    //friend class QUltralightPagePrivate;
    //friend class QUltralightFramePrivate;
    friend class DumpRenderTreeSupportQt;
    friend class WebCore::WidgetPrivate;
    friend class WebCore::FrameLoaderClientQt;
    friend class WebCore::ChromeClientQt;
    friend class WebCore::TextureMapperLayerClientQt;
    friend class WebKit::InspectorClientWebPage;
    //QUltralightFramePrivate *d;
    //Q_PRIVATE_SLOT(d, void _q_orientationChanged())

    QUltralightPage *_page;

    // Ultralight
    ultralight::RefPtr<ultralight::View> _view;
    QString ulStringToQString(const ultralight::String string) const;
    QUrl ulStringToQUrl(const ultralight::String string) const;
    ultralight::String QStringToUlString(const QString string) const;
    ultralight::String QUrlToUlString(const QUrl url) const;
    QUrl ensureAbsoluteUrl(const QUrl&);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(QUltralightFrame::RenderLayers)

#endif // QULTRALIGHTFRAME_H
