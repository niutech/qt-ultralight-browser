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

#include "qultralightsettings.h"

#include <QFont>

QUltralightSettings::QUltralightSettings()
{
    // Initialize our global defaults
    _fontSizes.insert(QUltralightSettings::MinimumFontSize, 0);
    _fontSizes.insert(QUltralightSettings::MinimumLogicalFontSize, 0);
    _fontSizes.insert(QUltralightSettings::DefaultFontSize, 16);
    _fontSizes.insert(QUltralightSettings::DefaultFixedFontSize, 13);

    QFont defaultFont;
    defaultFont.setStyleHint(QFont::Serif);
    _fontFamilies.insert(QUltralightSettings::StandardFont, defaultFont.defaultFamily());
    _fontFamilies.insert(QUltralightSettings::SerifFont, defaultFont.defaultFamily());

    defaultFont.setStyleHint(QFont::Fantasy);
    _fontFamilies.insert(QUltralightSettings::FantasyFont, defaultFont.defaultFamily());

    defaultFont.setStyleHint(QFont::Cursive);
    _fontFamilies.insert(QUltralightSettings::CursiveFont, defaultFont.defaultFamily());

    defaultFont.setStyleHint(QFont::SansSerif);
    _fontFamilies.insert(QUltralightSettings::SansSerifFont, defaultFont.defaultFamily());

    defaultFont.setStyleHint(QFont::Monospace);
    _fontFamilies.insert(QUltralightSettings::FixedFont, defaultFont.defaultFamily());

    _attributes.insert(QUltralightSettings::AutoLoadImages, true);
    _attributes.insert(QUltralightSettings::DnsPrefetchEnabled, false);
    _attributes.insert(QUltralightSettings::JavascriptEnabled, true);
    _attributes.insert(QUltralightSettings::SpatialNavigationEnabled, false);
    _attributes.insert(QUltralightSettings::LinksIncludedInFocusChain, true);
    _attributes.insert(QUltralightSettings::ZoomTextOnly, false);
    _attributes.insert(QUltralightSettings::PrintElementBackgrounds, true);
    _attributes.insert(QUltralightSettings::OfflineStorageDatabaseEnabled, false);
    _attributes.insert(QUltralightSettings::OfflineWebApplicationCacheEnabled, false);
    _attributes.insert(QUltralightSettings::LocalStorageEnabled, false);
    _attributes.insert(QUltralightSettings::LocalContentCanAccessRemoteUrls, false);
    _attributes.insert(QUltralightSettings::LocalContentCanAccessFileUrls, true);
    _attributes.insert(QUltralightSettings::AcceleratedCompositingEnabled, false);
    _attributes.insert(QUltralightSettings::WebGLEnabled, true);
    _attributes.insert(QUltralightSettings::WebAudioEnabled, false);
    _attributes.insert(QUltralightSettings::MediaSourceEnabled, false);
    _attributes.insert(QUltralightSettings::MediaEnabled, true);
    _attributes.insert(QUltralightSettings::CSSRegionsEnabled, true);
    _attributes.insert(QUltralightSettings::CSSGridLayoutEnabled, false);
    _attributes.insert(QUltralightSettings::HyperlinkAuditingEnabled, false);
    _attributes.insert(QUltralightSettings::TiledBackingStoreEnabled, false);
    _attributes.insert(QUltralightSettings::FrameFlatteningEnabled, false);
    _attributes.insert(QUltralightSettings::SiteSpecificQuirksEnabled, true);
    _attributes.insert(QUltralightSettings::ScrollAnimatorEnabled, false);
    _attributes.insert(QUltralightSettings::CaretBrowsingEnabled, false);
    _attributes.insert(QUltralightSettings::NotificationsEnabled, true);
    _attributes.insert(QUltralightSettings::Accelerated2dCanvasEnabled, false);
    _attributes.insert(QUltralightSettings::WebSecurityEnabled, true);
    _attributes.insert(QUltralightSettings::FullScreenSupportEnabled, true);
    _attributes.insert(QUltralightSettings::ImagesEnabled, true);
    _attributes.insert(QUltralightSettings::AllowRunningInsecureContent, false);
    _attributes.insert(QUltralightSettings::ErrorPageEnabled, true);
    _offlineStorageDefaultQuota = 5 * 1024 * 1024;
    _defaultTextEncoding = QLatin1String("iso-8859-1");
    _thirdPartyCookiePolicy = AlwaysAllowThirdPartyCookies;
    apply();
}

QUltralightSettings* QUltralightSettings::globalSettings()
{
    static QUltralightSettings* global = 0;
    if (!global) {
        global = new QUltralightSettings;
    }
    return global;
}

