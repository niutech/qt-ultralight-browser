#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qultralight.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QMessageBox>
#include <AppCore/AppCore.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace ultralight;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    RefPtr<App> app_;
    QLineEdit *urlBar_;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void run();
    QUltralight *currentTab();
    void newTab(QString url = QString());
    void closeTab(int index);
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
