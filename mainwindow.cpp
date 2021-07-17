#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString home("https://duckduckgo.com/");

    Settings settings_;
    settings_.developer_name = "Niu Tech";
    settings_.app_name = "Qt Ultralight Browser";
    app_ = App::Create(settings_);

    // URL bar
    urlBar_ = new QLineEdit(ui->toolBar);
    ui->toolBar->addWidget(urlBar_);
    connect(urlBar_, &QLineEdit::returnPressed, [=]() {
        currentTab()->load(urlBar_->text());
        urlBar_->clearFocus();
    });
    connect(ui->action_Back, &QAction::triggered, [=]() {
        currentTab()->back();
    });
    connect(ui->action_Forward, &QAction::triggered, [=]() {
        currentTab()->forward();
    });
    connect(ui->action_Reload, &QAction::triggered, [=]() {
        currentTab()->reload();
    });
    connect(ui->action_Stop, &QAction::triggered, [=]() {
        currentTab()->stop();
    });
    connect(ui->action_Home, &QAction::triggered, [=]() {
        currentTab()->load(home);
    });

    // Menu bar
    connect(ui->action_New, &QAction::triggered, [=]() {
        newTab();
    });
    connect(ui->action_Close, &QAction::triggered, [=]() {
        closeTab(ui->tabWidget->currentIndex());
    });
    connect(ui->action_About, &QAction::triggered, [=]() {
        QMessageBox::aboutQt(this, windowTitle());
    });

    // Tab bar
    connect(ui->tabWidget, &QTabWidget::currentChanged, [=]() {
        if (currentTab()) urlBar_->setText(currentTab()->url());
    });
    connect(ui->tabWidget, &QTabWidget::tabBarDoubleClicked, [=]() {
        newTab();
    });
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, [=](int index) {
        closeTab(index);
    });

    newTab(home);
}

MainWindow::~MainWindow() {
    app_ = nullptr;
    delete ui;
}

void MainWindow::run() {
    app_->Run();
}

QUltralight* MainWindow::currentTab() {
    return (QUltralight*) ui->tabWidget->currentWidget();
}

void MainWindow::newTab(QString url) {
    QUltralight *tab = new QUltralight(ui->tabWidget, app_->main_monitor());
    ui->tabWidget->addTab(tab, "New tab");
    ui->tabWidget->setCurrentWidget(tab);
    connect(tab, &QUltralight::titleChanged, [=](const QString &title) {
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), title);
    });
    connect(tab, &QUltralight::urlChanged, [=](const QString &url) {
        urlBar_->setText(url);
        ui->statusbar->showMessage("Loading " + url + "...");
    });
    connect(tab, &QUltralight::tooltipChanged, [=](const QString &tooltip) {
        ui->statusbar->showMessage(tooltip);
    });
    connect(tab, &QUltralight::started, [=](const QString &url) {
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), "Loading...");
        ui->statusbar->showMessage("Loading " + url + "...");
        ui->action_Reload->setVisible(false);
        ui->action_Stop->setVisible(true);
    });
    connect(tab, &QUltralight::loaded, [=](const QString &url) {
        ui->statusbar->showMessage("Done");
        ui->action_Reload->setVisible(true);
        ui->action_Stop->setVisible(false);
    });
    connect(tab, &QUltralight::failed, [=](const QString &url, const QString &description) {
        ui->statusbar->showMessage("Failed to load " + url + ": " + description);
        ui->action_Reload->setVisible(true);
        ui->action_Stop->setVisible(false);
    });
    connect(tab, &QUltralight::childViewCreated, [=](const QString &url) {
        newTab(url);
    });
    tab->load(url);
}

void MainWindow::closeTab(int index) {
    ui->tabWidget->widget(index)->deleteLater();
    ui->tabWidget->removeTab(index);
    if (ui->tabWidget->count() < 1)
        newTab();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    app_->Quit();
    exit(0);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (currentTab()) currentTab()->sendKey(event);
}
