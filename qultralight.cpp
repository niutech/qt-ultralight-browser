#include "qultralight.h"

QUltralight::QUltralight(QWidget *parent, Monitor *monitor) : QWidget(parent)
{
    window_ = Window::Create(monitor, 0, 0, false, kWindowFlags_Hidden);
    overlay_ = Overlay::Create(*window_.get(), 0, 0, 0, 0);
    overlay_->Resize(window_->width(), window_->height());
    overlay_->view()->set_load_listener(this);
    overlay_->view()->set_view_listener(this);
    window_->set_listener(this);

    QWidget *webview = createWindowContainer(QWindow::fromWinId((unsigned long) window_->native_handle()), this);
    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(webview);
    setLayout(layout);
}

QUltralight::~QUltralight() {
    overlay_->view()->set_load_listener(nullptr);
    overlay_->view()->set_view_listener(nullptr);
    window_->set_listener(nullptr);
    window_->Close();
    overlay_ = nullptr;
    window_ = nullptr;
}

QString QUltralight::url() {
    return QString::fromUtf8(overlay_->view()->url().utf8().data());
}

QString QUltralight::title() {
    return QString::fromUtf8(overlay_->view()->title().utf8().data());
}

void QUltralight::OnResize(Window* window, uint32_t width_px, uint32_t height_px) {
    overlay_->Resize(width_px, height_px);
}

void QUltralight::OnClose(Window* window) {
}

void QUltralight::OnChangeTitle(ultralight::View* caller, const String& title) {
    emit titleChanged(QString::fromUtf8(title.utf8().data()));
}

void QUltralight::OnChangeURL(ultralight::View* caller, const String& url) {
    emit urlChanged(QString::fromUtf8(url.utf8().data()));
}

void QUltralight::OnChangeTooltip(ultralight::View* caller, const String& tooltip) {
    emit tooltipChanged(QString::fromUtf8(tooltip.utf8().data()));
}

void QUltralight::OnBeginLoading(View* caller, uint64_t frame_id, bool is_main_frame, const String& url) {
    emit started(QString::fromUtf8(url.utf8().data()));
}

void QUltralight::OnFinishLoading(View* caller, uint64_t frame_id, bool is_main_frame, const String& url) {
    emit loaded(QString::fromUtf8(url.utf8().data()));
}

void QUltralight::OnFailLoading(View* caller, uint64_t frame_id, bool is_main_frame, const String& url, const String& description, const String& error_domain, int error_code) {
    emit failed(QString::fromUtf8(url.utf8().data()), QString::fromUtf8(description.utf8().data()));
}

void QUltralight::OnWindowObjectReady(View* caller, uint64_t frame_id, bool is_main_frame, const String& url) {
    emit windowReady(QString::fromUtf8(url.utf8().data()));
}

void QUltralight::OnDOMReady(View* caller, uint64_t frame_id, bool is_main_frame, const String& url) {
    emit domReady(QString::fromUtf8(url.utf8().data()));
}

void QUltralight::OnUpdateHistory(View* caller) {
    emit historyUpdated();
}

void QUltralight::OnChangeCursor(View* caller, Cursor cursor) {
    window_->SetCursor(cursor);
}

RefPtr<View> QUltralight::OnCreateChildView(View* caller, const String& opener_url, const String& target_url, bool is_popup, const IntRect& popup_rect) {
    emit childViewCreated(QString::fromUtf8(target_url.utf8().data()));
    return nullptr;
}

void QUltralight::load(QString url) {
    if (url.isEmpty()) return;
    if (!url.startsWith("http://") && !url.startsWith("https://")) url.prepend("http://");
    overlay_->view()->LoadURL((String8) url.toUtf8().constData());
}

void QUltralight::back() {
    overlay_->view()->GoBack();
}

void QUltralight::forward() {
    overlay_->view()->GoForward();
}

void QUltralight::reload() {
    overlay_->view()->Reload();
}

void QUltralight::stop() {
    overlay_->view()->Stop();
}

void QUltralight::sendKey(QKeyEvent *event) {
    KeyEvent evt;
    evt.type = KeyEvent::kType_Char;
    evt.text = (String8) event->text().toUtf8().constData();
    evt.unmodified_text = evt.text;
    evt.native_key_code = event->nativeScanCode();
    evt.virtual_key_code = event->nativeVirtualKey();
    evt.modifiers = event->modifiers();
    overlay_->view()->FireKeyEvent(evt);
}
