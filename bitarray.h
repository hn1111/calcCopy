#ifndef BITARRAY_H
#define BITARRAY_H

#include <QWidget>
#include <QVBoxLayout>
#include <bitbutton.h>
#include <QLabel>

class BitButton;
class BitArray : public QWidget
{
    Q_OBJECT
public:
    explicit BitArray(QWidget *parent = nullptr);

    quint64 getBitsValue();
    void setBits(long double n, bool success);
    void shift(int);
signals:
    void changed();

public slots:
    void bitsCLicked();

private:
    QList<BitButton*> bitArrayButtons;
    void setup();
    void block();
    void unblock();

};

#endif // BITARRAY_H
