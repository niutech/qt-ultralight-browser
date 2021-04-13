#include "qtultralight.h"
#include "ui_qtultralight.h"
#include <QWindow>
#include <QKeyEvent>
#include <QHBoxLayout>

QtUltralight::QtUltralight(QWidget *parent) : QWidget(parent), ui(new Ui::QtUltralight)
{
    ui->setupUi(this);

    parentWidget()->ui
}

QtUltralight::~QtUltralight()
{
    delete ui;
}


