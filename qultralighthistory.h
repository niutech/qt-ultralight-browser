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

#ifndef QULTRALIGHTHISTORY_H
#define QULTRALIGHTHISTORY_H

#include "qultralightview.h"

#include <QtCore/qurl.h>
#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qshareddata.h>
#include <QtGui/qicon.h>
#include <QVariant>

class QUltralightView;
class QUltralightPage;

namespace WebCore {
    class FrameLoaderClientQt;
}


class QUltralightHistoryInterface : public QObject {
    Q_OBJECT
public:
    explicit QUltralightHistoryInterface(QObject *parent = Q_NULLPTR);
    ~QUltralightHistoryInterface();

    static void setDefaultInterface(QUltralightHistoryInterface *defaultInterface);
    static QUltralightHistoryInterface *defaultInterface();

    virtual bool historyContains(const QString &url) const = 0;
    virtual void addHistoryEntry(const QString &url) = 0;
};


class QUltralightHistoryItem {
public:
    QUltralightHistoryItem();
    QUltralightHistoryItem(const QUltralightHistoryItem &other);
    QUltralightHistoryItem &operator=(const QUltralightHistoryItem &other);
    ~QUltralightHistoryItem();

    QUrl originalUrl() const;
    QUrl url() const;

    QString title() const;
    QDateTime lastVisited() const;

    QIcon icon() const;

    QVariant userData() const;
    void setUserData(const QVariant& userData);

    bool isValid() const;

    QVariantMap toMap() const;
    void loadFromMap(const QVariantMap &map);

private:
    QUrl _originalUrl;
    QUrl _url;
    QString _title;
    QDateTime _lastVisited;
    QIcon _icon;
    QVariant _userData;

    friend class QUltralightHistory;
    friend class QUltralightPage;
    friend class WebCore::FrameLoaderClientQt;
    friend class QUltralightHistoryItemPrivate;
    friend class DumpRenderTreeSupportQt;
    //friend QDataStream & operator<<(QDataStream& out,const QUltralightHistoryItem& hist);
    //friend QDataStream & operator>>(QDataStream& in,QUltralightHistoryItem& hist);
};

class QUltralightHistory {
public:
    void clear();

    QList<QUltralightHistoryItem> items() const;
    QList<QUltralightHistoryItem> backItems(int maxItems) const;
    QList<QUltralightHistoryItem> forwardItems(int maxItems) const;

    bool canGoBack() const;
    bool canGoForward() const;

    void back();
    void forward();
    void goToItem(const QUltralightHistoryItem &item);

    QUltralightHistoryItem backItem() const;
    QUltralightHistoryItem currentItem() const;
    QUltralightHistoryItem forwardItem() const;
    QUltralightHistoryItem itemAt(int i) const;

    int currentItemIndex() const;

    int count() const;

    int maximumItemCount() const;
    void setMaximumItemCount(int count);

    QVariantMap toMap() const;
    void loadFromMap(const QVariantMap &map);

private:
    QUltralightHistory();
    QUltralightHistory(QUltralightPage *page);
    ~QUltralightHistory();

    friend class QUltralightPage;
    friend QDataStream& operator>>(QDataStream&, QUltralightHistory&);
    friend QDataStream& operator<<(QDataStream&, const QUltralightHistory&);

    Q_DISABLE_COPY(QUltralightHistory)

    QList<QUltralightHistoryItem> _items;
    QUltralightPage *_page;
    QUltralightView *_view;
};

QDataStream& operator<<(QDataStream& stream, const QUltralightHistory& history);
QDataStream& operator>>(QDataStream& stream, QUltralightHistory& history);

#endif // QULTRALIGHTHISTORY_H