QUltralightSettings::QUltralightSettings(WebCore::Page* page)
{
    apply();
}

QUltralightSettings::~QUltralightSettings()
{
}

void QUltralightSettings::setFontSize(FontSize type, int size)
{
    _fontSizes.insert(type, size);
    apply();
}

int QUltralightSettings::fontSize(FontSize type) const
{
    int defaultValue = globalSettings()->_fontSizes.value(type);
    return _fontSizes.value(type, defaultValue);
}

void QUltralightSettings::resetFontSize(FontSize type)
{
    _fontSizes.remove(type);
    apply();
}

void QUltralightSettings::setUserStyleSheetUrl(const QUrl& location)
{
    _userStyleSheetLocation = location;
    apply();
}

QUrl QUltralightSettings::userStyleSheetUrl() const
{
    return _userStyleSheetLocation;
}

void QUltralightSettings::setDefaultTextEncoding(const QString& encoding)
{
    _defaultTextEncoding = encoding;
    apply();
}

QString QUltralightSettings::defaultTextEncoding() const
{
    return _defaultTextEncoding;
}

void QUltralightSettings::setIconDatabasePath(const QString& path)
{
//    WebCore::initializeWebCoreQt();
//#if ENABLE(ICONDATABASE)
//    // Make sure that IconDatabaseClientQt is instantiated.
//    WebCore::IconDatabaseClientQt::instance();
//#endif

//    WebCore::IconDatabase::delayDatabaseCleanup();

//    WebCore::IconDatabaseBase& db = WebCore::iconDatabase();

//    if (!path.isEmpty()) {
//        db.setEnabled(true);
//        if (db.isOpen())
//            db.close();
//        QFileInfo info(path);
//        if (info.isDir() && info.isWritable())
//            db.open(path, WebCore::IconDatabase::defaultDatabaseFilename());
//    } else {
//        db.setEnabled(false);
//        db.close();
//    }
}

QString QUltralightSettings::iconDatabasePath()
{
//    WebCore::initializeWebCoreQt();
//    if (WebCore::iconDatabase().isEnabled() && WebCore::iconDatabase().isOpen())
//        return WebCore::iconDatabase().databasePath();
//    else
        return QString();
}

void QUltralightSettings::clearIconDatabase()
{
//    WebCore::initializeWebCoreQt();
//    if (WebCore::iconDatabase().isEnabled() && WebCore::iconDatabase().isOpen())
//        WebCore::iconDatabase().removeAllIcons();
}

QIcon QUltralightSettings::iconForUrl(const QUrl& url)
{
//    WebCore::initializeWebCoreQt();
//    QPixmap* icon = WebCore::iconDatabase().synchronousNativeIconForPageURL(WebCore::URL(url).string(),
//                                WebCore::IntSize(16, 16));
//    if (!icon)
//        return QIcon();
//    return* icon;
    return QIcon();
}

void QUltralightSettings::setPluginSearchPaths(const QStringList& paths)
{
//    WebCore::initializeWebCoreQt();

//    Vector<String> directories;

//    for (int i = 0; i < paths.count(); ++i)
//        directories.append(paths.at(i));

//    WebCore::PluginDatabase::installedPlugins()->setPluginDirectories(directories);
//    // PluginDatabase::setPluginDirectories() does not refresh the database.
//    WebCore::PluginDatabase::installedPlugins()->refresh();
}

QStringList QUltralightSettings::pluginSearchPaths()
{
//    WebCore::initializeWebCoreQt();

//    QStringList paths;

//    const Vector<String>& directories = WebCore::PluginDatabase::installedPlugins()->pluginDirectories();
//    for (unsigned i = 0; i < directories.size(); ++i)
//        paths.append(directories[i]);

//    return paths;
    return QStringList();
}

static const char* resourceNameForWebGraphic(QUltralightSettings::WebGraphic type)
{
    switch (type) {
    case QUltralightSettings::MissingImageGraphic: return "missingImage";
    case QUltralightSettings::MissingPluginGraphic: return "nullPlugin";
    case QUltralightSettings::DefaultFrameIconGraphic: return "urlIcon";
    case QUltralightSettings::TextAreaSizeGripCornerGraphic: return "textAreaResizeCorner";
    case QUltralightSettings::DeleteButtonGraphic: return "deleteButton";
    case QUltralightSettings::InputSpeechButtonGraphic: return "inputSpeech";
    case QUltralightSettings::SearchCancelButtonGraphic: return "searchCancelButton";
    case QUltralightSettings::SearchCancelButtonPressedGraphic: return "searchCancelButtonPressed";
    }
    return 0;
}

