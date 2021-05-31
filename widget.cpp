#include "widget.h"
#include "./ui_widget.h"
#include "calculate.h"
#include "programmingmode.h"
#include "basicmode.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget)
{
    ui->setupUi(this);
    setup();
}

void Widget::setup()
{
    ui->comboBox->addItem("Basic Mode");
    ui->comboBox->addItem("Programming Mode");

    ui->stackedWidget->addWidget(new BasicMode(this));
    ui->stackedWidget->addWidget(new ProgrammingMode(this));

    changePage(0);
    expressionHistory = {};
    actionHistory.append("");
    ui->lineEdit->setFocus();
}

void Widget::addPushedButtonText()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    auto text = button->text();

    // put special symbols
    if (text == "XOR")
        text = XOR;
    else if (text == "OR")
        text = OR;
    else if (text == "AND")
        text = AND;
    else if (text == "NOT")
        text = NOT;
    ui->lineEdit->insert(text);
}

void Widget::calculate()
{
    QString expression = ui->lineEdit->text().trimmed();
    if (expression == "")
        return;

    Calculate calc;
    QString solution;
    try {
        solution = calc.calc(expression, base);
    } catch (const std::runtime_error& e) {
        ui->infoLabel->setText(e.what());
        return;
    }

    // insert into history
    expressionHistory.append(expression);

    QString text = QString(ui->lineEdit->text()) + QString(" = ") + solution;
    ui->listWidget->addItem(text);
    ui->listWidget->scrollToBottom();

    ui->lineEdit->clear();
    ui->lineEdit->insert(solution);

}

void Widget::clear()
{
    ui->lineEdit->clear();
    ui->infoLabel->clear();
    emit lineValueChanged();
}

void Widget::back()
{
    if (actionHistory.size() > 1)
        actionHistory.pop_back();

    ui->infoLabel->clear();
    ui->lineEdit->setText(actionHistory.back());
    emit lineValueChanged();
}

long double Widget::getLineValue(bool &success) const
{
    if (ui->lineEdit->text() == QString("")) {
        success = true;
        return 0;
    }
    long double t = ui->lineEdit->text().toULongLong(&success, base);
    return t;
}

void Widget::putText(QString s)
{
    if (s == ui->lineEdit->text())
        return;
    actionHistory.append(ui->lineEdit->text());
    ui->lineEdit->setText(s);
    ui->infoLabel->clear();
    ui->lineEdit->setFocus();
    emit lineValueChanged();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int index = ui->listWidget->row(item);
    putText(expressionHistory[index]);
//    ui->lineEdit->clear();
//    ui->lineEdit->setText(expressionHistory[index]);
//    emit lineValueChanged();
}

void Widget::on_lineEdit_textEdited(const QString &arg1)
{
    QString text = QString(ui->lineEdit->text());
    text.replace('*', MULTIPLICATION);
    text.replace('/', DIVISION);

    actionHistory.append(text);
    ui->lineEdit->setText(text);
    ui->infoLabel->clear();
    ui->lineEdit->setFocus();
    emit lineValueChanged();
}

void Widget::on_lineEdit_returnPressed()
{
    calculate();
}

void Widget::on_comboBox_currentIndexChanged(int index)
{
    changePage(index);
}

void Widget::bitsChanged()
{
    BitArray* ba = qobject_cast<BitArray*>(sender());
    putText(QString::number(ba->getBitsValue(), base));
}

void Widget::changeBase(int b)
{
    qDebug() << "change base" << b;
    base = b;
    ui->lineEdit->setFocus();
    emit lineValueChanged();
}

void Widget::changePage(int index)
{
    auto prevPage = qobject_cast<ModeBase*>(ui->stackedWidget->currentWidget());
    if (!prevPage)
        return;
    prevPage->deactivate();

    ui->stackedWidget->setCurrentIndex(index);
    auto nextPage = qobject_cast<ModeBase*>(ui->stackedWidget->currentWidget());
    nextPage->activate();

    ui->stackedWidget->adjustSize();
    //adjustSize();
    this->window()->adjustSize();

    ui->lineEdit->setFocus();
}
