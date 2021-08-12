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

#include "qultralighthistory.h"


static QUltralightHistoryInterface* default_interface;

void QUltralightHistoryInterface::setDefaultInterface(QUltralightHistoryInterface* defaultInterface)
{
    if (default_interface == defaultInterface)
        return;

    if (default_interface && !default_interface->parent())
        delete default_interface;

    default_interface = defaultInterface;
}

QUltralightHistoryInterface* QUltralightHistoryInterface::defaultInterface()
{
    return default_interface;
}

QUltralightHistoryInterface::QUltralightHistoryInterface(QObject* parent)
    : QObject(parent)
{
}

QUltralightHistoryInterface::~QUltralightHistoryInterface()
{
    if (default_interface == this)
        default_interface = 0;
}


QUltralightHistoryItem::QUltralightHistoryItem()
{
}

QUltralightHistoryItem::QUltralightHistoryItem(const QUltralightHistoryItem &other)
{
    loadFromMap(other.toMap());
}

QUltralightHistoryItem &QUltralightHistoryItem::operator=(const QUltralightHistoryItem &other)
{
    if (this != &other) {
        loadFromMap(other.toMap());
    }
    return *this;
}

QUltralightHistoryItem::~QUltralightHistoryItem()
{
}

QUrl QUltralightHistoryItem::originalUrl() const
{
    return _originalUrl;
}

QUrl QUltralightHistoryItem::url() const
{
    return _url;
}

QString QUltralightHistoryItem::title() const
{
    return _title;
}

QDateTime QUltralightHistoryItem::lastVisited() const
{
    return _lastVisited;
}

QIcon QUltralightHistoryItem::icon() const
{
    return _icon;
}

QVariant QUltralightHistoryItem::userData() const
{
    return _userData;
}

void QUltralightHistoryItem::setUserData(const QVariant& userData)
{
    _userData = userData;
}

bool QUltralightHistoryItem::isValid() const
{
    return _url.isValid();
}

QVariantMap QUltralightHistoryItem::toMap() const
{
    //TODO
    return QVariantMap();
}

void QUltralightHistoryItem::loadFromMap(const QVariantMap& map)
{
    //TODO
}

QUltralightHistory::QUltralightHistory()
{
}

QUltralightHistory::QUltralightHistory(QUltralightPage *page)
{
     _page = page;
}

QUltralightHistory::~QUltralightHistory()
{
    _items.clear();
}

void QUltralightHistory::clear()
{
//    //shortcut to private BackForwardList
//    WebCore::BackForwardList* lst = d->lst;

//    VisitedLinkStoreQt::singleton().removeAllVisitedLinks();

//    //if count() == 0 then just return
//    if (!lst->entries().size())
//        return;

//    RefPtr<WebCore::HistoryItem> current = lst->currentItem();
//    int capacity = lst->capacity();
//    lst->setCapacity(0);
//    lst->setCapacity(capacity);   //revert capacity

//    if (current) {
//        lst->addItem(*current); // insert old current item
//        lst->goToItem(current.get()); // and set it as current again
//    }

//    d->page()->updateNavigationActions();
}

QList<QUltralightHistoryItem> QUltralightHistory::items() const
{
//    WebCore::HistoryItemVector &items = d->lst->entries();

//    QList<QUltralightHistoryItem> ret;
//    for (unsigned i = 0; i < items.size(); ++i) {
//        QUltralightHistoryItemPrivate *priv = new QUltralightHistoryItemPrivate(items[i].get());
//        ret.append(QUltralightHistoryItem(priv));
//    }
//    return ret;
    return _items;
}

QList<QUltralightHistoryItem> QUltralightHistory::backItems(int maxItems) const
{
//    WebCore::HistoryItemVector items(maxItems);
//    d->lst->backListWithLimit(maxItems, items);

//    QList<QUltralightHistoryItem> ret;
//    for (unsigned i = 0; i < items.size(); ++i) {
//        QUltralightHistoryItemPrivate *priv = new QUltralightHistoryItemPrivate(items[i].get());
//        ret.append(QUltralightHistoryItem(priv));
//    }
//    return ret;
    return _items;
}

QList<QUltralightHistoryItem> QUltralightHistory::forwardItems(int maxItems) const
{
//    WebCore::HistoryItemVector items(maxItems);
//    d->lst->forwardListWithLimit(maxItems, items);

//    QList<QUltralightHistoryItem> ret;
//    for (unsigned i = 0; i < items.size(); ++i) {
//        QUltralightHistoryItemPrivate *priv = new QUltralightHistoryItemPrivate(items[i].get());
//        ret.append(QUltralightHistoryItem(priv));
//    }
//    return ret;
    return _items;
}

bool QUltralightHistory::canGoBack() const
{
    if (_page)
        return _page->_overlay->view()->CanGoBack();
    return false;
}

bool QUltralightHistory::canGoForward() const
{
    if (_page)
        return _page->_overlay->view()->CanGoForward();
    return false;
}

void QUltralightHistory::back()
{
    if (canGoBack())
        _page->_overlay->view()->GoBack();
}

