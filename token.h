#ifndef TOKEN_H
#include <QChar>
#include <QString>
#define TOKEN_H

class Token
{
public:
    Token();
    Token(QChar);
    Token(long double d);
    // for function
    //Token(QString);

    enum notationType{NONE, INFIX, PREFIX, POSTFIX};
    notationType notation;
    QChar type;
    long double val;
    char associativity = 'l';
    int params;
};

#endif // TOKEN_H
