#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "CRSImage.h"
#include <QMessageBox>
#include "imgdia.h"
#include "histdia.h"

struct matrix{
    float a11, a12, a13;
    float  a21, a22, a23;
    float  a31, a32, a33;
};

QT_BEGIN_NAMESPACE
namespace Ui { class mainWidget; }
QT_END_NAMESPACE

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    mainWidget(QWidget *parent = nullptr);
    ~mainWidget();
private:
    void OpenIMG();
    void Information();
    void Statistics();
    void Histogram();
    void Write();
    void Closed();
    void DisplayImage();
    void ScaleImage();
    void RotateImage();
    void FilterImage();
    void VisualHist();
    void Save();
    Ui::mainWidget *ui;
    QMessageBox *msg;
    CRSImage *test;
    ImgDia *dd;
    HistDia *hh;

    float rat;
    float deg;
    unsigned char*** dst;
    unsigned char*** dst1;
    unsigned char*** dst2;
    struct matrix* matarr[4];
    static int counter;

};
#endif // MAINWIDGET_H
