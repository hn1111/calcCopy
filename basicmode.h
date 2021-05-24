#ifndef BASICMODE_H
#define BASICMODE_H

#include <QWidget>
#include "widget.h"
#include "modebase.h"

namespace Ui {
class BasicMode;
}

class BasicMode : public ModeBase
{
    Q_OBJECT

public:
    explicit BasicMode(QWidget *parent = nullptr);
    ~BasicMode();

private:
    Ui::BasicMode *ui;
    void setup();
    Widget *par;
};

#endif // BASICMODE_H
