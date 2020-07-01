#ifndef IMGDIA_H
#define IMGDIA_H
#include "CRSImage.h"
#include <QDialog>
#include <QPainter>
#include <QImage>
#include <QString>
class ImgDia:public QDialog
{
public:
    ImgDia();
    ~ImgDia();
    QImage *CRSIMG;
    void Inintial(unsigned char** ppRed,unsigned char** ppGrn,unsigned char** ppBlu);
    int width;
    int height;
    QString srcDirPath;
    bool save;

protected:
    void paintEvent(QPaintEvent *);

    unsigned char**r;
    unsigned char**g;
    unsigned char**b;



};

#endif // IMGDIA_H
