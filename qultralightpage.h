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

#ifndef QULTRALIGHTPAGE_H
#define QULTRALIGHTPAGE_H

#include <QtCore/qobject.h>
#include <QtCore/qurl.h>
#include <QtWidgets/qwidget.h>
#include "qultralightframe.h"
#include "qultralightsettings.h"

QT_BEGIN_NAMESPACE
class QNetworkProxy;
class QUndoStack;
class QMenu;
class QNetworkRequest;
class QNetworkReply;
class QNetworkAccessManager;
class QScreen;
QT_END_NAMESPACE

class QWebElement;
class QWebNetworkRequest;
class QUltralightView;
class QUltralightFrame;
class QUltralightHistory;
class QUltralightSettings;

class QUltralightFrameData;
//class QWebFullScreenRequest;
class QUltralightHistoryItem;
class QUltralightHitTestResult;
class QWebNetworkInterface;
class QWebPluginFactory;
class QWebSecurityOrigin;
class QtViewportAttributesPrivate;

namespace WebCore {
    class ChromeClientQt;
    class EditorClientQt;
    class FrameLoaderClientQt;
    class InspectorClientQt;
    class InspectorServerRequestHandlerQt;
    class InspectorFrontendClientQt;
    class ResourceHandle;
    class QNetworkReplyHandler;

    struct FrameLoadRequest;
}

class QUltralightPage : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool modified READ isModified)
    Q_PROPERTY(QString selectedText READ selectedText)
    Q_PROPERTY(QString selectedHtml READ selectedHtml)
    Q_PROPERTY(bool hasSelection READ hasSelection)
    Q_PROPERTY(QSize viewportSize READ viewportSize WRITE setViewportSize)
    Q_PROPERTY(QSize preferredContentsSize READ preferredContentsSize WRITE setPreferredContentsSize)
    Q_PROPERTY(bool forwardUnsupportedContent READ forwardUnsupportedContent WRITE setForwardUnsupportedContent)
    Q_PROPERTY(LinkDelegationPolicy linkDelegationPolicy READ linkDelegationPolicy WRITE setLinkDelegationPolicy)
    Q_PROPERTY(QPalette palette READ palette WRITE setPalette)
    Q_PROPERTY(bool contentEditable READ isContentEditable WRITE setContentEditable)
    Q_PROPERTY(VisibilityState visibilityState READ visibilityState WRITE setVisibilityState)
    Q_PROPERTY(bool recentlyAudible READ recentlyAudible NOTIFY recentlyAudibleChanged)
    Q_ENUMS(LinkDelegationPolicy MessageLevel MessageSource NavigationType VisibilityState WebAction)

