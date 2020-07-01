#include "histdia.h"
#include <QPainter>
HistDia::HistDia() : num(0)
{
//    hsData = new CRSImage();
//    hsData->Open("C:\\Users\\Administrator\\Desktop\\cppkeshe\\test.img");
//    hsData->Histogram();
}
void HistDia::paintEvent(QPaintEvent *){
        //resize(2800,800);
        QPainter painter(this);
        painter.setPen(QColor(0,0,0));

        painter.drawLine(50,600,1060,600);     // x: 800
        painter.drawLine(50,600,50,100);      // y: 500
        painter.drawLine(1055,595,1060,600);    //arrow x
        painter.drawLine(1055,605,1060,600);
        painter.drawLine(50,100,45,105);      //arrow y
        painter.drawLine(50,100,55,105);

        int ex = 10;            // unit x
        int ey  = 50;           // unit y
        int u = 3;              //mark size
        //draw y mark
        for(int i=0;i<=10;i++)
        {
            painter.drawLine(50,600-ey*i,50+u,600-ey*i);    //draw line
            painter.drawText(QPoint(40,603-ey*i),QString::number(i));   //draw number
        }
        //draw x mark
        for(int i=1;i<=100;i++)
        {
            painter.drawLine(50 + ex*i,600,50 + ex*i,603);   //draw line
            if(i%10 == 0)
                painter.drawText(QPoint(50 + ex*i,612),QString::number(i));     //draw number
        }
        painter.setBrush(QColor(62,147,192));
        for(int i=0;i<=100;i++)
        {
            painter.drawRect(50 + ex*i,600,10, -hsData->hs->Percent[num][i]*ey);
        }


}

