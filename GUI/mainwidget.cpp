#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QBuffer>
#include <QMessageBox>
#include "CRSImage.h"
#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <qglobal.h>
#include <QFileDialog>
#include <QString>
#include <QDebug>
using namespace std;

int mainWidget::counter = 0;
//"############ Remote Sensing Image Process Tools.############\n";
//"# O – Open \t打开影像文件"
//"# C –Closed\t关闭当前图像"
//"# I – Information\t输出当前图像的路径 ，行列值 、波段数 、数据类 数据类 型、排列方式等信息"
//"# S – Statistics\t输出图像数据统计量"
//"# H – Histogram\t输出图像的直方图"
//"# W – Write\t将图像每个波段的最大值、最小值、均值、方差、直方图保存到文本中"
//"# D – Display \t显示影像文件"
//"# Z – Zoom\t图像缩放，输入比例尺出"
//"# R – Rotate\t图像旋转，输入角度逆时针"
//"# F - Filter\t输入滤波类型(1,2,3,4)，输出滤波后图像"
//"# A – Save as\t输入保存的文件路径 ，输出图像为二进制文件"

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    msg = new QMessageBox();
    test = new CRSImage;
    dd = new ImgDia();
    hh = new HistDia();
    hh->hsData =test;

    dst = new unsigned char** [3];
    dst1 = new unsigned char** [3];
    dst2 = new unsigned char** [3];
    dst[0] = nullptr;
    dst1[0] = nullptr;
    dst2[0] = nullptr;
    matarr[0] = new matrix {1/16.0,1/8.0,1/16.0,1/8.0,1/4.0,1 / 8.0,1 / 16.0,1 / 8.0,1 / 16.0 };
    matarr[1] = new matrix{ 0,1,0,0,-4,0,0,1,0 };
    matarr[2] = new matrix{1,1,1,1,-8,1,1,1,1};
    matarr[3] = new matrix{ 0,1,0,1,-4,1,0,1,0 };

    //test->Open("C:\\Users\\Administrator\\Desktop\\cppkeshe\\test.img");
    ui->setupUi(this);
    rat = 0;
    connect(ui->pushButton,&QPushButton::clicked,this,&mainWidget::OpenIMG);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&mainWidget::Information);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&mainWidget::Statistics);
    connect(ui->pushButton_4,&QPushButton::clicked,this,&mainWidget::Histogram);
    connect(ui->pushButton_5,&QPushButton::clicked,this,&mainWidget::Write);
    connect(ui->pushButton_6,&QPushButton::clicked,this,&mainWidget::Closed);
    connect(ui->pushButton_7,&QPushButton::clicked,this,&mainWidget::DisplayImage);
    connect(ui->pushButton_8,&QPushButton::clicked,this,&mainWidget::ScaleImage);
    connect(ui->pushButton_9,&QPushButton::clicked,this,&mainWidget::RotateImage);
    connect(ui->pushButton_10,&QPushButton::clicked,this,&mainWidget::FilterImage);
    connect(ui->pushButton_11,&QPushButton::clicked,this,&mainWidget::VisualHist);
    connect(ui->pushButton_12,&QPushButton::clicked,this,&mainWidget::Save);
}
void mainWidget::Save(){

        if(dd->save == true){

            QString srcDirPath = QFileDialog::getExistingDirectory(
                       this, "choose src Directory",
                        "/");
            dd->CRSIMG->save(srcDirPath+"/"+QString::number(counter,10)+".jpg", "JPG", 100);
            ++counter;
        }else{
            msg->question(this,"Error:|Save","NO IMG File Display",0,
                          QMessageBox::NoButton,
                          QMessageBox::Ok);

        }
}
mainWidget::~mainWidget()
{
    int nLines = test->GetLines();

        if(dst2[0] != nullptr){
            for(int k = 0 ; k < 3 ; ++k){
                for(int i = 0; i < nLines;++i){
                    delete[] dst2[k][i];
                }
                delete[] dst2[k];
            }
            dst2[0] = nullptr;
        }
    if(dst1[0] != nullptr){
        for(int k = 0 ; k < 3 ; ++k){
            for(int i = 0; i < nLines;++i){
                delete[] dst1[k][i];
            }
            delete[] dst1[k];
        }
        dst1[0] = nullptr;
    }
    if(dst[0] != nullptr){
        for(int k = 0 ; k < 3 ; ++k){
            for(int i = 0; i < (int)(nLines * rat);++i){
                delete[] dst[k][i];
            }
            delete[] dst[k];
        }
        dst[0] = nullptr;
    }

    delete ui;
    delete test;

}
void mainWidget::VisualHist(){
    if (test->FileStatus == true) {
        if(test->hs->Npts != nullptr){
            int num = ui->lineEdit_4->text().toInt();
            if(num < test->GetBands()){
                hh->num = num;
                hh->show();
            }else{
                msg->question(this,"Error:|VisualHist","Band not right",0,
                              QMessageBox::NoButton,
                              QMessageBox::Ok);
            }
        }else{
            msg->question(this,"Error:|VisualHist","Histgram isn't calced",0,
                          QMessageBox::NoButton,
                          QMessageBox::Ok);
        }
    }else{
        msg->question(this,"Error:|VisualHist","NO IMG File opened",0,
                      QMessageBox::NoButton,
                      QMessageBox::Ok);

    }
}
void mainWidget::FilterImage(){
        if (test->FileStatus == true) {
            //TODO num judgement
            int num = 0;
            num = ui->lineEdit_3->text().toInt();
            struct matrix *mat;
            switch(num){
            case 1:
                mat = matarr[0];
                break;
            case 2:
                mat = matarr[1];
                break;
            case 3:
                mat = matarr[2];
                break;
            case 4:
                mat = matarr[3];
                break;
            default:
                msg->question(this,"Error:|FilterImage","Number not fit",0,
                              QMessageBox::NoButton,
                              QMessageBox::Ok);
                return;
            }

            int		nSamples = test->GetSamples();
            int		nLines = test->GetLines();
            if(dst2[0] != nullptr){
                for(int k = 0 ; k < 3 ; ++k){
                    for(int i = 0; i < nLines;++i){
                        delete[] dst2[k][i];
                    }
                    delete[] dst2[k];
                }
                dst2[0] = nullptr;
            }



        unsigned char** ppRed = test->m_pppData[0];
        unsigned char** ppGrn =test->m_pppData[1];
        unsigned char** ppBlu = test->m_pppData[2];
        for (int i = 0; i < 3; ++i) {
            dst2[i] = new unsigned char* [nLines];
            for (int j = 0; j < nLines; ++j) {
                dst2[i][j] = new unsigned char[nSamples];
            }
        }

        for (int x = 0; x < nSamples - 1; x++) {
            for (int y = 1; y < nLines - 1; y++) {
                dst2[0][y][x] =
                    (ppRed[y - 1][x - 1]* (mat->a11)) + (ppRed[y-1][x] * (mat->a12)) + (ppRed[y - 1][x + 1] * (mat->a13)) +
                    (ppRed[y][x-1] * (mat->a21)) + (ppRed[y][x] * (mat->a22)) + (ppRed[y][x+1] * (mat->a23)) +
                    (ppRed[y+1][x - 1] * (mat->a31)) + (ppRed[y+1][x] * (mat->a32)) + (ppRed[y + 1][x + 1] * (mat->a33));
                dst2[1][y][x] =
                        (ppGrn[y - 1][x - 1]* (mat->a11)) + (ppGrn[y-1][x] * (mat->a12)) + (ppGrn[y - 1][x + 1] * (mat->a13)) +
                        (ppGrn[y][x-1] * (mat->a21)) +      (ppGrn[y][x] * (mat->a22)) +   (ppGrn[y][x+1] * (mat->a23)) +
                        (ppGrn[y+1][x - 1] * (mat->a31)) +  (ppGrn[y+1][x] * (mat->a32)) + (ppGrn[y + 1][x + 1] * (mat->a33));
                dst2[2][y][x] =
                        (ppBlu[y - 1][x - 1]* (mat->a11)) + (ppBlu[y-1][x] * (mat->a12)) + (ppBlu[y - 1][x + 1] * (mat->a13)) +
                        (ppBlu[y][x-1] * (mat->a21)) +      (ppBlu[y][x] * (mat->a22)) +   (ppBlu[y][x+1] * (mat->a23)) +
                        (ppBlu[y+1][x - 1] * (mat->a31)) +  (ppBlu[y+1][x] * (mat->a32)) + (ppBlu[y + 1][x + 1] * (mat->a33));

            }
        }

        dd->width = nSamples;
        dd->height = nLines;
        dd->Inintial(dst2[0],dst2[1],dst2[2]);
        dd->show();
    }else{
        msg->question(this,"Error:|Display","NO IMG File opened",0,
                      QMessageBox::NoButton,
                      QMessageBox::Ok);

    }
}
void mainWidget::RotateImage(){
    if (test->FileStatus == true) {
        //TODO degree judgement
        deg = ui->lineEdit_2->text().toFloat();
        int		nSamples = test->GetSamples();
        int		nLines = test->GetLines();
        if(deg == 0){
            msg->question(this,"Error:|RotateImage","you can use Display",0,
                          QMessageBox::NoButton,
                          QMessageBox::Ok);
            return;
        }
        if(deg > 40){
            msg->question(this,"Error:|RotateImage","bro, your head is ok?",0,
                          QMessageBox::NoButton,
                          QMessageBox::Ok);
            return;
        }
        if(dst1[0] != nullptr){
            for(int k = 0 ; k < 3 ; ++k){
                for(int i = 0; i < nLines;++i){
                    delete[] dst1[k][i];
                }
                delete[] dst1[k];
            }
            dst1[0] = nullptr;
        }


        unsigned char** ppRed = test->m_pppData[0];
        unsigned char** ppGrn =test->m_pppData[1];
        unsigned char** ppBlu = test->m_pppData[2];

        for (int i = 0; i < 3; ++i) {
            dst1[i] = new unsigned char* [nLines];
            for (int j = 0; j < nLines; ++j) {
                dst1[i][j] = new unsigned char[nSamples]{0};
            }
        }
        unsigned char rr, gg, bb;
        unsigned char r11, r12, r21, r22;
        unsigned char g11, g12, g21, g22;
        unsigned char b11, b12, b21, b22;
        int a, b;
        double xx, yy;
        double rx0 = nSamples * 0.5;
        double ry0 = nLines * 0.5;
        for (int x = 0; x < nSamples; x++) {

            for (int y = 0; y < nLines; y++) {
                xx = (x - rx0) * cos(deg) - (y - ry0) * sin(deg) + rx0;
                yy = (x - rx0) * sin(deg) + (y - ry0) * cos(deg) + ry0;
                a = (int)xx;
                b = (int)yy;

                if ((a == nSamples-1) || (b == nLines-1)) {
                    dst1[0][y][x] = ppRed[b][a];
                    dst1[1][y][x] = ppGrn[b][a];
                    dst1[2][y][x] = ppBlu[b][a];
                }


                if ( ((a < nSamples-1) && a>=0) && ((b< nLines-1) && b>=0)){
                    r11 = ppRed[b][a];
                    r12 = ppRed[b][a + 1];
                    r21 = ppRed[b + 1][a];
                    r22 = ppRed[b + 1][a + 1];
                    rr = (unsigned char)(r11 * (a + 1 - xx) * (b + 1 - yy) + r12 * (a + 1 - xx) * (yy - b)
                        + r21 * (xx - a) * (b + 1 - yy) + r22 * (xx - a) * (yy - b));

                    g11 = ppGrn[b][a];
                    g12 = ppGrn[b][a + 1];
                    g21 = ppGrn[b + 1][a];
                    g22 = ppGrn[b + 1][a + 1];
                    gg = (unsigned char)(g11 * (a + 1 - xx) * (b + 1 - yy) + g12 * (a + 1 - xx) * (yy - b)
                        + g21 * (xx - a) * (b + 1 - yy) + g22 * (xx - a) * (yy - b));

                    b11 = ppBlu[b][a];
                    b12 = ppBlu[b][a + 1];
                    b21 = ppBlu[b + 1][a];
                    b22 = ppBlu[b + 1][a + 1];
                    bb = (unsigned char)(b11 * (a + 1 - xx) * (b + 1 - yy) + b12 * (a + 1 - xx) * (yy - b)
                        + b21 * (xx - a) * (b + 1 - yy) + b22 * (xx - a) * (yy - b));
                    dst1[0][y][x] = rr;
                    dst1[1][y][x] = gg;
                    dst1[2][y][x] = bb;
                }
            }
        }
        dd->width = nSamples;
        dd->height = nLines;
        dd->Inintial(dst1[0],dst1[1],dst1[2]);
        dd->show();
    }else{
        msg->question(this,"Error:|Display","NO IMG File opened",0,
                      QMessageBox::NoButton,
                      QMessageBox::Ok);

    }
}
void mainWidget::ScaleImage(){
    if (test->FileStatus == true) {
        //TODO rat judgement
        float r =rat;
        rat = ui->lineEdit->text().toFloat();
        if(rat <= 0.01){
            msg->question(this,"Error:|ScaleImage","ARE U Leeuwenhoek?",0,
                          QMessageBox::NoButton,
                          QMessageBox::Ok);
            return;
        }
        if(rat >= 5){
            msg->question(this,"Error:|ScaleImage","ohh~~,So big I can take it any more",0,
                          QMessageBox::NoButton,
                          QMessageBox::Ok);
            return;
        }
        int		nSamples = test->GetSamples();
        int		nLines = test->GetLines();
        if(dst[0] != nullptr){
            for(int k = 0 ; k < 3 ; ++k){
                for(int i = 0; i < (int)(nLines * r);++i){
                    delete[] dst[k][i];
                }
                delete[] dst[k];
            }
            dst[0] = nullptr;
        }


        int widthNew, heightNew;
        unsigned char **ppRed = test->m_pppData[0];
        unsigned char **ppGrn = test->m_pppData[1];
        unsigned char **ppBlu = test->m_pppData[2];
        float widthScale = (float)(1.0 / rat), heightScale = (float)(1.0 / rat);
        widthNew = (int)(nSamples * rat);
        heightNew = (int)(nLines * rat);
        float xx, yy;
        int a, b;
        unsigned char rr, gg, bb;
        unsigned char r11, r12, r21, r22;
        unsigned char g11, g12, g21, g22;
        unsigned char b11, b12, b21, b22;
        for (int i = 0; i < 3; ++i) {
            dst[i] = new unsigned char* [heightNew];
            for (int j = 0; j < heightNew; ++j) {
                dst[i][j] = new unsigned char[widthNew];
            }
        }
        for (int x = 0; x < widthNew; x++) {
            for (int y = 0; y < heightNew; y++) {
                xx = x * widthScale;
                yy = y * heightScale;
                if (xx <= 1e-8)
                    xx = 0;
                if (xx > nSamples - 2)
                    xx = (float)(nSamples - 2);
                if (yy <= 1e-8)
                    yy = 0;
                if (yy > nLines - 2)
                    yy = (float)(nLines - 2);
                a = (int)xx;
                b = (int)yy;
                r11 = ppRed[b][a];
                r12 = ppRed[b][a + 1];
                r21 = ppRed[b + 1][a];
                r22 = ppRed[b + 1][a + 1];
                rr = (unsigned char)(r11 * (a + 1 - xx) * (b + 1 - yy) + r12 * (a + 1 - xx) * (yy - b)
                    + r21 * (xx - a) * (b + 1 - yy) + r22 * (xx - a) * (yy - b));

                g11 = ppGrn[b][a];
                g12 = ppGrn[b][a + 1];
                g21 = ppGrn[b + 1][a];
                g22 = ppGrn[b + 1][a + 1];
                gg = (unsigned char)(g11 * (a + 1 - xx) * (b + 1 - yy) + g12 * (a + 1 - xx) * (yy - b)
                    + g21 * (xx - a) * (b + 1 - yy) + g22 * (xx - a) * (yy - b));

                b11 = ppBlu[b][a];
                b12 = ppBlu[b][a + 1];
                b21 = ppBlu[b + 1][a];
                b22 = ppBlu[b + 1][a + 1];
                bb = (unsigned char)(b11 * (a + 1 - xx) * (b + 1 - yy) + b12 * (a + 1 - xx) * (yy - b)
                    + b21 * (xx - a) * (b + 1 - yy) + b22 * (xx - a) * (yy - b));
                dst[0][y][x] = (unsigned char)qMin(255, (int)rr);
                dst[1][y][x] = (unsigned char)qMin(255, (int)gg);
                dst[2][y][x] = (unsigned char)qMin(255, (int)bb);
            }
        }
        dd->width = widthNew;
        dd->height = heightNew;
        dd->Inintial(dst[0],dst[1],dst[2]);
        dd->show();
    }else{
        msg->question(this,"Error:|Display","NO IMG File opened",0,
                      QMessageBox::NoButton,
                      QMessageBox::Ok);

    }
}

