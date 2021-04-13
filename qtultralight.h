#ifndef QTULTRALIGHT_H
#define QTULTRALIGHT_H

#include <QWidget>
#include <AppCore/AppCore.h>

namespace Ui {
class QtUltralight;
}


class QtUltralight : public QWidget
{
    Q_OBJECT

public:
    explicit QtUltralight(QWidget *parent = nullptr);
    ~QtUltralight();


private:
    Ui::QtUltralight *ui;
};

#endif // QTULTRALIGHT_H