public:
    enum NavigationType {
        NavigationTypeLinkClicked,
        NavigationTypeFormSubmitted,
        NavigationTypeBackOrForward,
        NavigationTypeReload,
        NavigationTypeFormResubmitted,
        NavigationTypeOther
    };

    enum struct MenuAction {
        NoAction = - 1,
        OpenLink,
        OpenLinkInNewWindow,
        OpenLinkInThisWindow,
        DownloadLinkToDisk,
        CopyLinkToClipboard,
        OpenImageInNewWindow,
        DownloadImageToDisk,
        CopyImageToClipboard,
        CopyImageUrlToClipboard,
        OpenFrameInNewWindow,
        Copy,
        Back,
        Forward,
        Stop,
        Reload,
        Cut,
        Paste,
        Undo,
        Redo,
        SetTextDirectionDefault,
        SetTextDirectionLeftToRight,
        SetTextDirectionRightToLeft,
        ToggleBold,
        ToggleItalic,
        ToggleUnderline,
        SelectAll,
        DownloadMediaToDisk,
        CopyMediaUrlToClipboard,
        ToggleMediaControls,
        ToggleMediaLoop,
        ToggleMediaPlayPause,
        ToggleMediaMute,
        ToggleVideoFullscreen,
        InspectElement,
        ActionCount
    };

    enum WebAction {
        NoWebAction = - 1,
        OpenLink,
        OpenLinkInNewWindow,
        OpenFrameInNewWindow,
        DownloadLinkToDisk,
        CopyLinkToClipboard,
        OpenImageInNewWindow,
        DownloadImageToDisk,
        CopyImageToClipboard,
        Back,
        Forward,
        Stop,
        Reload,
        Cut,
        Copy,
        Paste,
        Undo,
        Redo,
        MoveToNextChar,
        MoveToPreviousChar,
        MoveToNextWord,
        MoveToPreviousWord,
        MoveToNextLine,
        MoveToPreviousLine,
        MoveToStartOfLine,
        MoveToEndOfLine,
        MoveToStartOfBlock,
        MoveToEndOfBlock,
        MoveToStartOfDocument,
        MoveToEndOfDocument,
        SelectNextChar,
        SelectPreviousChar,
        SelectNextWord,
        SelectPreviousWord,
        SelectNextLine,
        SelectPreviousLine,
        SelectStartOfLine,
        SelectEndOfLine,
        SelectStartOfBlock,
        SelectEndOfBlock,
        SelectStartOfDocument,
        SelectEndOfDocument,
        DeleteStartOfWord,
        DeleteEndOfWord,
        SetTextDirectionDefault,
        SetTextDirectionLeftToRight,
        SetTextDirectionRightToLeft,
        ToggleBold,
        ToggleItalic,
        ToggleUnderline,
        InspectElement,
        InsertParagraphSeparator,
        InsertLineSeparator,
        SelectAll,
        ReloadAndBypassCache,
        PasteAndMatchStyle,
        RemoveFormat,
        ToggleStrikethrough,
        ToggleSubscript,
        ToggleSuperscript,
        InsertUnorderedList,
        InsertOrderedList,
        Indent,
        Outdent,
        AlignCenter,
        AlignJustified,
        AlignLeft,
        AlignRight,
        StopScheduledPageRefresh,
        CopyImageUrlToClipboard,
        OpenLinkInThisWindow,
        DownloadMediaToDisk,
        CopyMediaUrlToClipboard,
        ToggleMediaControls,
        ToggleMediaLoop,
        ToggleMediaPlayPause,
        ToggleMediaMute,
        ToggleVideoFullscreen,
        RequestClose,
        Unselect,
        WebActionCount
    };

    enum FindFlag {
        FindBackward = 1,
        FindCaseSensitively = 2,
        FindWrapsAroundDocument = 4,
        HighlightAllOccurrences = 8,
        FindAtWordBeginningsOnly = 16,
        TreatMedialCapitalAsWordBeginning = 32,
        FindBeginsInSelection = 64,
        FindAtWordEndingsOnly = 128,
        FindExactMatchOnly = (FindAtWordBeginningsOnly | FindAtWordEndingsOnly)
    };
    Q_DECLARE_FLAGS(FindFlags, FindFlag)

    enum LinkDelegationPolicy {
        DontDelegateLinks,
        DelegateExternalLinks,
        DelegateAllLinks
    };

    enum WebWindowType {
        WebBrowserWindow,
        WebModalDialog
    };

    enum PermissionPolicy {
        PermissionUnknown,
        PermissionGrantedByUser,
        PermissionDeniedByUser
    };

    enum Feature {
        Notifications,
        Geolocation
    };

    enum VisibilityState {
        VisibilityStateVisible,
        VisibilityStateHidden,
        VisibilityStatePrerender,
        VisibilityStateUnloaded
    };

    enum MessageSource {
        XmlMessageSource,
        JSMessageSource,
        NetworkMessageSource,
        ConsoleAPIMessageSource,
        StorageMessageSource,
        AppCacheMessageSource,
        RenderingMessageSource,
        CSSMessageSource,
        SecurityMessageSource,
        ContentBlockerMessageSource,
        OtherMessageSource,
    };

    enum MessageLevel {
        LogMessageLevel = 1,
        WarningMessageLevel = 2,
        ErrorMessageLevel = 3,
        DebugMessageLevel = 4,
        InfoMessageLevel = 5,
    };

//    class ViewportAttributes {
//    public:
//        ViewportAttributes();
//        ViewportAttributes(const QUltralightPage::ViewportAttributes& other);

//        ~ViewportAttributes();

//        QUltralightPage::ViewportAttributes& operator=(const QUltralightPage::ViewportAttributes& other);

//        inline qreal initialScaleFactor() const { return m_initialScaleFactor; }
//        inline qreal minimumScaleFactor() const { return m_minimumScaleFactor; }
//        inline qreal maximumScaleFactor() const { return m_maximumScaleFactor; }
//        inline qreal devicePixelRatio() const { return m_devicePixelRatio; }
//        inline bool isUserScalable() const { return m_isUserScalable; }
//        inline bool isValid() const { return m_isValid; }
//        inline QSizeF size() const { return m_size; }

