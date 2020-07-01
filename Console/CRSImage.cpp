#pragma warning (disable: 4996) 
#include "CRSImage.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string.h>
#include <cmath>
using namespace std;
// Operations
// for() is not very efficient
const char BSQ[] = "BSQ";
const char BIL[] = "BIL";
const char BIP[] = "BIP";
int CRSImage::GetBands() { return m_nBands; }
int CRSImage::GetSamples() { return m_nSamples; }
int CRSImage::GetLines() { return m_nLines; }
unsigned char*** CRSImage::GetDataBuffer() { return m_pppData; }


CRSImage::CRSImage() {
    bs = new Basic_Stats{ NULL,NULL,NULL,NULL };
    hs = new Hist{ 0,NULL,NULL,NULL,NULL };
    hs->DN = 255;
};

CRSImage::~CRSImage() {
    if(bs->max != NULL){
        delete[] bs->max;
        delete[] bs->min;
        delete[] bs->mean;
        delete[] bs->stdev;
    }
    bs->max = NULL;
    if(hs->Npts !=NULL){
        for (int i = 0; i < m_nBands; ++i) {
            delete[] hs->Npts[i];
            delete[] hs->Total[i];
            delete[] hs->Percent[i];
            delete[] hs->AccPct[i];
        }
        delete[] hs->Npts;
        delete[] hs->Total;
        delete[] hs->Percent;
        delete[] hs->AccPct;
    }
    hs->Npts = NULL;
    if(m_pppData != NULL){
        for (int i = 0; i < m_nBands; ++i) {
            for (int j = 0; j < m_nLines; ++j) {
                delete[] m_pppData[i][j];
            }
            delete[] m_pppData[i];
        }
        delete[] m_pppData;
    }
    m_pppData = NULL;
    //TODO 
    //release
};

bool CRSImage::Open(const char* lpstrPath)
{
	//*lpstrPath whether is a path 
	if (NULL == lpstrPath){
		cerr << "Read Path Failed." << endl;
		return false;
	}

	// 1. Read Meta Data
	string strMetaFilePath = lpstrPath;
	int pos = strMetaFilePath.rfind('.');	//0~pos
	if (pos >= 0)
	{
		strMetaFilePath = strMetaFilePath.substr(0, pos);
	}
	strMetaFilePath.append(".hdr");
	if (!ReadMetaData(strMetaFilePath.c_str()))
	{
		cerr << "Read Meta Data Failed." << endl;
		return false;
	}

	// 2. Initialize Buffer
	if (!InitBuffer())
	{
		cerr << "Initialize Buffer Failed." << endl;
		return false;
	}

	// 3. Read File
	if (!ReadImgData(lpstrPath))
	{
		cerr << "Read Image Data Failed." << endl;
		return false;
	}

	return true;
}

bool	CRSImage::ReadMetaData(const char* lpstrMetaFilePath) {
    ifstream    ifs;		//input file stream
    string      strLine;
    string      strSubTxt;
    stringstream    ss;		//string stream

    char sBuff[260];

    ifs.open(lpstrMetaFilePath, ios_base::in);
    if (!ifs.is_open())
        return false;

    while (!ifs.eof())   //end of file
    {
        ifs.getline(sBuff, 260);
        strLine = sBuff;

        ss.clear();
        ss.str(strLine);    //"samples = 640"
        ss >> strSubTxt;

        if (strSubTxt == "samples")
        {
            ss >> strSubTxt >> m_nSamples;
        }
        else if (strSubTxt == "lines")
        {
            ss >> strSubTxt >> m_nLines;
        }
        else if (strSubTxt == "bands")
        {
            ss >> strSubTxt >> m_nBands;
        }
        else if (strSubTxt == "interleave")
        {
            ss >> strSubTxt >> strSubTxt;
            if (strSubTxt == "bsq")
            {
                m_eInterleave = BSQ;
            }
            else if (strSubTxt == "bip")
            {
                m_eInterleave = BIP;
            }
            else if (strSubTxt == "bil")
            {
                m_eInterleave = BIL;
            }
            else
            {
                // blank
            }
        }
        else if (strSubTxt == "data")
        {
            ss >> strSubTxt;
            if (strSubTxt == "type")
            {
                ss >> strSubTxt >> m_nDataType;
            }
        }
        else
        {
            // blank
        }
    }

    return true;
}

