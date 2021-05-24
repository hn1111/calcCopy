#include "bitarray.h"

BitArray::BitArray(QWidget *parent) : QWidget(parent)
{
    setup();
}

void BitArray::setup()
{
    /*  0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0
     *  63                                 47                               32
     *  0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0  0 0 0 0
     *  31                                 15                                0
     */

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QHBoxLayout *layout1, *layout2, *layout3, *layout4;
    layout1 = new QHBoxLayout();
    layout2 = new QHBoxLayout();
    layout3 = new QHBoxLayout();
    layout4 = new QHBoxLayout();

    // layout 1 - button row
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            BitButton *button = new BitButton(63 - bitArrayButtons.size(),
                                              parentWidget());
            bitArrayButtons.append(button);
            layout1->addWidget(button);
        }
        layout1->addSpacerItem(new QSpacerItem(10,4));

    }
    layout1->setSpacing(2);

    // layout 2 - labels showing order of bit button above
    QLabel *label63 = new QLabel("63", this);
    label63->setAlignment(Qt::AlignLeft);
    QLabel *label47 = new QLabel("47", this);
    label47->setAlignment(Qt::AlignCenter);
    QLabel *label32 = new QLabel("32", this);
    label32->setAlignment(Qt::AlignRight);

    layout2->addWidget(label63);
    layout2->addWidget(label47);
    layout2->addWidget(label32);

    // layout 3 - button row
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            BitButton *button = new BitButton(63 - bitArrayButtons.size(),
                                              parentWidget());
            bitArrayButtons.append(button);

            layout3->addWidget(button);
        }
        layout3->addSpacerItem(new QSpacerItem(10,4));
    }
    layout3->setSpacing(2);

    // layout 4 - labels showing order of bit button above
    QLabel *label31 = new QLabel("31", this);
    label31->setAlignment(Qt::AlignLeft);
    QLabel *label15 = new QLabel("15", this);
    label15->setAlignment(Qt::AlignCenter);
    QLabel *label0 = new QLabel("0", this);
    label0->setAlignment(Qt::AlignRight);

    layout4->addWidget(label31);
    layout4->addWidget(label15);
    layout4->addWidget(label0);


    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);
    mainLayout->addLayout(layout3);
    mainLayout->addLayout(layout4);

    // could use mapper
    for (auto button : bitArrayButtons)
        connect(button, &BitButton::clicked, this, &BitArray::bitsCLicked);
}

void BitArray::block()
{
    for (auto button : bitArrayButtons) {
        button->setDisabled(true);
    }
}

void BitArray::unblock()
{
    for (auto button : bitArrayButtons) {
        button->setDisabled(false);
    }
}


quint64 BitArray::getBitsValue()
{
    quint64 n = 0;
    for (auto b : bitArrayButtons) {
        if (b->getValue())
            n += powl(2, b->getOrder());
    }
    return n;
}

void BitArray::setBits(long double strN, bool success) {
    if (success)
        unblock();
    else {
        block();
        return;
    }
    const long double MAX = ULLONG_MAX;
    if (strN > MAX)
        return;
    int ord = 0;
    unsigned long long int n = strN;
    for (int i = 0; i < 64; i++) {
        bitArrayButtons[63-i]->setValue(n % 2);
        n /= 2;
        ++ord;
    }
}

void BitArray::shift(int bits)
{
    QVector<int> v(64);
    for (int i = 0; i < 64; ++i) {
        if ((i - bits > 63) || (i - bits < 0))
            v[i] = 0;
        else
            v[i] = bitArrayButtons[i - bits]->getValue();
    }
    for (int i = 0; i < 64; ++i)
        bitArrayButtons[i]->setValue(v[i]);
    emit changed();
}

void BitArray::bitsCLicked()
{
    auto button = qobject_cast<BitButton*>(sender());
    button->setValue(button->getValue() ? 0 : 1);
    setBits(getBitsValue(), true);
    emit changed();
}
