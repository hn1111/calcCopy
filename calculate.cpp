#include "calculate.h"
#include "widget.h"


QVector<Token> Calculate::readTokens(QString expression, int base) {
    const QRegularExpression NUM10("^([0-9]*\\.?[0-9]+)");
    // float values aren't supported for non 10 base numbers
    const QRegularExpression NUM2("^([0-1]+)");
    const QRegularExpression NUM8("^([0-7]+)");
    const QRegularExpression NUM16("^([0-9A-Fa-f]+)");
    QRegularExpression num;
    switch(base) {
    case 2:
        num = NUM2;
        break;
    case 8:
        num = NUM8;
        break;
    case 10:
        num = NUM10;
        break;
    case 16:
        num = NUM16;
        break;
    default:
        throw std::runtime_error("Wrong base.");
    }

    QVector<Token> tokens;
    bool start = true; // TODO: delete?
    while (expression.size() != 0) {
        Token token;
        QRegularExpressionMatch match =  (num).match(expression);
        if (match.hasMatch()) { // if it's number

            if (base == 10)
                token = Token(match.captured(0).toDouble());
            else
                token = Token(match.captured(0).toULongLong(nullptr, base));
            tokens.push_back(token);
            start = false;

            expression = expression.sliced(match.captured(0).size());
        } else {
            token = Token(expression[0]);
            if (token.type == '-' && start) {
                token.notation = token.PREFIX;
                token.params = 1;
            }
            tokens.push_back(token);
            start = true;
            expression = expression.sliced(1);
        }

    }
    return tokens;
}

QString Calculate::calc(QString expression, int base)
{
    QVector<Token> tokens = readTokens(changeForm(expression), base);

    std::deque<Token> RPN_Tokens = convertToRPN(tokens);
    std::vector<Token> numbers;

    for (auto token : RPN_Tokens) {
        if (token.type == 'd')
        {
            numbers.push_back(token);
            continue;
        }
        // % in the begin calculated as 0.xx
        // % in the middle - error
        // % in the end - a * 0.xx

        Token n2,n1,n;        
        n.type = 'd';

        if (numbers.size() < static_cast<decltype (numbers.size())>(token.params))
            throw std::runtime_error("Error. Wrong use of operands.");

        if (token.params == 2) {
            n2 = numbers.back();
            numbers.pop_back();
            n1 = numbers.back();
            numbers.pop_back();

            switch(token.type.unicode())
            {
            case '+':
                n.val = n1.val + n2.val;
                break;
            case '-':
                n.val = n1.val - n2.val;
                break;
            case MULTIPLICATION.unicode():
                n.val = n1.val * n2.val;
                break;
            case DIVISION.unicode():
                if (n2.val == 0)
                    throw std::runtime_error("Division by zero is undefined");
                n.val = n1.val / n2.val;

                break;
            case '^':
                n.val = pow(n1.val, n2.val);
                break;
            case '%':
                n.val = n1.val * 0.01 * n2.val;
                numbers.push_back(n1);
                break;
            case XOR.unicode():
                n.val = static_cast<int>(n1.val) ^ static_cast<int>(n2.val);
                break;
            case AND.unicode():
                n.val = static_cast<int>(n1.val) & static_cast<int>(n2.val);
                break;
            case OR.unicode():
                n.val = static_cast<int>(n1.val) | static_cast<int>(n2.val);
                break;
            default :
                throw std::runtime_error("Error. Unknown operator.");
            }
        } else if (token.params == 1) {
            n1 = numbers.back();
            numbers.pop_back();
            switch(token.type.unicode()) {
            case SQUARE_ROOT.unicode():
                n.val = pow(n1.val, 0.5);
                break;
            case '%':
                n.val = n1.val * 0.01;
                break;
            case NOT.unicode():
                if (n1.val < 0) {
                    throw std::runtime_error("Not defined for negative.");
                }
                n.val = ~static_cast<unsigned long long int>(n1.val);
                break;
            case '-':
                n.val = -n1.val;
            default:
                throw std::runtime_error("Error. Unknown operator.");
            }
        }
        numbers.push_back(n);
    }
    if (numbers.size() != 1)
        throw std::runtime_error("Error. Missed operand?");
    if (base == 10)
        return QString::number(static_cast<double>(numbers.back().val));
    else {
        long double t = 0;
        t = std::modf(numbers.back().val, nullptr);
        if (t)
            throw std::runtime_error("mal9");

        return QString::number(
                    static_cast<unsigned long long int>(numbers.back().val),
                    base);

    }

}


std::deque<Token> Calculate::convertToRPN(QVector<Token> tokens) {
    // RPN = Reverse Polish notation
    // Shunting-yard algorithm
    //
    // Information on prefix and postfix operators modification.
    // https://stackoverflow.com/questions/16380234/handling-extra-operators-in-shunting-yard/16392115#16392115
    bool need_operand = true;
    QVector<Token> operators;
    std::deque<Token> output;

    std::map<QChar, int> precedence {{XOR, 0}, {OR, 0}, {XOR, 0},
                                     {'+', 1}, {'-', 1},
                                     {MULTIPLICATION, 2}, {DIVISION, 2},
                                     {'^', 3}, {NOT, 3},
                                     {SQUARE_ROOT, 4}, {'%', 4}};

    for (Token token : tokens) {
        if (need_operand) {
            if (token.notation == Token::PREFIX || token.type == '(')
                operators.push_back(token);
            else if (token.type == 'd') {
                output.push_back(token);
                need_operand = false;
            } else
                throw std::runtime_error("Error. Can't find operand.");

        } else { // need operator
            if (token.notation == Token::POSTFIX) {
                if (token.type == '%') {
                    token.params = operators.empty() ? 1 : 2;
                }
                operators.push_back(token);
             } else if (token.type == ')') {
                while (true) {
                    if (operators.size() == 0)
                        throw std::runtime_error("m4");
                    Token op;
                    op = operators.back();
                    if (op.type == '(') {
                        // For functions implementation add:
                        // if (op.notation == Token::INFIX)
                            // add call
                        operators.pop_back();
                        break;
                    }
                    output.push_back(op);
                    operators.pop_back();
                }

            } else if (token.notation == Token::INFIX) {
                while (!operators.empty()
                       && ((precedence[operators.back().type] > precedence[token.type])
                           || (precedence[operators.back().type] == precedence[token.type]
                               && token.associativity == 'l'))
                       && operators.back().type != '(') {
                    output.push_back(operators.back());
                    operators.pop_back();
                }
                operators.push_back(token);
                need_operand = true;
            } else if (token.type == 'd')
                throw std::runtime_error("m5");
            // For function implementation add: if token is ','
        }

    }
    if (need_operand)
        throw std::runtime_error("Error. Missed operand?");
    while (operators.size() > 0) {
        if (operators.back().type == '(')
            throw std::runtime_error("Error. (");
        output.push_back(operators.back());
        operators.pop_back();
    }
    return output;
}

QString Calculate::changeForm(QString expression)
{
    // remove spaces, put multiplication sign
    static const QString OPERATIONS = QString("(") + SQUARE_ROOT;

    expression.remove(' ');
    // can live without inserting *,
    // but I need to change algorithm in calc function
    decltype (expression.size()) i = 0;
    while (i < expression.size() - 1) {
        QChar curr = expression[i];
        QChar next = expression[i+1];
        if (((curr.isDigit() || curr == ')') && OPERATIONS.contains(next)) ||
            (curr == ')' && (next.isDigit() || next == '(')))
        {
            expression.insert(i+1, MULTIPLICATION);
            ++i;
        }
        ++i;
    }
    return expression;
}