bool	CRSImage::InitBuffer(void) { 
    m_pppData = new unsigned char** [m_nBands];

    if (m_pppData != NULL){
        for (int i = 0; i < m_nBands; ++i) {
            m_pppData[i] = new unsigned char* [m_nLines];
            for (int j = 0; j < m_nLines; ++j) {
                m_pppData[i][j] = new unsigned char[m_nSamples];
            }
        }
        return true;
    }
    else {
        cerr << " char*** failed to malloc" << endl;
        return false;
    }
};

bool	CRSImage::ReadImgData(const char* lpstrImgFilePath) {


    FILE* fp;

    fp = fopen(lpstrImgFilePath, "rb");
    if (fp==NULL) {
        cerr << "IMG open fail" << endl;
        return false;
    }
    //roughly check the .img.
    fseek(fp, 0, SEEK_END);
    int IMGSIZE = ftell(fp);
    if (IMGSIZE != m_nBands * m_nLines * m_nSamples) {
        cerr << "IMG is not right" << endl;
        return false;
    }
    fseek(fp, 0, 0);
    if (m_eInterleave == "BSQ") {
        for (int bds = 0; bds < m_nBands; ++bds) {
            for (int i = 0; i < m_nLines; ++i) {
                fread(m_pppData[bds][i], m_nSamples, 1, fp);
            }
        }
    }
    else {
        // may be you should check it before this func ;
        cerr << "NOt BSQ FORMAT" << endl;
        return false;
    }
    fclose(fp);
    return true;

};

void CRSImage::Max(void) {
    //bsq
    unsigned char* arr = new unsigned char[m_nLines] {0};
    unsigned char* arr1 = new unsigned char[m_nBands] {0};
    unsigned char ch = 0;
    unsigned char ch1 = 0;


    for (int k = 0; k < m_nBands; ++k) {
        for (int i = 0; i < m_nLines; ++i) {
            ch = m_pppData[k][i][0];
            for (int j = 1; j < m_nSamples; ++j) {
                if (ch < m_pppData[k][i][j]) {
                    ch = m_pppData[k][i][j];
                }
   
            }
            arr[i] = ch;
        }
        ch1 = arr[0];
        for (int i = 1; i < m_nLines; ++i) {
            if (ch1 < arr[i]) {
                ch1 = arr[i];
            }
        }
        arr1[k] = ch1;
    }
    bs->max = arr1;
    //for (int i = 0; i < m_nBands; ++i) {
    //    printf("%d ", bs->max[i]);
    //}
 
}

void CRSImage::Min(void) {
    //bsq
    unsigned char* arr = new unsigned char[m_nLines] {0};
    unsigned char* arr1 = new unsigned char[m_nBands] {0};
    unsigned char ch = 0;
    unsigned char ch1 = 0;


    for (int k = 0; k < m_nBands; ++k) {
        for (int i = 0; i < m_nLines; ++i) {
            ch = m_pppData[k][i][0];
            for (int j = 1; j < m_nSamples; ++j) {
                if (ch > m_pppData[k][i][j]) {
                    ch = m_pppData[k][i][j];
                }

            }
            arr[i] = ch;
        }
        ch1 = arr[0];
        for (int i = 1; i < m_nLines; ++i) {
            if (ch1 > arr[i]) {
                ch1 = arr[i];
            }
        }
        arr1[k] = ch1;
    }
    delete[] arr;
    bs->min = arr1;
    //for (int i = 0; i < m_nBands; ++i) {
    //    printf("%d ", bs->min[i]);
    //}

}

