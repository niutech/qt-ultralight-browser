#ifndef QULTRALIGHT_H
#define QULTRALIGHT_H

#include <QWindow>
#include <QWidget>
#include <QUrl>
#include <QGridLayout>
#include <QKeyEvent>
#include <AppCore/AppCore.h>

using namespace ultralight;

class QUltralight : public QWidget,
        public WindowListener,
        public LoadListener,
        public ViewListener
{
    Q_OBJECT
    RefPtr<Window> window_;
    RefPtr<Overlay> overlay_;
    static int QtModsToUltralightMods(int mods);
    static int QtKeyCodeToUltralightKeyCode(int key);

public:
    QUltralight(QWidget *parent = nullptr, Monitor *monitor = nullptr);
    ~QUltralight();
    QString url();
    QString title();
    virtual void OnResize(Window* window, uint32_t width_px, uint32_t height_px) override;
    virtual void OnClose(Window* window) override;
    virtual void OnChangeTitle(View* caller, const String& title) override;
    virtual void OnChangeURL(View* caller, const String& url) override;
    virtual void OnChangeTooltip(View* caller, const String& tooltip) override;
    virtual void OnBeginLoading(View* caller,
                                uint64_t frame_id,
                                bool is_main_frame,
                                const String& url) override;
    virtual void OnFinishLoading(View* caller,
                                 uint64_t frame_id,
                                 bool is_main_frame,
                                 const String& url) override;
    virtual void OnFailLoading(View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const String& url,
                               const String& description,
                               const String& error_domain,
                               int error_code) override;
    virtual void OnWindowObjectReady(View* caller,
                                     uint64_t frame_id,
                                     bool is_main_frame,
                                     const String& url) override;
    virtual void OnDOMReady(View* caller,
                            uint64_t frame_id,
                            bool is_main_frame,
                            const String& url) override;
    virtual void OnUpdateHistory(View* caller) override;
    virtual void OnChangeCursor(View* caller, Cursor cursor) override;
    virtual RefPtr<View> OnCreateChildView(View* caller,
                                           const String& opener_url,
                                           const String& target_url,
                                           bool is_popup,
                                           const IntRect& popup_rect) override;

public slots:
    void load(QString url);
    void back();
    void forward();
    void reload();
    void stop();
    void sendKey(QKeyEvent *event);

signals:
    void titleChanged(const QString &);
    void urlChanged(const QString &);
    void tooltipChanged(const QString &);
    void started(const QString &);
    void loaded(const QString &);
    void failed(const QString &, const QString &);
    void windowReady(const QString &);
    void domReady(const QString &);
    void historyUpdated();
    void childViewCreated(const QString &);
};

#endif // QULTRALIGHT_H
