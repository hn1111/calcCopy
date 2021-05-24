#include "token.h"
#include "widget.h"

Token::Token() : notation(NONE),
                 type(0),
                 val(0),
                 associativity('l'),
                 params(0)
{

}

Token::Token(QChar ch) : Token()
{
    type = ch;
    static const QString PREFIX_OPERATIONS = QString("") + SQUARE_ROOT + NOT;
    static const QString POSTFIX_OPERATIONS = QString("%");

    // other chars are invalid
    static const QString BINARY_OPERATIONS_CHARS = QString("()+-^%") +
                                            MULTIPLICATION +
                                            DIVISION +
                                            XOR + OR + AND;
    static const QString UNARY_OPERATIONS_CHAR = QString("") + SQUARE_ROOT + NOT;

    if (ch == '^' or ch == '%')
        associativity = 'r';
    if (BINARY_OPERATIONS_CHARS.contains(type))
        params = 2;
    else if (UNARY_OPERATIONS_CHAR.contains(type)) {
        params = 1;
    }

    if (PREFIX_OPERATIONS.contains(type))
        notation = PREFIX;
    else if (POSTFIX_OPERATIONS.contains(type))
        notation = POSTFIX;
    else
        notation = INFIX;
}

Token::Token(long double value) : Token()
{
    type = 'd';
    val = value;
}
