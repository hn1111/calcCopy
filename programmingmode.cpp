#include "programmingmode.h"
#include "ui_programmingmode.h"

ProgrammingMode::ProgrammingMode(QWidget *parent) :
    ModeBase(parent),
    ui(new Ui::ProgrammingMode)
{
    ui->setupUi(this);
    setup();
}

void ProgrammingMode::activate()
{
    ModeBase::activate();
}

void ProgrammingMode::deactivate()
{
    ui->baseComboBox->setCurrentIndex(2); // 10 base as default
    ModeBase::deactivate();
}

ProgrammingMode::~ProgrammingMode()
{
    delete ui;
}

void ProgrammingMode::setup()
{
    ModeBase::setup(findChildren<QPushButton*>(QString()));

    // set to decimal
    ui->baseComboBox->setCurrentIndex(2);

    // bit array
    connect(qobject_cast<Widget*>(parentWidget()),
            &Widget::lineValueChanged,
            this,
            [&](){ bool success;
                   long double n = qobject_cast<Widget*>(parentWidget()->parentWidget())->getLineValue(success);
                   ui->bitArrayWidget->setBits(n, success);

                           });
    connect(ui->bitArrayWidget,
            &BitArray::changed,
            qobject_cast<Widget*>(parentWidget()),
            &Widget::bitsChanged);

    // base combo box
    connect(ui->baseComboBox,
            &QComboBox::currentIndexChanged,
            this, &ProgrammingMode::getBase);
    connect(this, &ProgrammingMode::baseChange,
            qobject_cast<Widget*>(parentWidget()),
            &Widget::changeBase);

    // shift combo box
    for (int i = 1; i < 16; i++) {
        ui->leftShiftComboBox->addItem(QString::number(i) );
        ui->rightShiftComboBox->addItem(QString::number(i) );
    }
    ui->leftShiftComboBox->setCurrentIndex(-1);
    ui->leftShiftComboBox->setMaximumWidth(BUTTON_SIZE);
    ui->leftShiftComboBox->setMinimumWidth(BUTTON_SIZE);
    ui->leftShiftComboBox->setPlaceholderText("<<");
    ui->rightShiftComboBox->setCurrentIndex(-1);
    ui->rightShiftComboBox->setMaximumWidth(BUTTON_SIZE);
    ui->rightShiftComboBox->setMinimumWidth(BUTTON_SIZE);
    ui->rightShiftComboBox->setPlaceholderText(">>");

}

int ProgrammingMode::getBase(int index) {
    int newBase;
    switch(index) {
    case 0:
        newBase = 2;
        break;
    case 1:
        newBase = 8;
        break;
    case 2:
        newBase = 10;
        break;
    case 3:
        newBase = 16;
        break;
    default:
        newBase = 10;
    }
    qDebug() << "new base" << newBase << index;
    emit baseChange(newBase);
    return newBase;
}

void ProgrammingMode::on_leftShiftComboBox_activated(int index)
{
    ui->bitArrayWidget->shift(-(index + 1));
    ui->leftShiftComboBox->setCurrentIndex(-1);
}

void ProgrammingMode::on_rightShiftComboBox_activated(int index)
{
    ui->bitArrayWidget->shift(index + 1);
    ui->rightShiftComboBox->setCurrentIndex(-1);
}
