#include "modebase.h"
#include "ui_modebase.h"

ModeBase::ModeBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeBase)
{
    ui->setupUi(this);
}

void ModeBase::activate()
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void ModeBase::deactivate()
{
    // Prevent stacked widget size from effecting main window size
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

ModeBase::~ModeBase()
{
    delete ui;
}

void ModeBase::setup(QList<QPushButton*> lst)
{
    // ignore invisible stacked widgets
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    // buttons
    foreach(auto *button, lst) {
        // set size
        int size;
        if (button->sizePolicy().horizontalStretch())
            size = button->sizePolicy().horizontalStretch() * BUTTON_SIZE;
        else
            size = BUTTON_SIZE;
        button->setMinimumWidth(size);
        button->setMaximumWidth(size);

        // special buttons
        if (button->objectName() == "clearButton")
            connect(button,
                    &QPushButton::clicked,
                    qobject_cast<Widget*>(this->parentWidget()),
                    &Widget::clear);
        else if (button->objectName() == "equalButton")
            connect(button,
                    &QPushButton::clicked,
                    qobject_cast<Widget*>(this->parentWidget()),
                    &Widget::calculate);
        else if (button->objectName() == "backButton")
            connect(button,
                    &QPushButton::clicked,
                    qobject_cast<Widget*>(this->parentWidget()),
                    &Widget::back);
        // buttons for typing one character into lineEdit
        else
            connect(button,
                    &QPushButton::clicked,
                    qobject_cast<Widget*>(this->parentWidget()),
                    &Widget::addPushedButtonText);

        // set special characters as button text
        if (button->objectName() == "backButton")
            button->setText(QString(ARROW));
        if (button->objectName() == "multiplicationButton")
            button->setText(QString(MULTIPLICATION));
        if (button->objectName() == "sqrtButton")
            button->setText(QString(SQUARE_ROOT));
        if (button->objectName() == "divisionButton")
            button->setText(QString(DIVISION));
    }
}