//    private:
//        QSharedDataPointer<QtViewportAttributesPrivate> d;
//        qreal m_initialScaleFactor;
//        qreal m_minimumScaleFactor;
//        qreal m_maximumScaleFactor;
//        qreal m_devicePixelRatio;
//        bool m_isUserScalable;
//        bool m_isValid;
//        QSizeF m_size;

//        friend class WebCore::ChromeClientQt;
//        friend class QUltralightPage;
//    };


    explicit QUltralightPage(QObject *parent = Q_NULLPTR);
    ~QUltralightPage();

    void createMainFrame();
    QUltralightFrame *mainFrame() const;
    QUltralightFrame *currentFrame() const;
    QUltralightFrame* frameAt(const QPoint& pos) const;

    QUltralightHistory *history() const;
    QUltralightSettings *settings() const;

    void setView(QWidget *view);
    QWidget *view() const;

    bool isModified() const;
#ifndef QT_NO_UNDOSTACK
    QUndoStack *undoStack() const;
#endif

    void setNetworkAccessManager(QNetworkAccessManager *manager);
    QNetworkAccessManager *networkAccessManager() const;

    void setPluginFactory(QWebPluginFactory *factory);
    QWebPluginFactory *pluginFactory() const;

    quint64 totalBytes() const;
    quint64 bytesReceived() const;

    VisibilityState visibilityState() const;
    void setVisibilityState(VisibilityState);

    bool recentlyAudible() const;

    bool hasSelection() const;
    QString selectedText() const;
    QString selectedHtml() const;

#ifndef QT_NO_ACTION
    QAction *action(WebAction action) const;
    QAction *customAction(int action) const;
#endif
    virtual void triggerAction(WebAction action, bool checked = false);

    void setDevicePixelRatio(qreal ratio);
    qreal devicePixelRatio() const;
    void resetDevicePixelRatio();

    QSize viewportSize() const;
    void setViewportSize(const QSize &size) const;
    //ViewportAttributes viewportAttributesForSize(const QSize& availableSize) const;

    QSize preferredContentsSize() const;
    void setPreferredContentsSize(const QSize &size) const;
    void setActualVisibleContentRect(const QRect& rect) const;

    bool event(QEvent*) Q_DECL_OVERRIDE;
    bool focusNextPrevChild(bool next);

    QVariant inputMethodQuery(Qt::InputMethodQuery property) const;

    bool findText(const QString &subString, FindFlags options = FindFlags());

    void setForwardUnsupportedContent(bool forward);
    bool forwardUnsupportedContent() const;

    void setLinkDelegationPolicy(LinkDelegationPolicy policy);
    LinkDelegationPolicy linkDelegationPolicy() const;

    void setPalette(const QPalette &palette);
    QPalette palette() const;

    void setContentEditable(bool editable);
    bool isContentEditable() const;

#ifndef QT_NO_CONTEXTMENU
    bool swallowContextMenuEvent(QContextMenuEvent *event);
#endif
    void updatePositionDependentActions(const QPoint &pos);

    QMenu *createStandardContextMenu();

    void setFeaturePermission(QUltralightFrame* frame, Feature feature, PermissionPolicy policy);

    QStringList supportedContentTypes() const;
    bool supportsContentType(const QString& mimeType) const;

    enum Extension {
        ChooseMultipleFilesExtension,
        ErrorPageExtension
    };
    class ExtensionOption
    {};
    class ExtensionReturn
    {};

    class ChooseMultipleFilesExtensionOption : public ExtensionOption {
    public:
        QUltralightFrame *parentFrame;
        QStringList suggestedFileNames;
    };

    class ChooseMultipleFilesExtensionReturn : public ExtensionReturn {
    public:
        QStringList fileNames;
    };

    enum ErrorDomain { QtNetwork, Http, WebKit };
    class ErrorPageExtensionOption : public ExtensionOption {
    public:
        QUrl url;
        QUltralightFrame* frame;
        ErrorDomain domain;
        int error;
        QString errorString;
    };

    class ErrorPageExtensionReturn : public ExtensionReturn {
    public:
        ErrorPageExtensionReturn() : contentType(QLatin1String("text/html")), encoding(QLatin1String("utf-8")) {};
        QString contentType;
        QString encoding;
        QUrl baseUrl;
        QByteArray content;
    };


    virtual bool extension(Extension extension, const ExtensionOption *option = Q_NULLPTR, ExtensionReturn *output = Q_NULLPTR);
    virtual bool supportsExtension(Extension extension) const;

    virtual bool shouldInterruptJavaScript();