void QUltralightSettings::setWebGraphic(WebGraphic type, const QPixmap& graphic)
{
//    WebCore::initializeWebCoreQt();
//    WebCore::Image::setPlatformResource(resourceNameForWebGraphic(type), graphic);
}

QPixmap QUltralightSettings::webGraphic(WebGraphic type)
{
//    WebCore::initializeWebCoreQt();
//    RefPtr<WebCore::Image> img = WebCore::Image::loadPlatformResource(resourceNameForWebGraphic(type));
//    if (!img)
//        return QPixmap();
//    QPixmap* pixmap = img->nativeImageForCurrentFrame();
//    if (!pixmap)
//        return QPixmap();
//    return *pixmap;
    return QPixmap();
}

void QUltralightSettings::clearMemoryCaches()
{
//    WebCore::initializeWebCoreQt();

//    //FIXME: This code is very similar to QtTestSupport::clearMemoryCaches().

//    // Turn the cache on and off.  Disabling the object cache will remove all
//    // resources from the cache.  They may still live on if they are referenced
//    // by some Web page though.
//    auto& memoryCache = WebCore::MemoryCache::singleton();
//    if (!memoryCache.disabled()) {
//        memoryCache.setDisabled(true);
//        memoryCache.setDisabled(false);
//    }

//    auto& pageCache = WebCore::PageCache::singleton();
//    int pageCacheMaxSize = pageCache.maxSize();
//    // Setting size to 0, makes all pages be released.
//    pageCache.setMaxSize(0);
//    pageCache.setMaxSize(pageCacheMaxSize);

//    // Invalidating the font cache and freeing all inactive font data.
//    WebCore::FontCache::singleton().invalidate();

//    // Empty the Cross-Origin Preflight cache
//    WebCore::CrossOriginPreflightResultCache::singleton().empty();

//    // Drop JIT compiled code from ExecutableAllocator.
//    WebCore::GCController::singleton().deleteAllCode();
//    // Garbage Collect to release the references of CachedResource from dead objects.
//    WebCore::GCController::singleton().garbageCollectNow();

//    // FastMalloc has lock-free thread specific caches that can only be cleared from the thread itself.
//    WebCore::StorageThread::releaseFastMallocFreeMemoryInAllThreads();
//    WebCore::WorkerThread::releaseFastMallocFreeMemoryInAllThreads();
//    WTF::releaseFastMallocFreeMemory();
}

void QUltralightSettings::setMaximumPagesInCache(int pages)
{
//    QUltralightSettingsPrivate* global = QUltralightSettings::globalSettings()->d;
//    WebCore::PageCache::singleton().setMaxSize(qMax(0, pages));
//    global->apply();
}

int QUltralightSettings::maximumPagesInCache()
{
//    WebCore::initializeWebCoreQt();
//    return WebCore::PageCache::singleton().maxSize();
    return 1000;
}

void QUltralightSettings::setObjectCacheCapacities(int cacheMinDeadCapacity, int cacheMaxDead, int totalCapacity)
{
//    WebCore::initializeWebCoreQt();
//    bool disableCache = !cacheMinDeadCapacity && !cacheMaxDead && !totalCapacity;
//    auto& memoryCache = WebCore::MemoryCache::singleton();
//    memoryCache.setDisabled(disableCache);

//    memoryCache.setCapacities(qMax(0, cacheMinDeadCapacity),
//                              qMax(0, cacheMaxDead),
//                              qMax(0, totalCapacity));
//    memoryCache.setDeadDecodedDataDeletionInterval(disableCache ? std::chrono::seconds{0}
//                                                                : std::chrono::seconds{60});
}

void QUltralightSettings::setThirdPartyCookiePolicy(ThirdPartyCookiePolicy policy)
{
    _thirdPartyCookiePolicy = policy;
}

QUltralightSettings::ThirdPartyCookiePolicy QUltralightSettings::thirdPartyCookiePolicy() const
{
    return _thirdPartyCookiePolicy;
}

void QUltralightSettings::setCSSMediaType(const QString& type)
{
    _mediaType = type;
    apply();
}

QString QUltralightSettings::cssMediaType() const
{
    return _mediaType;
}

void QUltralightSettings::setFontFamily(FontFamily which, const QString& family)
{
    _fontFamilies.insert(which, family);
    apply();
}

QString QUltralightSettings::fontFamily(FontFamily which) const
{
    QString defaultValue = globalSettings()->_fontFamilies.value(which);
    return _fontFamilies.value(which, defaultValue);
}

