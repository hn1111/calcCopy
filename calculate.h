#ifndef CALCULATE_H
#include <vector>
#include <QChar>
#include <QString>
#include <QRegularExpression>
#include <deque>
#include <map>
#include <qDebug>
#include "token.h"
#define CALCULATE_H


class Calculate
{
public:
    static QString calc(QString expression, int bbase = 10);

private:
    static std::deque<Token> convertToRPN(QVector<Token>);
    static QString changeForm(QString);
    static Token tokenFromSign(QChar);
    static QVector<Token> readTokens(QString s, int base = 10);
};

#endif // CALCULATE_H
