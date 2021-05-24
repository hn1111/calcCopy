#include "bitbutton.h"
#include <QDebug>

BitButton::BitButton(int ord, QWidget *parent) :
    QToolButton(parent),
    val(0),
    order(ord)
{
    setStyleSheet("border: none;");
    setText(QString::number(val));
    setMinimumWidth(6);
    setMaximumWidth(6);
}

int BitButton::getValue() const
{
    return val;
}

int BitButton::getOrder() const
{
    return order;
}
