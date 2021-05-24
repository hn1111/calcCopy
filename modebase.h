#ifndef MODEBASE_H
#define MODEBASE_H

#include <QWidget>
#include "widget.h"
#include <QPushButton>

namespace Ui {
class ModeBase;

}

class ModeBase : public QWidget
{
    Q_OBJECT

public:
    explicit ModeBase(QWidget *parent = nullptr);
    virtual void activate();
    virtual void deactivate();
    ~ModeBase() = 0;

protected:
    void setup(QList<QPushButton*> lst);
    int BUTTON_SIZE = 45;

private:
    Ui::ModeBase *ui;
    Widget *par;
};

#endif // MODEBASE_H