Q_SIGNALS:
    void loadStarted();
    void loadProgress(int progress);
    void loadFinished(bool ok);

    void linkHovered(const QString &link, const QString &title, const QString &textContent);
    void statusBarMessage(const QString& text);
    void selectionChanged();
    void frameCreated(QUltralightFrame *frame);
    void geometryChangeRequested(const QRect& geom);
    void repaintRequested(const QRect& dirtyRect);
    void scrollRequested(int dx, int dy, const QRect& scrollViewRect);
    void windowCloseRequested();
    void printRequested(QUltralightFrame *frame);
    void linkClicked(const QUrl &url);

    void toolBarVisibilityChangeRequested(bool visible);
    void statusBarVisibilityChangeRequested(bool visible);
    void menuBarVisibilityChangeRequested(bool visible);

    void unsupportedContent(QNetworkReply *reply);
    void downloadRequested(const QNetworkRequest &request);

    void focusedElementChanged(const QWebElement &element);
    void microFocusChanged();
    void contentsChanged();
    void databaseQuotaExceeded(QUltralightFrame* frame, QString databaseName);
    void applicationCacheQuotaExceeded(QWebSecurityOrigin* origin, quint64 defaultOriginQuota, quint64 totalSpaceNeeded);

    void saveFrameStateRequested(QUltralightFrame* frame, QUltralightHistoryItem* item);
    void restoreFrameStateRequested(QUltralightFrame* frame);

    void viewportChangeRequested();

    void featurePermissionRequested(QUltralightFrame* frame, QUltralightPage::Feature feature);
    void featurePermissionRequestCanceled(QUltralightFrame* frame, QUltralightPage::Feature feature);
    //void fullScreenRequested(QWebFullScreenRequest fullScreenRequest);

    void consoleMessageReceived(MessageSource source, MessageLevel level, const QString& message, int lineNumber, const QString& sourceID);

    void recentlyAudibleChanged(bool recentlyAudible);

protected:
    virtual QUltralightPage *createWindow(WebWindowType type);
    virtual QObject *createPlugin(const QString &classid, const QUrl &url, const QStringList &paramNames, const QStringList &paramValues);

    virtual bool acceptNavigationRequest(QUltralightFrame *frame, const QNetworkRequest &request, NavigationType type);
    virtual QString chooseFile(QUltralightFrame *originatingFrame, const QString& oldFile);
    virtual void javaScriptAlert(QUltralightFrame *originatingFrame, const QString& msg);
    virtual bool javaScriptConfirm(QUltralightFrame *originatingFrame, const QString& msg);
    virtual bool javaScriptPrompt(QUltralightFrame *originatingFrame, const QString& msg, const QString& defaultValue, QString* result);
    virtual void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID);

    virtual QString userAgentForUrl(const QUrl& url) const;

private:
    QWidget *_view;
    QUltralightFrame *_mainFrame;
    QUltralightHistory *_history;
    QUltralightSettings *_settings;
    QUltralightHitTestResult *_hitTestResult;
    QUndoStack *_undoStack;

    friend class QUltralightFrame;
    //friend class QWebFullScreenRequest;
    friend class QUltralightView;
    friend class QUltralightHistory;
    //friend class QGraphicsWebView;
    //friend class QWebInspector;
    friend class WebCore::ChromeClientQt;
    friend class WebCore::EditorClientQt;
    friend class WebCore::FrameLoaderClientQt;
    friend class WebCore::InspectorClientQt;
    friend class WebCore::InspectorServerRequestHandlerQt;
    friend class WebCore::InspectorFrontendClientQt;
    friend class WebCore::ResourceHandle;
    friend class WebCore::QNetworkReplyHandler;
    friend class DumpRenderTreeSupportQt;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QUltralightPage::FindFlags)

#endif // QULTRALIGHTPAGE_H
