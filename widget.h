#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include <QPushButton>

const QChar ARROW = u'⤺';
constexpr QChar MULTIPLICATION = u'×';
constexpr QChar SQUARE_ROOT = u'√';
constexpr QChar DIVISION = u'÷';

constexpr QChar XOR = u'⊻';
constexpr QChar OR = u'∨';
constexpr QChar AND = u'∧';
constexpr QChar NOT = u'¬';

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void setup();
    long double getLineValue(bool &success) const;


public slots:
    void bitsChanged();
    void changeBase(int);
    void calculate();
    void addPushedButtonText();
    void clear();
    void back();

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_returnPressed();

    void on_comboBox_currentIndexChanged(int index);

signals:
    void lineValueChanged();

private:
    Ui::Widget *ui;
    QList<QString> expressionHistory;
    QList<QString> actionHistory;
    int base = 10;
    void changePage(int i);
    void putText(QString);
};


#endif // WIDGET_H