void mainWidget::DisplayImage(){
    if (test->FileStatus == true) {
        dd->width = test->m_nSamples;
        dd->height = test->m_nLines;
        dd->Inintial(test->m_pppData[0],test->m_pppData[1],test->m_pppData[2]);
        dd->show();
    }else{
        msg->question(this,"Error:|Display","NO IMG File opened",0,
                      QMessageBox::NoButton,
                      QMessageBox::Ok);

    }
}

void mainWidget::Write(){
    if (test->FileStatus == true && (test->hs->Npts !=NULL) && (test->bs->max != NULL)) {
        ofstream ofs;
        ofs.open("data.doc", ios::out);
        ofs << setw(10) <<
            "Basic Stats"
            << setw(10) << "Min" << "\t"
            << setw(10) << "Max" << "\t"
            << setw(10) << "Mean" << "\t"
            << setw(10) << "Stdev" << endl;
        for (int i = 0; i < test->GetBands(); ++i) {
            ofs << setw(10) <<
                "Band " << i + 1
                << setw(10) << (int)test->bs->min[i] << "\t"
                << setw(10) << (int)test->bs->max[i] << "\t"
                << setw(10) << test->bs->mean[i] << "\t"
                << setw(10) << test->bs->stdev[i] << endl;
        }
        ofs << setw(10) <<
            "Histogram"
            << setw(10) << "DN" << "\t"
            << setw(10) << "Npts" << "\t"
            << setw(10) << "Total" << "\t"
            << setw(10) << "Percent" << "\t"
            << setw(10) << "Acc Pct" << "\t" << endl;
        for (int i = 0; i < test->GetBands(); ++i) {
            for (int j = 0; j < (test->hs->DN + 1); ++j) {
                if (j == 0) {
                    ofs << setw(10) << "Band " << i + 1;
                }
                else {
                    ofs << setw(10) << " " ;
                }
                ofs << setw(10) <<j << "\t"
                    << setw(10) << (int)test->hs->Npts[i][j] << "\t"
                    << setw(10) << (int)test->hs->Total[i][j] << "\t"
                    << setw(10) << test->hs->Percent[i][j] << "\t"
                    << setw(10) << test->hs->AccPct[i][j] << endl;
            }

        }
        ofs.close();
    }else{
        msg->question(this,"Error:|Write","|NO IMG File opened\n|Statistics and Histogram needs to calc first",0,
                      QMessageBox::NoButton,
                      QMessageBox::Ok);

    }
}
void mainWidget::OpenIMG(){
    if (test->FileStatus == false) {
        QString path = QFileDialog::getOpenFileName(this,
                 "open", "/"  );
        string pp = path.toStdString();
        const char *FileName = pp.c_str();
        if(test->Open(FileName)){
            test->FileStatus = true;
        }
        else{
            //TODO
                    msg->question(this,"Error:OpenIMG","|IMG File opened failed\n|PLEASE OPEN .IMG",0,
                                  QMessageBox::NoButton,
                                  QMessageBox::Ok);
        }
    }else{
        msg->question(this,"Error:OpenIMG","IMG File has been opened",0,
                      QMessageBox::NoButton,
                      QMessageBox::Ok);

    }
}
void mainWidget::Information(){
    if (test->FileStatus == true) {
        ofstream ofs;
        ofs.open("Information.txt", ios::out);
        ofs << "bands "	<<test->m_nBands << endl
            << "lines " << test->m_nLines << endl
            << "samples " << test->m_nSamples << endl
            << "interleave " << test->m_eInterleave << endl
            << "data type " << test->m_nDataType<<endl;
        ofs.close();
        QFile file;
        file.setFileName("Information.txt");

        bool isOpen = file.open(QIODevice::ReadOnly);
        if(true == isOpen)
        {
            QTextStream stream(&file);
            //stream.setCodec("UTF-8");
            QString str = stream.readAll();
            ui->textEdit->setText(str);
            file.close();
        }else{
            msg->question(this,"Error:Information","PLEASE RETRY",0,
                          QMessageBox::NoButton,
                          QMessageBox::Ok);

        }
    }else{
        msg->question(this,"Error:Information","NO IMG File opened",0,
                      QMessageBox::NoButton,
                      QMessageBox::Ok);

    }

}
void mainWidget::Statistics(){
    if (test->FileStatus == true) {
        ofstream ofs;
        ofs.open("BasicData.txt", ios::out);
        test->Min();
        test->Max();
        test->Mean();
        test->Stdev();
        ofs << setw(12) <<
            "Basic Stats"<<"\t"
            << setw(10) <<"Min"<<"\t"
            << setw(10) <<"Max"<<"\t"
            << setw(10) <<"Mean"<<"\t"
            << setw(10) <<"Stdev" << endl;
        for(int i = 0;i< test->GetBands();++i){
            ofs << setw(12) <<
                "Band "<<i+1<< "\t"
                << setw(10) << (int)test->bs->min[i] << "\t"
                << setw(10) << (int)test->bs->max[i] << "\t"
                << setw(10) << test->bs->mean[i] << "\t"
                << setw(10) << test->bs->stdev[i] << endl;
        }
        ofs.close();
        QFile file;
        file.setFileName("BasicData.txt");
        //TODO//if not alert a dialog
        bool isOpen = file.open(QIODevice::ReadOnly);
        if(true == isOpen)
        {
            QTextStream stream(&file);
            QString str = stream.readAll();
            ui->textEdit->setText(str);
            file.close();
        }else{
            msg->question(this,"Error:Statistics","PLEASE RETRY",0,
                          QMessageBox::NoButton,
                          QMessageBox::Ok);
        }

    }else{
        msg->question(this,"Error:Statistics","NO IMG File opened",0,
                      QMessageBox::NoButton,
                      QMessageBox::Ok);

    }
}
void mainWidget::Histogram(){
    if (test->FileStatus == true) {
        ofstream ofs;
        ofs.open("Histogram.txt", ios::out);
        test->Histogram();
        test->Histogram();
        ofs << setw(12) <<
            "Histogram" << "\t"
            << setw(10) << "DN" << "\t"
            << setw(10) << "Npts" << "\t"
            << setw(10) << "Total" << "\t"
            << setw(10) << "Percent" << "\t"
            << setw(10) << "Acc Pct" << "\t" << endl;
        for (int i = 0; i < test->GetBands(); ++i) {
            for (int j = 0; j < (test->hs->DN + 1); ++j) {
                if (j == 0) {
                    ofs << setw(12) << "Band " << i + 1 << "\t";
                }
                else {
                    ofs << setw(12) << " " << "\t";
                }
                ofs << setw(10) << j << "\t"
                    << setw(10) << (int)test->hs->Npts[i][j] << "\t"
                    << setw(10) << (int)test->hs->Total[i][j] << "\t"
                    << setw(10) << test->hs->Percent[i][j] << "\t"
                    << setw(10) << test->hs->AccPct[i][j] << endl;
            }

        }
        ofs.close();
        QFile file;
        file.setFileName("Histogram.txt");
        //TODO//if not alert a dialog
        bool isOpen = file.open(QIODevice::ReadOnly);
        if(true == isOpen)
        {
            QTextStream stream(&file);
            QString str = stream.readAll();
            ui->textEdit->setText(str);
            file.close();
        }else{
            msg->question(this,"Error:Histogram","PLEASE RETRY",0,
                          QMessageBox::NoButton,
                          QMessageBox::Ok);

        }
    }else{
        msg->question(this,"Error:Histogram","NO IMG File opened",0,
                      QMessageBox::NoButton,
                      QMessageBox::Ok);

    }


}
void mainWidget::Closed(){
    if (test->FileStatus == true) {
        test->~CRSImage();
        test->FileStatus = false;
    }else{
        msg->question(this,"Error:Closed","NO IMG File opened",0,
                      QMessageBox::NoButton,
                      QMessageBox::Ok);

    }
}