void QUltralightSettings::resetFontFamily(FontFamily which)
{
    _fontFamilies.remove(which);
    apply();
}

void QUltralightSettings::setAttribute(WebAttribute attr, bool on)
{
    _attributes.insert(attr, on);
    apply();
}

bool QUltralightSettings::testAttribute(WebAttribute attr) const
{
    bool defaultValue = globalSettings()->_attributes.value(attr);
    return _attributes.value(attr, defaultValue);
}

void QUltralightSettings::resetAttribute(WebAttribute attr)
{
    _attributes.remove(attr);
    apply();
}

void QUltralightSettings::setOfflineStoragePath(const QString& path)
{
    globalSettings()->_offlineDatabasePath = path;
//    WebCore::DatabaseManager::singleton().setDatabaseDirectoryPath(path);
}

QString QUltralightSettings::offlineStoragePath()
{
    return globalSettings()->_offlineDatabasePath;
}

void QUltralightSettings::setOfflineStorageDefaultQuota(qint64 maximumSize)
{
    globalSettings()->_offlineStorageDefaultQuota = maximumSize;
}

qint64 QUltralightSettings::offlineStorageDefaultQuota()
{
    return globalSettings()->_offlineStorageDefaultQuota;
}

void QUltralightSettings::setOfflineWebApplicationCachePath(const QString& path)
{
//    WebCore::initializeWebCoreQt();
//    WebCore::ApplicationCacheStorage::singleton().setCacheDirectory(path);
}

QString QUltralightSettings::offlineWebApplicationCachePath()
{
//    WebCore::initializeWebCoreQt();
//    return WebCore::ApplicationCacheStorage::singleton().cacheDirectory();
    return QString();
}

void QUltralightSettings::setOfflineWebApplicationCacheQuota(qint64 maximumSize)
{
//    WebCore::initializeWebCoreQt();
//    auto& applicationCacheStorage = WebCore::ApplicationCacheStorage::singleton();
//    applicationCacheStorage.empty();
//    applicationCacheStorage.vacuumDatabaseFile();
//    applicationCacheStorage.setMaximumSize(maximumSize);
}

qint64 QUltralightSettings::offlineWebApplicationCacheQuota()
{
//    WebCore::initializeWebCoreQt();
//    return WebCore::ApplicationCacheStorage::singleton().maximumSize();
    return 1000;
}

void QUltralightSettings::setLocalStoragePath(const QString& path)
{
    _localStoragePath = path;
    apply();
}

QString QUltralightSettings::localStoragePath() const
{
    return _localStoragePath;
}

void QUltralightSettings::enablePersistentStorage(const QString& path)
{
//    WebCore::initializeWebCoreQt();
//#ifndef QT_NO_DESKTOPSERVICES
//    QString storagePath;

//    if (path.isEmpty()) {

//        storagePath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
//        if (storagePath.isEmpty())
//            storagePath = WebCore::pathByAppendingComponent(QDir::homePath(), QCoreApplication::applicationName());
//    } else
//        storagePath = path;

//    WebCore::makeAllDirectories(storagePath);

//    QUltralightSettings::setIconDatabasePath(storagePath);
//    QUltralightSettings::setOfflineWebApplicationCachePath(storagePath);
//    QUltralightSettings::setOfflineStoragePath(WebCore::pathByAppendingComponent(storagePath, "Databases"));
//    QUltralightSettings::globalSettings()->setLocalStoragePath(WebCore::pathByAppendingComponent(storagePath, "LocalStorage"));
//    QUltralightSettings::globalSettings()->setAttribute(QUltralightSettings::LocalStorageEnabled, true);
//    QUltralightSettings::globalSettings()->setAttribute(QUltralightSettings::OfflineStorageDatabaseEnabled, true);
//    QUltralightSettings::globalSettings()->setAttribute(QUltralightSettings::OfflineWebApplicationCacheEnabled, true);

//#if ENABLE(NETSCAPE_PLUGIN_METADATA_CACHE)
//    // All applications can share the common QtWebkit cache file(s).
//    // Path is not configurable and uses QDesktopServices::CacheLocation by default.
//    QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
//    WebCore::makeAllDirectories(cachePath);

//    QFileInfo info(cachePath);
//    if (info.isDir() && info.isWritable()) {
//        WebCore::PluginDatabase::setPersistentMetadataCacheEnabled(true);
//        WebCore::PluginDatabase::setPersistentMetadataCachePath(cachePath);
//    }
//#endif
//#endif
}

void QUltralightSettings::apply()
{
    //TODO
}

