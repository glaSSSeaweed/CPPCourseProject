#ifndef HISTDIA_H
#define HISTDIA_H

#include <QDialog>
#include "CRSImage.h"
class HistDia:public QDialog
{
public:
    HistDia();
    CRSImage *hsData;
    int num;
protected:
    void paintEvent(QPaintEvent *);
    void calc();
};

#endif // HISTDIA_H
