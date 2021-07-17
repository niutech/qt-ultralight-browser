#include "qultralight.h"

int QUltralight::QtModsToUltralightMods(int mods)
{
    int result = 0;
    if (mods & Qt::AltModifier)
      result |= KeyEvent::kMod_AltKey;
    if (mods & Qt::ControlModifier)
      result |= KeyEvent::kMod_CtrlKey;
    if (mods & Qt::MetaModifier)
      result |= KeyEvent::kMod_MetaKey;
    if (mods & Qt::ShiftModifier)
      result |= KeyEvent::kMod_ShiftKey;
    return result;
}

int QUltralight::QtKeyCodeToUltralightKeyCode(int key)
{
    using namespace KeyCodes;
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
    evt.type = KeyEvent::kType_RawKeyDown;
    evt.virtual_key_code = QtKeyCodeToUltralightKeyCode(event->key());
    evt.native_key_code = event->nativeScanCode();
    GetKeyIdentifierFromVirtualKeyCode(event->nativeVirtualKey(), evt.key_identifier);
    evt.modifiers = QtModsToUltralightMods(event->modifiers());
    overlay_->view()->FireKeyEvent(evt);
    // Support typing chars
    evt.type = KeyEvent::kType_Char;
    evt.text = (String8) event->text().toUtf8().constData();
    evt.unmodified_text = evt.text;
    overlay_->view()->FireKeyEvent(evt);
}
