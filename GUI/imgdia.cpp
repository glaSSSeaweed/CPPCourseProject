#include "imgdia.h"
#include "CRSImage.h"
#include <QDebug>

ImgDia::ImgDia() : save(false)
{
}
ImgDia::~ImgDia()
{
    delete CRSIMG;
}

void ImgDia::Inintial(unsigned char** ppRed,unsigned char** ppGrn,unsigned char** ppBlu){
    CRSIMG = new QImage(width, height, QImage::Format_RGB888);
    r =ppRed;
    g = ppGrn;
    b = ppBlu;
}

void ImgDia::paintEvent(QPaintEvent *){
    save = true;
    resize(width,height);
    QPainter painter(this);

    for(int i=0; i<width; i++){

        for(int j=0; j<height; j++)

        {
            QRgb rgbPix = qRgb(r[j][i],g[j][i],b[j][i]);
            CRSIMG->setPixel(i,j,rgbPix);
        }
    }
    painter.begin(this);
    painter.drawImage(0,0,*CRSIMG);
    painter.end();
    //文件夹路径

}
