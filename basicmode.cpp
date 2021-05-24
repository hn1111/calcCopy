#include "basicmode.h"
#include "ui_basicmode.h"

BasicMode::BasicMode(QWidget *parent) :
    ModeBase(parent),
    ui(new Ui::BasicMode)
{
    ui->setupUi(this);
    setup();
}

BasicMode::~BasicMode()
{
    delete ui;
}

void BasicMode::setup()
{
    ModeBase::setup(findChildren<QPushButton*>(QString()));
}

