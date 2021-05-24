#ifndef BITBUTTON_H
#define BITBUTTON_H

#include <QObject>
#include <QToolButton>
#include "programmingmode.h"

class BitButton : public QToolButton
{
    Q_OBJECT
public:
    explicit BitButton(int order, QWidget *parent = nullptr);
    int getValue() const;
    void setValue(int n) {
        val = n;
        setText(QString::number(n));
    }
    int getOrder() const;

signals:

private:
    int val;
    int order;

};

#endif // BITBUTTON_H
