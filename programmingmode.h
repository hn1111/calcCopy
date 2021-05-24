#ifndef PROGRAMMINGMODE_H
#define PROGRAMMINGMODE_H

#include <QWidget>
#include "widget.h"
#include "modebase.h"
#include <QGroupBox>
#include "bitbutton.h"
#include <QLabel>
#include <QList>
#include "bitarray.h"

class BitArray;
namespace Ui {
class ProgrammingMode;
}

class ProgrammingMode : public ModeBase
{
    Q_OBJECT

public:
    explicit ProgrammingMode(QWidget *parent = nullptr);
    quint64 getBitsNumber();
    void activate() override;
    void deactivate() override;

    ~ProgrammingMode();

signals:
    void stringChange(QString);
    void baseChange(int base);

private slots:

    int getBase(int index);

    void on_leftShiftComboBox_activated(int index);

    void on_rightShiftComboBox_activated(int index);

private:
    Ui::ProgrammingMode *ui;
    void setup();
    void changeBits();
};

#endif // PROGRAMMINGMODE_H