void CRSImage::Mean(void) {

    //bsq
    double * arows = new double[m_nLines]{0};
    double* mean = new double[m_nBands] {0};


    for (int k = 0; k < m_nBands; ++k) {
        for (int i = 0; i < m_nLines; ++i) {
            for (int j = 0; j < m_nSamples; ++j) {
                arows[i] += m_pppData[k][i][j];
            }
            arows[i] /= m_nSamples;
        }
        for (int t = 0; t < m_nLines; ++t) {
            mean[k] += arows[t];
            arows[t] = 0;
        }
        mean[k] /= m_nLines;
    }
    delete[] arows;
    bs->mean = mean;
    //for (int i = 0; i < m_nBands; ++i) {
    //    printf("%f ", bs->mean[i]);
    //}

}

void CRSImage::Stdev(void) {
    //bsq
    double* arows = new double[m_nLines] {0};
    double* stdev = new double[m_nBands] {0};
    if (bs->mean == NULL) {
        cerr << "Fail: mean is not initial";
        return;
    }

    for (int k = 0; k < m_nBands; ++k) {
        for (int i = 0; i < m_nLines; ++i) {
            for (int j = 0; j < m_nSamples; ++j) {
                arows[i] += pow(m_pppData[k][i][j] - (bs->mean)[k],2);
            }
            //arows[i] /= m_nSamples;
        }
        for (int t = 0; t < m_nLines; ++t) {
            stdev[k] += arows[t];
            arows[t] = 0;
        }
        //stdev[k] /= m_nLines;
        stdev[k] /= (m_nLines * m_nSamples-1);
        stdev[k] = sqrt(stdev[k]);
    }
    delete[] arows;
    bs->stdev = stdev;
    //for (int i = 0; i < m_nBands; ++i) {
    //    printf("%f ", bs->stdev[i]);
    //}
}

void CRSImage::Histogram(void) {
    // not sure DNs are all 0~100
    const int DN = hs->DN; //DN
    unsigned int** Npts = new unsigned int* [m_nBands];
    
    for (int i = 0; i < m_nBands; ++i) {
        Npts[i] = new unsigned int[DN + 1]{0};
    }    

    unsigned int** Total = new unsigned int* [m_nBands];
    double** Percent = new double* [m_nBands];
    double** AccPct = new double* [m_nBands];
    for (int k = 0; k < m_nBands; ++k) {
        for (int i = 0; i < m_nLines; ++i) {
            for (int j = 0; j < m_nSamples; ++j) {
                for (unsigned char t = 0; t <= DN; ++t) {
                    if (t == m_pppData[k][i][j]) {
                        Npts[k][t] += 1;
                        break;
                    }
                }
            }
        }
    }
    for (int i = 0; i < m_nBands; ++i) {
        Total[i] = new unsigned int[DN + 1]{ 0 };
        Total[i][0] = Npts[i][0];
        for (int t = 1; t <= DN; ++t) {
            Total[i][t] += (Npts[i][t] + Total[i][t - 1]);
        }
    }
    for (int i = 0; i < m_nBands; ++i) {
        Percent[i] = new double[DN + 1]{ 0 };
        for (int t = 0; t <= DN; ++t) {
            Percent[i][t] = (100*Npts[i][t]) / (640 * 400.0);
        }
    }
    for (int i = 0; i < m_nBands; ++i) {
        AccPct[i] = new double[DN + 1]{ 0 };
        AccPct[i][0] = Percent[i][0];
        for (int t = 1; t <= DN; ++t) {
            AccPct[i][t] += (Percent[i][t] + AccPct[i][t - 1]);
        }
    }
    hs->Npts = Npts;
    hs->AccPct = AccPct;
    hs->Percent = Percent;
    hs->Total = Total;
    //for (int t = 0; t <=DN; ++t) {
    //    cout << AccPct[0][t] <<"    ";
    //}
}