void QUltralightHistory::forward()
{
    if (canGoForward())
        _page->_overlay->view()->GoForward();
}

void QUltralightHistory::goToItem(const QUltralightHistoryItem &item)
{
    //TODO calclate offset
    int offset = 0;
    if ((offset < 0 && canGoBack()) || (offset > 0 && canGoForward()))
        _page->_overlay->view()->GoToHistoryOffset(offset);
}

QUltralightHistoryItem QUltralightHistory::backItem() const
{
//    WebCore::HistoryItem *i = d->lst->backItem();
//    QUltralightHistoryItemPrivate *priv = new QUltralightHistoryItemPrivate(i);
//    return QUltralightHistoryItem(priv);
    return QUltralightHistoryItem();
}

QUltralightHistoryItem QUltralightHistory::currentItem() const
{
//    WebCore::HistoryItem *i = d->lst->currentItem();
//    QUltralightHistoryItemPrivate *priv = new QUltralightHistoryItemPrivate(i);
//    return QUltralightHistoryItem(priv);
    return QUltralightHistoryItem();
}

QUltralightHistoryItem QUltralightHistory::forwardItem() const
{
//    WebCore::HistoryItem *i = d->lst->forwardItem();
//    QUltralightHistoryItemPrivate *priv = new QUltralightHistoryItemPrivate(i);
//    return QUltralightHistoryItem(priv);
    return QUltralightHistoryItem();
}

int QUltralightHistory::currentItemIndex() const
{
//    return d->lst->backListCount();
    return 0;
}

QUltralightHistoryItem QUltralightHistory::itemAt(int i) const
{
//    QUltralightHistoryItemPrivate *priv;
//    if (i < 0 || i >= count())
//        priv = new QUltralightHistoryItemPrivate(0);
//    else {
//        WebCore::HistoryItem& item = d->lst->entries()[i].get();
//        priv = new QUltralightHistoryItemPrivate(item);
//    }
//    return QUltralightHistoryItem(priv);
    return QUltralightHistoryItem();
}

int QUltralightHistory::count() const
{
    return _items.size();
}

int QUltralightHistory::maximumItemCount() const
{
    return 1000;
}

void QUltralightHistory::setMaximumItemCount(int count)
{
//    d->lst->setCapacity(count);
}

QVariantMap QUltralightHistory::toMap() const
{
//    WebCore::KeyedEncoderQt encoder;
//    encoder.encodeUInt32("currentItemIndex", currentItemIndex());

//    const WebCore::HistoryItemVector &items = d->lst->entries();
//    encoder.encodeObjects("history", items.begin(), items.end(), [&encoder](WebCore::KeyedEncoder&, const WebCore::HistoryItem& item) {
//        WebCore::encodeBackForwardTree(encoder, item);
//    });

//    return encoder.toMap();
    return QVariantMap();
}

void QUltralightHistory::loadFromMap(const QVariantMap& map)
{
//    clear();

//    // after clear() is new clear HistoryItem (at the end we had to remove it)
//    WebCore::HistoryItem* nullItem = d->lst->currentItem();

//    WebCore::KeyedDecoderQt decoder { QVariantMap(map) };

//    int currentIndex;
//    if (!decoder.decodeInt32("currentItemIndex", currentIndex))
//        return;

//    auto* lst = d->lst;
//    Vector<int> ignore;
//    bool result = decoder.decodeObjects("history", ignore, [&lst, &decoder](WebCore::KeyedDecoder&, int&) -> bool {
//        auto item = WebCore::HistoryItem::create();
//        if (!WebCore::decodeBackForwardTree(decoder, item))
//            return false;
//        lst->addItem(WTFMove(item));
//        return true;
//    });

//    if (result && !d->lst->entries().isEmpty()) {
//        d->lst->removeItem(nullItem);
//        goToItem(itemAt(currentIndex));
//    }

//    d->page()->updateNavigationActions();
}

QDataStream& operator<<(QDataStream& target, const QUltralightHistory& history)
{
//    target << HistoryStreamVersion;
    target << history.toMap();
    return target;
}

QDataStream& operator>>(QDataStream& source, QUltralightHistory& history)
{
    // Clear first, to have the same behavior if our version doesn't match and if the HistoryItem's version doesn't.
    history.clear();

//    // This version covers every field we serialize in QUltralightHistory.cpp and HistoryItemQt.cpp (like the HistoryItem::userData()).
//    // HistoryItem has its own version in the stream covering the work done in encodeBackForwardTree.
//    // If any of those two stream version changes, the effect should be the same and the QUltralightHistory should fail to restore.
    int version;
    source >> version;
//    if (version != HistoryStreamVersion) {
//        // We do not try to decode previous history stream versions.
//        // Make sure that our history is cleared and mark the rest of the stream as invalid.
//        ASSERT(history.count() <= 1);
//        source.setStatus(QDataStream::ReadCorruptData);
//        return source;
//    }

    QVariantMap map;
    source >> map;
    history.loadFromMap(map);

    return source;
}
